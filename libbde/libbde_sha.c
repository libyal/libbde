/*
 * SHA-256 functions
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#endif

#include "libbde_sha.h"

#if defined( WINAPI )

#if !defined( PROV_RSA_AES )
#define PROV_RSA_AES		24
#endif

#endif /* defined( WINAPI ) */

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_SHA_H ) )

#define libbde_sha256_f1( value ) \
	( byte_stream_bit_rotate_right( value, 2 ) \
	^ byte_stream_bit_rotate_right( value, 13 ) \
	^ byte_stream_bit_rotate_right( value, 22 ) )

#define libbde_sha256_f2( value ) \
	( byte_stream_bit_rotate_right( value, 6 ) \
	^ byte_stream_bit_rotate_right( value, 11 ) \
	^ byte_stream_bit_rotate_right( value, 25 ) )

#define libbde_sha256_f3( value ) \
	( byte_stream_bit_rotate_right( value, 7 ) \
	^ byte_stream_bit_rotate_right( value, 18 ) \
	^ byte_stream_bit_shift_right( value, 3 ) )

#define libbde_sha256_f4( value ) \
	( byte_stream_bit_rotate_right( value, 17 ) \
	^ byte_stream_bit_rotate_right( value, 19 ) \
	^ byte_stream_bit_shift_right( value, 10 ) )

/* The first 32-bits of the fractional parts of the square roots of the first 8 primes 2..19
 */
uint32_t libbde_sha256_prime_square_roots[ 8 ] = {
	0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL, 0xa54ff53aUL,
	0x510e527fUL, 0x9b05688cUL, 0x1f83d9abUL, 0x5be0cd19UL
};

/* The first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311
 */
uint32_t libbde_sha256_prime_cube_roots[ 64 ] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#endif /* !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_SHA_H ) ) */

/* Initializes the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_initialize(
     libbde_sha256_context_t *context,
     liberror_error_t **error )
{
	static char *function = "libbde_sha256_initialize";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	context->crypt_provider = 0;
	context->hash           = 0;

	/* Request the AES crypt provider, fail back to the RSA crypt provider
	*/
 	if( CryptAcquireContext(
	     &( context->crypt_provider ),
	     NULL,
	     NULL,
	     PROV_RSA_AES,
	     CRYPT_VERIFYCONTEXT ) == 0 )
	{
		if( CryptAcquireContext(
		     &( context->crypt_provider ),
		     NULL,
		     NULL,
		     PROV_RSA_FULL,
		     CRYPT_VERIFYCONTEXT ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create AES or RSA crypt provider.",
			 function );

			return( -1 );
		}
	}
	if( context->crypt_provider == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to create crypt provider.",
		 function );

		return( -1 );
	}
	if( CryptCreateHash(
	     context->crypt_provider,
	     CALG_SHA256,
	     0,
	     0,
	     &context->hash ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash object.",
		 function );

		CryptReleaseContext(
		 context->crypt_provider,
		 0 );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestInit_ex(
	     context,
	     EVP_sha256(),
	     NULL ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	SHA256_Init( context );
#else
	if( memory_set(
	     context,
	     0,
	     sizeof( libbde_sha256_context_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     context->hash,
	     libbde_sha256_prime_square_roots,
	     sizeof( uint32_t ) * 8 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to copy initial hash values.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Updates the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_update(
     libbde_sha256_context_t *context,
     const uint8_t *buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function = "libbde_sha256_update";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
	if( CryptHashData(
	     context->hash,
	     (BYTE *) buffer,
	     (DWORD) size,
	     0 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update hash.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestUpdate(
	     context,
	     (const void *) buffer,
	     (unsigned long) size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	SHA256_Update( context, buffer, size );

#else
/* TODO fallback */
#error TODO implement fallback function
{
	const uint8_t *shifted_data = NULL;
	size_t available_block_size = 0;
	size_t new_size             = 0;

    unsigned int block_nb;

	available_block_size = SHA256_BLOCK_SIZE - context->block_index;

	if( available_block_size < size )
	{
		available_block_size = size;
	}
	if( memory_copy(
	     &( context->block[ context->block_index ] ),
	     data,
	     available_block_size ) == NULL )
	{
/* TODO */
	}
	if( context->block_index + size < SHA256_BLOCK_SIZE )
	{
		context->block_index += size;

		return;
	}

    new_size = size - available_block_size;
    block_nb = new_size / SHA256_BLOCK_SIZE;

    shifted_data = data + available_block_size;

    sha256_transf(context, context->block, 1);
    sha256_transf(context, shifted_data, block_nb);

    available_block_size = new_size % SHA256_BLOCK_SIZE;

    memcpy(context->block, &shifted_data[block_nb << 6],
           available_block_size);

	context->block_index = available_block_size;
    context->total_size += (block_nb + 1) << 6;
}
#endif
	return( 1 );
}

/* Finalizes the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_finalize(
     libbde_sha256_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error )
{
	static char *function       = "libbde_sha256_finalize";
#if defined( WINAPI )
	DWORD safe_hash_size        = 0;
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	unsigned int safe_hash_size = 0;
#else
	size_t safe_hash_size       = 0;
#endif

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( hash_size > (size_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
	if( hash != NULL )
	{
		safe_hash_size = (DWORD) hash_size;

		if( CryptGetHashParam(
		     context->hash,
		     HP_HASHVAL,
		     (BYTE *) hash,
		     &safe_hash_size,
		     0 ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize hash.",
			 function );

			return( -1 );
		}
	}
	if( context->crypt_provider != 0 )
	{
		CryptReleaseContext(
		 context->crypt_provider,
		 0 );
	}
	if( context->hash != 0 )
	{
		CryptDestroyHash(
		 context->hash );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( hash != NULL )
	{
		if( hash_size > (size_t) UINT_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid hash size value exceeds maximum.",
			 function );

			return( -1 );
		}
		safe_hash_size = (unsigned int) hash_size;

		if( EVP_DigestFinal_ex(
		     context,
		     (unsigned char *) hash,
		     &safe_hash_size ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize context.",
			 function );

			return( -1 );
		}
	}
	if( EVP_MD_CTX_cleanup(
	     context ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clean up context.",
		 function );
	}
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	if( hash != NULL )
	{
		if( hash_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid hash size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( hash_size < (size_t) LIBBDE_SHA256_HASH_SIZE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid hash size value too small.",
			 function );

			return( -1 );
		}
		SHA256_Final( hash, context );
	}
#else
/* TODO fallback */
#error TODO implement fallback function
{
	unsigned int block_nb = 0;
	unsigned int pm_len   = 0;
	unsigned int len_b    = 0;

#if !defined( LIBBDE_SHA_UNROLLED_LOOPS )
	int i = 0;
#endif

	block_nb = ( 1 + ( ( SHA256_BLOCK_SIZE - 9 )
	         < ( context->block_index % SHA256_BLOCK_SIZE ) ) );

	len_b = (context->total_size + context->block_index) << 3;

	pm_len = block_nb << 6;

	memset(
	 context->block + context->block_index,
	 0,
	 pm_len - context->block_index );

	context->block[ context->block_index ] = 0x80;

	UNPACK32(len_b, context->block + pm_len - 4);

	sha256_transf(
	 context,
	 context->block,
	 block_nb );

#if !defined( LIBBDE_SHA_UNROLLED_LOOPS )
	for( i = 0;
	     i < 8;
	     i++ )
	{
		UNPACK32(context->hash[i], &digest[i << 2]);
	}
#else
	UNPACK32(context->hash[ 0 ], &digest[ 0 ]);
	UNPACK32(context->hash[ 1 ], &digest[ 4 ]);
	UNPACK32(context->hash[ 2 ], &digest[ 8 ]);
	UNPACK32(context->hash[ 3 ], &digest[ 12 ]);
	UNPACK32(context->hash[ 4 ], &digest[ 16 ]);
	UNPACK32(context->hash[ 5 ], &digest[ 20 ]);
	UNPACK32(context->hash[ 6 ], &digest[ 24 ]);
	UNPACK32(context->hash[ 7 ], &digest[ 28 ]);
#endif /* !defined( LIBBDE_SHA_UNROLLED_LOOPS ) */
}
#endif
	return( 1 );
}

/* Calculates the SHA256 digest hash of the data
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error )
{
	libbde_sha256_context_t context;

	static char *function = "libbde_sha256_initialize";

	if( libbde_sha256_initialize(
	     &context,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize context.",
		 function );

		return( -1 );
	}
	if( libbde_sha256_update(
	     &context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		libbde_sha256_finalize(
		 &context,
		 NULL,
		 0,
		 NULL );

		return( -1 );
	}
	if( libbde_sha256_finalize(
	     &context,
	     hash,
	     hash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#ifdef TODO_FALLBACK
/* FIPS 180-2 compliant SHA-256 functions */

#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define UNPACK32(x, str) \
{ \
    *((str) + 3) = (uint8_t) ((x)      ); \
    *((str) + 2) = (uint8_t) ((x) >>  8); \
    *((str) + 1) = (uint8_t) ((x) >> 16); \
    *((str) + 0) = (uint8_t) ((x) >> 24); \
}

#define PACK32(str, x) \
{ \
    *(x) =   ((uint32_t) *((str) + 3)      ) \
           | ((uint32_t) *((str) + 2) <<  8) \
           | ((uint32_t) *((str) + 1) << 16) \
           | ((uint32_t) *((str) + 0) << 24); \
}

/* Macros used for loops unrolling */

#define SHA256_SCR(i) \
{ \
    w[i] =  SHA256_F4(w[i -  2]) + w[i -  7] \
          + SHA256_F3(w[i - 15]) + w[i - 16]; \
}

#define SHA256_EXP(a, b, c, d, e, f, g, h, j) \
{ \
    t1 = wv[h] + SHA256_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
         + sha256_k[j] + w[j];                              \
    t2 = SHA256_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
    wv[d] += t1;                                            \
    wv[h] = t1 + t2;                                        \
}

void sha256_transf(
      sha256_context_t *context,
      const uint8_t *data,
      unsigned int block_nb )
{
	uint32_t w[64];
	uint32_t wv[8];

	const uint8_t *sub_block = NULL;
	uint32_t t1              = 0;
	uint32_t t2              = 0;
	int i                    = 0;

#if !defined( LIBBDE_SHA_UNROLLED_LOOPS )
	int j                    = 0;
#endif

	for( i = 0;
	     i < (int) block_nb;
	     i++ )
	{
		sub_block = data + (i << 6);

#if !defined( LIBBDE_SHA_UNROLLED_LOOPS )
        for (j = 0; j < 16; j++) {
            PACK32(&sub_block[j << 2], &w[j]);
        }

        for (j = 16; j < 64; j++) {
            SHA256_SCR(j);
        }

        for (j = 0; j < 8; j++) {
            wv[j] = context->hash[ j ];
        }

        for (j = 0; j < 64; j++) {
            t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
                + sha256_k[j] + w[j];
            t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
            wv[7] = wv[6];
            wv[6] = wv[5];
            wv[5] = wv[4];
            wv[4] = wv[3] + t1;
            wv[3] = wv[2];
            wv[2] = wv[1];
            wv[1] = wv[0];
            wv[0] = t1 + t2;
        }

        for (j = 0; j < 8; j++) {
            context->hash[ j ] += wv[j];
        }
#else
        PACK32(&sub_block[ 0], &w[ 0]); PACK32(&sub_block[ 4], &w[ 1]);
        PACK32(&sub_block[ 8], &w[ 2]); PACK32(&sub_block[12], &w[ 3]);
        PACK32(&sub_block[16], &w[ 4]); PACK32(&sub_block[20], &w[ 5]);
        PACK32(&sub_block[24], &w[ 6]); PACK32(&sub_block[28], &w[ 7]);
        PACK32(&sub_block[32], &w[ 8]); PACK32(&sub_block[36], &w[ 9]);
        PACK32(&sub_block[40], &w[10]); PACK32(&sub_block[44], &w[11]);
        PACK32(&sub_block[48], &w[12]); PACK32(&sub_block[52], &w[13]);
        PACK32(&sub_block[56], &w[14]); PACK32(&sub_block[60], &w[15]);

        SHA256_SCR(16); SHA256_SCR(17); SHA256_SCR(18); SHA256_SCR(19);
        SHA256_SCR(20); SHA256_SCR(21); SHA256_SCR(22); SHA256_SCR(23);
        SHA256_SCR(24); SHA256_SCR(25); SHA256_SCR(26); SHA256_SCR(27);
        SHA256_SCR(28); SHA256_SCR(29); SHA256_SCR(30); SHA256_SCR(31);
        SHA256_SCR(32); SHA256_SCR(33); SHA256_SCR(34); SHA256_SCR(35);
        SHA256_SCR(36); SHA256_SCR(37); SHA256_SCR(38); SHA256_SCR(39);
        SHA256_SCR(40); SHA256_SCR(41); SHA256_SCR(42); SHA256_SCR(43);
        SHA256_SCR(44); SHA256_SCR(45); SHA256_SCR(46); SHA256_SCR(47);
        SHA256_SCR(48); SHA256_SCR(49); SHA256_SCR(50); SHA256_SCR(51);
        SHA256_SCR(52); SHA256_SCR(53); SHA256_SCR(54); SHA256_SCR(55);
        SHA256_SCR(56); SHA256_SCR(57); SHA256_SCR(58); SHA256_SCR(59);
        SHA256_SCR(60); SHA256_SCR(61); SHA256_SCR(62); SHA256_SCR(63);

        wv[ 0 ] = context->hash[ 0 ];
	wv[ 1 ] = context->hash[ 1 ];
        wv[ 2 ] = context->hash[ 2 ];
	wv[ 3 ] = context->hash[ 3 ];
        wv[ 4 ] = context->hash[ 4 ];
	wv[ 5 ] = context->hash[ 5 ];
        wv[ 6 ] = context->hash[ 6 ];
	wv[ 7 ] = context->hash[ 7 ];

        SHA256_EXP(0,1,2,3,4,5,6,7, 0); SHA256_EXP(7,0,1,2,3,4,5,6, 1);
        SHA256_EXP(6,7,0,1,2,3,4,5, 2); SHA256_EXP(5,6,7,0,1,2,3,4, 3);
        SHA256_EXP(4,5,6,7,0,1,2,3, 4); SHA256_EXP(3,4,5,6,7,0,1,2, 5);
        SHA256_EXP(2,3,4,5,6,7,0,1, 6); SHA256_EXP(1,2,3,4,5,6,7,0, 7);
        SHA256_EXP(0,1,2,3,4,5,6,7, 8); SHA256_EXP(7,0,1,2,3,4,5,6, 9);
        SHA256_EXP(6,7,0,1,2,3,4,5,10); SHA256_EXP(5,6,7,0,1,2,3,4,11);
        SHA256_EXP(4,5,6,7,0,1,2,3,12); SHA256_EXP(3,4,5,6,7,0,1,2,13);
        SHA256_EXP(2,3,4,5,6,7,0,1,14); SHA256_EXP(1,2,3,4,5,6,7,0,15);
        SHA256_EXP(0,1,2,3,4,5,6,7,16); SHA256_EXP(7,0,1,2,3,4,5,6,17);
        SHA256_EXP(6,7,0,1,2,3,4,5,18); SHA256_EXP(5,6,7,0,1,2,3,4,19);
        SHA256_EXP(4,5,6,7,0,1,2,3,20); SHA256_EXP(3,4,5,6,7,0,1,2,21);
        SHA256_EXP(2,3,4,5,6,7,0,1,22); SHA256_EXP(1,2,3,4,5,6,7,0,23);
        SHA256_EXP(0,1,2,3,4,5,6,7,24); SHA256_EXP(7,0,1,2,3,4,5,6,25);
        SHA256_EXP(6,7,0,1,2,3,4,5,26); SHA256_EXP(5,6,7,0,1,2,3,4,27);
        SHA256_EXP(4,5,6,7,0,1,2,3,28); SHA256_EXP(3,4,5,6,7,0,1,2,29);
        SHA256_EXP(2,3,4,5,6,7,0,1,30); SHA256_EXP(1,2,3,4,5,6,7,0,31);
        SHA256_EXP(0,1,2,3,4,5,6,7,32); SHA256_EXP(7,0,1,2,3,4,5,6,33);
        SHA256_EXP(6,7,0,1,2,3,4,5,34); SHA256_EXP(5,6,7,0,1,2,3,4,35);
        SHA256_EXP(4,5,6,7,0,1,2,3,36); SHA256_EXP(3,4,5,6,7,0,1,2,37);
        SHA256_EXP(2,3,4,5,6,7,0,1,38); SHA256_EXP(1,2,3,4,5,6,7,0,39);
        SHA256_EXP(0,1,2,3,4,5,6,7,40); SHA256_EXP(7,0,1,2,3,4,5,6,41);
        SHA256_EXP(6,7,0,1,2,3,4,5,42); SHA256_EXP(5,6,7,0,1,2,3,4,43);
        SHA256_EXP(4,5,6,7,0,1,2,3,44); SHA256_EXP(3,4,5,6,7,0,1,2,45);
        SHA256_EXP(2,3,4,5,6,7,0,1,46); SHA256_EXP(1,2,3,4,5,6,7,0,47);
        SHA256_EXP(0,1,2,3,4,5,6,7,48); SHA256_EXP(7,0,1,2,3,4,5,6,49);
        SHA256_EXP(6,7,0,1,2,3,4,5,50); SHA256_EXP(5,6,7,0,1,2,3,4,51);
        SHA256_EXP(4,5,6,7,0,1,2,3,52); SHA256_EXP(3,4,5,6,7,0,1,2,53);
        SHA256_EXP(2,3,4,5,6,7,0,1,54); SHA256_EXP(1,2,3,4,5,6,7,0,55);
        SHA256_EXP(0,1,2,3,4,5,6,7,56); SHA256_EXP(7,0,1,2,3,4,5,6,57);
        SHA256_EXP(6,7,0,1,2,3,4,5,58); SHA256_EXP(5,6,7,0,1,2,3,4,59);
        SHA256_EXP(4,5,6,7,0,1,2,3,60); SHA256_EXP(3,4,5,6,7,0,1,2,61);
        SHA256_EXP(2,3,4,5,6,7,0,1,62); SHA256_EXP(1,2,3,4,5,6,7,0,63);

        context->h[0] += wv[0]; context->h[1] += wv[1];
        context->h[2] += wv[2]; context->h[3] += wv[3];
        context->h[4] += wv[4]; context->h[5] += wv[5];
        context->h[6] += wv[6]; context->h[7] += wv[7];
#endif /* !defined( LIBBDE_SHA_UNROLLED_LOOPS ) */
    }
}

#endif /* TODO_FALLBACK */

