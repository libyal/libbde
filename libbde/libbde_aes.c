/*
 * AES encryption functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#include <openssl/sha.h>

#endif

#include "libbde_aes.h"

#if defined( WINAPI )

#if !defined( PROV_RSA_AES )
#define PROV_RSA_AES		24
#endif

#endif /* defined( WINAPI ) */

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) )

/* TODO */

/* FIPS-197 compliant AES encryption functions
 *
 * The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 * http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 * http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#define PADLOCK_ALIGN16(x) \
	(uint32_t *) (16 + ((intptr_t) x & ~15))

#include <string.h>

#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif /* GET_ULONG_LE */

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif /* PUT_ULONG_LE */

#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ FT0[ ( Y0       ) & 0xff ] ^   \
                 FT1[ ( Y1 >>  8 ) & 0xff ] ^   \
                 FT2[ ( Y2 >> 16 ) & 0xff ] ^   \
                 FT3[ ( Y3 >> 24 ) & 0xff ];    \
                                                \
    X1 = *RK++ ^ FT0[ ( Y1       ) & 0xff ] ^   \
                 FT1[ ( Y2 >>  8 ) & 0xff ] ^   \
                 FT2[ ( Y3 >> 16 ) & 0xff ] ^   \
                 FT3[ ( Y0 >> 24 ) & 0xff ];    \
                                                \
    X2 = *RK++ ^ FT0[ ( Y2       ) & 0xff ] ^   \
                 FT1[ ( Y3 >>  8 ) & 0xff ] ^   \
                 FT2[ ( Y0 >> 16 ) & 0xff ] ^   \
                 FT3[ ( Y1 >> 24 ) & 0xff ];    \
                                                \
    X3 = *RK++ ^ FT0[ ( Y3       ) & 0xff ] ^   \
                 FT1[ ( Y0 >>  8 ) & 0xff ] ^   \
                 FT2[ ( Y1 >> 16 ) & 0xff ] ^   \
                 FT3[ ( Y2 >> 24 ) & 0xff ];    \
}

#define AES_RROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ RT0[ ( Y0       ) & 0xff ] ^   \
                 RT1[ ( Y3 >>  8 ) & 0xff ] ^   \
                 RT2[ ( Y2 >> 16 ) & 0xff ] ^   \
                 RT3[ ( Y1 >> 24 ) & 0xff ];    \
                                                \
    X1 = *RK++ ^ RT0[ ( Y1       ) & 0xff ] ^   \
                 RT1[ ( Y0 >>  8 ) & 0xff ] ^   \
                 RT2[ ( Y3 >> 16 ) & 0xff ] ^   \
                 RT3[ ( Y2 >> 24 ) & 0xff ];    \
                                                \
    X2 = *RK++ ^ RT0[ ( Y2       ) & 0xff ] ^   \
                 RT1[ ( Y1 >>  8 ) & 0xff ] ^   \
                 RT2[ ( Y0 >> 16 ) & 0xff ] ^   \
                 RT3[ ( Y3 >> 24 ) & 0xff ];    \
                                                \
    X3 = *RK++ ^ RT0[ ( Y3       ) & 0xff ] ^   \
                 RT1[ ( Y2 >>  8 ) & 0xff ] ^   \
                 RT2[ ( Y1 >> 16 ) & 0xff ] ^   \
                 RT3[ ( Y0 >> 24 ) & 0xff ];    \
}

/*
 * Forward S-box & tables
 */
static unsigned char FSb[256];
static uint32_t FT0[256]; 
static uint32_t FT1[256]; 
static uint32_t FT2[256]; 
static uint32_t FT3[256]; 

/*
 * Reverse S-box & tables
 */
static unsigned char RSb[256];
static uint32_t RT0[256];
static uint32_t RT1[256];
static uint32_t RT2[256];
static uint32_t RT3[256];

/*
 * Round constants
 */
static uint32_t libbde_aes_round_constants[ 10 ];

/*
 * Tables generation code
 */
#define ROTL8(x) ( ( x << 8 ) & 0xffffffff ) | ( x >> 24 )
#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )
#define MUL(x,y) ( ( x && y ) ? pow[(log[x]+log[y]) % 255] : 0 )

static int libbde_aes_tables_initialized = 0;

static void libbde_aes_tables_generate( void )
{
    int i, x, y, z;
    int pow[256];
    int log[256];

    /*
     * compute pow and log tables over GF(2^8)
     */
    for( i = 0, x = 1; i < 256; i++ )
    {
        pow[i] = x;
        log[x] = i;
        x = ( x ^ XTIME( x ) ) & 0xff;
    }

    /*
     * calculate the round constants
     */
    for( i = 0, x = 1; i < 10; i++ )
    {
        libbde_aes_round_constants[i] = (unsigned long) x;
        x = XTIME( x ) & 0xff;
    }

    /*
     * generate the forward and reverse S-boxes
     */
    FSb[0x00] = 0x63;
    RSb[0x63] = 0x00;

    for( i = 1; i < 256; i++ )
    {
        x = pow[255 - log[i]];

        y  = x; y = ( (y << 1) | (y >> 7) ) & 0xff;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xff;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xff;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xff;
        x ^= y ^ 0x63;

        FSb[i] = (unsigned char) x;
        RSb[x] = (unsigned char) i;
    }

    /*
     * generate the forward and reverse tables
     */
    for( i = 0; i < 256; i++ )
    {
        x = FSb[i];
        y = XTIME( x ) & 0xff;
        z =  ( y ^ x ) & 0xff;

        FT0[i] = ( (unsigned long) y       ) ^
                 ( (unsigned long) x <<  8 ) ^
                 ( (unsigned long) x << 16 ) ^
                 ( (unsigned long) z << 24 );

        FT1[i] = ROTL8( FT0[i] );
        FT2[i] = ROTL8( FT1[i] );
        FT3[i] = ROTL8( FT2[i] );

        x = RSb[i];

        RT0[i] = ( (unsigned long) MUL( 0x0E, x )       ) ^
                 ( (unsigned long) MUL( 0x09, x ) <<  8 ) ^
                 ( (unsigned long) MUL( 0x0D, x ) << 16 ) ^
                 ( (unsigned long) MUL( 0x0B, x ) << 24 );

        RT1[i] = ROTL8( RT0[i] );
        RT2[i] = ROTL8( RT1[i] );
        RT3[i] = ROTL8( RT2[i] );
    }
}

#endif /* !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) ) */

/* Initializes the AES context
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_initialize(
     libbde_aes_context_t *context,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_initialize";

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
	/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_CIPHER_CTX_init(
	 &( context->evp_context ) );

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	/* TODO */
#else
	/* TODO */
#endif
	return( 1 );
}

/* Finalizes the AES context
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_finalize(
     libbde_aes_context_t *context,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_finalize";

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
	/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	EVP_CIPHER_CTX_cleanup(
	 &( context->evp_context ) );

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	/* TODO */
#else
	/* TODO */
#endif
	return( 1 );
}

/* Sets the AES decryption key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_set_decyption_key(
     libbde_aes_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error )
{
	static char *function    = "libbde_aes_set_decyption_key";

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) )
	libbde_aes_context encryption_context;

	uint32_t *round_keys     = NULL;
	size_t key_index         = 0;
	int round_constant_index = 0;
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
	if( ( bit_size != 128 )
	 && ( bit_size != 192 )
	 && ( bit_size != 256 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported key bit size.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( memory_copy(
	     context->key,
	     key,
	     bit_size / 8 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key.",
		 function );

		return( -1 );
	}
	context->bit_size = bit_size;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	if( AES_set_decrypt_key(
	     (unsigned char *) key,
	     (int) bit_size,
	     &( context->key ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set decryption key in context.",
		 function );

		return( -1 );
	}
#else
	if( bit_size == 128 )
	{
		context->number_of_round_keys = 10;
	}
	else if( bit_size == 192 )
	{
		context->number_of_round_keys = 12;
	}
	else if( bit_size == 256 )
	{
		context->number_of_round_keys = 14;
	}
	/* Align the buffer to next 16-byte blocks
	 */
	context->round_keys = (uint32_t *) ( 16 + ( (intptr_t) context->round_keys_data & ~15 ) )

	round_keys = context->round_keys;

	if( libbde_aes_set_encryption_key(
	     &encryption_context,
	     key,
	     bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set encryption key in context.",
		 function );

		return( -1 );
	}
/* TODO */
    int i, j;
    uint32_t *SK;

    SK = cty.rk + cty.nr * 4;

    *round_keys++ = *SK++;
    *round_keys++ = *SK++;
    *round_keys++ = *SK++;
    *round_keys++ = *SK++;

    for( i = context->number_of_round_keys, SK -= 8;
         i > 1;
         i--, SK -= 8 )
    {
        for( j = 0; j < 4; j++, SK++ )
        {
            *round_keys++ = RT0[ FSb[ ( *SK       ) & 0xff ] ] ^
                    RT1[ FSb[ ( *SK >>  8 ) & 0xff ] ] ^
                    RT2[ FSb[ ( *SK >> 16 ) & 0xff ] ] ^
                    RT3[ FSb[ ( *SK >> 24 ) & 0xff ] ];
        }
    }

    *round_keys++ = *SK++;
    *round_keys++ = *SK++;
    *round_keys++ = *SK++;
    *round_keys++ = *SK++;

    memset( &cty, 0, sizeof( aes_context ) );
#endif
	return( 1 );
}

/* Sets the AES encryption key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_set_encryption_key(
     libbde_aes_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error )
{
	static char *function    = "libbde_aes_set_encryption_key";

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) )
	uint32_t *round_keys     = NULL;
	size_t key_index         = 0;
	int round_constant_index = 0;
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
	if( ( bit_size != 128 )
	 && ( bit_size != 192 )
	 && ( bit_size != 256 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported key bit size.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( memory_copy(
	     context->key,
	     key,
	     bit_size / 8 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key.",
		 function );

		return( -1 );
	}
	context->bit_size = bit_size;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	if( AES_set_encrypt_key(
	     (unsigned char *) key,
	     (int) bit_size,
	     &( context->key ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set encryption key in context.",
		 function );

		return( -1 );
	}
#else
	if( libbde_aes_tables_initialized == 0 )
	{
/* TODO */
		libbde_aes_tables_generate();
	}
	/* Align the buffer to next 16-byte blocks
	 */
	context->round_keys = (uint32_t *) ( 16 + ( (intptr_t) context->round_keys_data & ~15 ) )

	round_keys = context->round_keys;

	for( key_index = 0;
	     key_index < bit_size;
	     key_index += 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
	         &( key[ key_index ] ),
	         round_keys[ round_constant_index ] );

		round_constant_index++;
	}
	if( bit_size == 128 )
	{
		context->number_of_round_keys = 10;

		for( round_constant_index = 0;
		     round_constant_index < 10;
		     round_constant_index++ )
		{
			round_keys[ 4 ] = libbde_aes_round_constants[ round_constant_index ]
			                ^ round_keys[ 0 ]
			                ^ ( FSb[ ( round_keys[ 3 ] >> 8 ) & 0xff ] )
		        	        ^ ( FSb[ ( round_keys[ 3 ] >> 16 ) & 0xff ] << 8 )
			                ^ ( FSb[ ( round_keys[ 3 ] >> 24 ) & 0xff ] << 16 )
			                ^ ( FSb[ ( round_keys[ 3 ] ) & 0xff ] << 24 );

			round_keys[ 5 ] = round_keys[ 1 ] ^ round_keys[ 4 ];
			round_keys[ 6 ] = round_keys[ 2 ] ^ round_keys[ 5 ];
			round_keys[ 7 ] = round_keys[ 3 ] ^ round_keys[ 6 ];

			round_keys += 4;
		}
	}
	else if( bit_size == 192 )
	{
		context->number_of_round_keys = 12;

		for( round_constant_index = 0;
		     round_constant_index < 8;
		     round_constant_index++ )
		{
			round_keys[ 6 ] = libbde_aes_round_constants[ round_constant_index ]
			                ^ round_keys[ 0 ]
			                ^ ( FSb[ ( round_keys[ 5 ] >> 8 ) & 0xff ] )
			                ^ ( FSb[ ( round_keys[ 5 ] >> 16 ) & 0xff ] << 8 )
			                ^ ( FSb[ ( round_keys[ 5 ] >> 24 ) & 0xff ] << 16 )
			                ^ ( FSb[ ( round_keys[ 5 ] ) & 0xff ] << 24 );

			round_keys[ 7 ]  = round_keys[ 1 ] ^ round_keys[ 6 ];
			round_keys[ 8 ]  = round_keys[ 2 ] ^ round_keys[ 7 ];
			round_keys[ 9 ]  = round_keys[ 3 ] ^ round_keys[ 8 ];
			round_keys[ 10 ] = round_keys[ 4 ] ^ round_keys[ 9 ];
			round_keys[ 11 ] = round_keys[ 5 ] ^ round_keys[ 10 ];

			round_keys += 6;
		}
	}
	else if( bit_size == 256 )
	{
		context->number_of_round_keys = 14;

		for( round_constant_index = 0;
		     round_constant_index < 7;
		     round_constant_index++ )
		{
			round_keys[ 8 ] = libbde_aes_round_constants[ round_constant_index ]
			                ^ round_keys[ 0 ]
			                ^ ( FSb[ ( round_keys[ 7 ] >> 8 ) & 0xff ] )
			                ^ ( FSb[ ( round_keys[ 7 ] >> 16 ) & 0xff ] << 8 )
			                ^ ( FSb[ ( round_keys[ 7 ] >> 24 ) & 0xff ] << 16 )
			                ^ ( FSb[ ( round_keys[ 7 ] ) & 0xff ] << 24 );

			round_keys[ 9 ]  = round_keys[ 1 ] ^ round_keys[ 8 ];
			round_keys[ 10 ] = round_keys[ 2 ] ^ round_keys[ 9 ];
			round_keys[ 11 ] = round_keys[ 3 ] ^ round_keys[ 10 ];

			round_keys[ 12 ] = round_keys[ 4 ]
			                 ^ ( FSb[ ( round_keys[ 11 ] ) & 0xff ] )
			                 ^ ( FSb[ ( round_keys[ 11 ] >> 8 ) & 0xff ] << 8 )
			                 ^ ( FSb[ ( round_keys[ 11 ] >> 16 ) & 0xff ] << 16 )
			                 ^ ( FSb[ ( round_keys[ 11 ] >> 24 ) & 0xff ] << 24 );

			round_keys[ 13 ] = round_keys[ 5 ] ^ round_keys[ 12 ];
			round_keys[ 14 ] = round_keys[ 6 ] ^ round_keys[ 13 ];
			round_keys[ 15 ] = round_keys[ 7 ] ^ round_keys[ 14 ];

			round_keys += 8;
		}
	}
#endif
	return( 1 );
}

/* Decrypts AES encrypted data
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_crypt(
     libbde_aes_context_t *context,
     int mode,
     const uint8_t *initialization_vector,
     size_t initialization_vector_size,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	uint8_t internal_initialization_vector[ 20 ];
	uint8_t block_data[ 16 ];

	static char *function    = "libbde_aes_ccm_crypt";
	size_t data_index        = 0;
	uint8_t block_data_index = 0;

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
	if( mode != LIBBDE_AES_CRYPT_MODE_DECRYPT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( initialization_vector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid initialization vector.",
		 function );

		return( -1 );
	}
/* TODO size check max of 15 bytes */
	if( input_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input data.",
		 function );

		return( -1 );
	}
	if( input_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO size check, multitude of 16 ? */
	if( output_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* The internal IV consists of:
	 * 1 byte size value formatted as: 15 - IV size - 1
	 * a maximum of 14 bytes containing IV bytes
	 * 1 byte counter
	 */
	if( memory_set(
	     internal_initialization_vector,
	     0,
	     20 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal initialization vector.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( internal_initialization_vector[ 1 ] ),
	     initialization_vector,
	     initialization_vector_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy initialization vector.",
		 function );

		return( -1 );
	}
	internal_initialization_vector[ 0 ] = 15 - initialization_vector_size - 1;

	while( data_index < input_data_size )
	{
		if( libbde_aes_ecb_crypt(
		     context,
		     LIBBDE_AES_CRYPT_MODE_ENCRYPT,
		     internal_initialization_vector,
		     20,
		     block_data,
		     16,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
			 "%s: unable to encrypt initialization vector.",
			 function );

			return( -1 );
		}
		for( block_data_index = 0;
		     block_data_index < 16;
		     block_data_index++ )
		{
			output_data[ data_index ] = input_data[ data_index ]
			                          ^ block_data[ block_data_index ];

			data_index++;

			if( data_index >= input_data_size )
			{
				break;
			}
		}
		internal_initialization_vector[ 15 ] += 1;
	}
	return( 1 );
}

/* De- or encrypts a 16-byte block using AES-ECB
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ecb_crypt(
     libbde_aes_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	static char *function     = "libbde_aes_ecb_crypt";

#if defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	const EVP_CIPHER *cipher  = NULL;
	int safe_output_data_size = 0;
#endif

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) )
/* TODO refactor */
	uint32_t *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3;
	int i;
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
	if( ( mode != LIBBDE_AES_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBBDE_AES_CRYPT_MODE_ENCRYPT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( input_data_size > (size_t) INT_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) INT_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_output_data_size = (int) output_data_size;

	if( context->bit_size == 128 )
	{
		cipher = EVP_aes_128_ecb();
	}
	else if( context->bit_size == 192 )
	{
		cipher = EVP_aes_192_ecb();
	}
	else if( context->bit_size == 256 )
	{
		cipher = EVP_aes_256_ecb();
	}
	if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
	{
/* TODO test */
		if( EVP_EncryptInit(
		     &( context->evp_context ),
		     cipher,
		     (unsigned char *) context->key,
		     NULL ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to initialize encryption.",
			 function );

			return( -1 );
		}
		if( EVP_EncryptUpdate(
		     &( context->evp_context ),
		     (unsigned char *) output_data,
		     &safe_output_data_size,
		     (unsigned char *) input_data,
		     (int) input_data_size ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update encryption.",
			 function );

			return( -1 );
		}
/* TODO what about output data size ? */
		if( EVP_EncryptFinal(
		     &( context->evp_context ),
		     (unsigned char *) output_data,
		     &safe_output_data_size ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to finalize encryption.",
			 function );

			return( -1 );
		}
	}
	else
	{
/* TODO test */
		if( EVP_DecryptInit(
		     &( context->evp_context ),
		     cipher,
		     (unsigned char *) context->key,
		     NULL ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to initialize decryption.",
			 function );

			return( -1 );
		}
		if( EVP_DecryptUpdate(
		     &( context->evp_context ),
		     (unsigned char *) output_data,
		     &safe_output_data_size,
		     (unsigned char *) input_data,
		     (int) input_data_size ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to update decryption.",
			 function );

			return( -1 );
		}
/* TODO what about output data size ? */
		if( EVP_DecryptFinal(
		     &( context->evp_context ),
		     (unsigned char *) output_data,
		     &safe_output_data_size ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to finalize decryption.",
			 function );

			return( -1 );
		}
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	AES_ecb_encrypt(
	 input,
	 output,
	 context->key,
	 mode );
#else
    RK = context->rk;

    GET_ULONG_LE( X0, input,  0 ); X0 ^= *RK++;
    GET_ULONG_LE( X1, input,  4 ); X1 ^= *RK++;
    GET_ULONG_LE( X2, input,  8 ); X2 ^= *RK++;
    GET_ULONG_LE( X3, input, 12 ); X3 ^= *RK++;

    if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
    {
        for( i = (context->number_of_round_keys >> 1); i > 1; i-- )
        {
            AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_FROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( FSb[ ( Y0       ) & 0xff ]       ) ^
                     ( FSb[ ( Y1 >>  8 ) & 0xff ] <<  8 ) ^
                     ( FSb[ ( Y2 >> 16 ) & 0xff ] << 16 ) ^
                     ( FSb[ ( Y3 >> 24 ) & 0xff ] << 24 );

        X1 = *RK++ ^ ( FSb[ ( Y1       ) & 0xff ]       ) ^
                     ( FSb[ ( Y2 >>  8 ) & 0xff ] <<  8 ) ^
                     ( FSb[ ( Y3 >> 16 ) & 0xff ] << 16 ) ^
                     ( FSb[ ( Y0 >> 24 ) & 0xff ] << 24 );

        X2 = *RK++ ^ ( FSb[ ( Y2       ) & 0xff ]       ) ^
                     ( FSb[ ( Y3 >>  8 ) & 0xff ] <<  8 ) ^
                     ( FSb[ ( Y0 >> 16 ) & 0xff ] << 16 ) ^
                     ( FSb[ ( Y1 >> 24 ) & 0xff ] << 24 );

        X3 = *RK++ ^ ( FSb[ ( Y3       ) & 0xff ]       ) ^
                     ( FSb[ ( Y0 >>  8 ) & 0xff ] <<  8 ) ^
                     ( FSb[ ( Y1 >> 16 ) & 0xff ] << 16 ) ^
                     ( FSb[ ( Y2 >> 24 ) & 0xff ] << 24 );
    }
    else
    {
        for( i = (context->number_of_round_keys >> 1); i > 1; i-- )
        {
            AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_RROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( RSb[ ( Y0       ) & 0xff ]       ) ^
                     ( RSb[ ( Y3 >>  8 ) & 0xff ] <<  8 ) ^
                     ( RSb[ ( Y2 >> 16 ) & 0xff ] << 16 ) ^
                     ( RSb[ ( Y1 >> 24 ) & 0xff ] << 24 );

        X1 = *RK++ ^ ( RSb[ ( Y1       ) & 0xff ]       ) ^
                     ( RSb[ ( Y0 >>  8 ) & 0xff ] <<  8 ) ^
                     ( RSb[ ( Y3 >> 16 ) & 0xff ] << 16 ) ^
                     ( RSb[ ( Y2 >> 24 ) & 0xff ] << 24 );

        X2 = *RK++ ^ ( RSb[ ( Y2       ) & 0xff ]       ) ^
                     ( RSb[ ( Y1 >>  8 ) & 0xff ] <<  8 ) ^
                     ( RSb[ ( Y0 >> 16 ) & 0xff ] << 16 ) ^
                     ( RSb[ ( Y3 >> 24 ) & 0xff ] << 24 );

        X3 = *RK++ ^ ( RSb[ ( Y3       ) & 0xff ]       ) ^
                     ( RSb[ ( Y2 >>  8 ) & 0xff ] <<  8 ) ^
                     ( RSb[ ( Y1 >> 16 ) & 0xff ] << 16 ) ^
                     ( RSb[ ( Y0 >> 24 ) & 0xff ] << 24 );
    }

    PUT_ULONG_LE( X0, output,  0 );
    PUT_ULONG_LE( X1, output,  4 );
    PUT_ULONG_LE( X2, output,  8 );
    PUT_ULONG_LE( X3, output, 12 );
#endif
	return( 1 );
}

/* De- or encrypts a block of data using AES-CBC
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_cbc_crypt(
     libbde_aes_context_t *context,
     int mode,
     int length,
     unsigned char iv[16],
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_cbc_crypt";

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_AES_H ) )
    int i;
    unsigned char temp[16];
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
/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
/* TODO */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
/* TODO */

#else
	if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
	{
		while( length > 0 )
		{
			for( i = 0; i < 16; i++ )
			{
				output[i] = (unsigned char)( input[i] ^ iv[i] );
			}
			aes_ecb_crypt( context, mode, output, output );
			memcpy( iv, output, 16 );

			input  += 16;
			output += 16;
			length -= 16;
		}
	}
	else
	{
		while( length > 0 )
		{
			memcpy( temp, input, 16 );
			aes_ecb_crypt( context, mode, input, output );

			for( i = 0; i < 16; i++ )
			{
				output[i] = (unsigned char)( output[i] ^ iv[i] );
			}
			memcpy( iv, temp, 16 );

			input  += 16;
			output += 16;
			length -= 16;
		}
  	}
#endif
	return( 1 );
}

/* De- or encrypts a block of data using AES-CBF
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_cfb_crypt(
     libbde_aes_context_t *context,
     int mode,
     uint8_t initialization_vector[ 16 ],
     size_t *initialization_vector_index,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_cfb_crypt";
	size_t data_index     = 0;

	if( ( mode != LIBBDE_AES_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBBDE_AES_CRYPT_MODE_ENCRYPT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( initialization_vector_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid initialization vector index.",
		 function );

		return( -1 );
	}
	if( input_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid input data.",
		 function );

		return( -1 );
	}
	if( input_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( output_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid output data.",
		 function );

		return( -1 );
	}
	if( output_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid output data size value exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO check if output size > input size */
/* TODO check if iv index >= 16 */

	for( data_index = 0;
	     data_index < input_data_size;
	     data_index++ )
	{
		if( *initialization_vector_index == 0 )
		{
			if( libbde_aes_ecb_crypt(
			     context,
			     mode,
			     initialization_vector,
			     16,
			     initialization_vector,
			     16,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to de/encrypt initialization vector.",
				 function );

				return( -1 );
			}
		}
		output_data[ data_index ] = input_data[ data_index ]
		                          ^ initialization_vector[ *initialization_vector_index ];

		if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
		{
			initialization_vector[ *initialization_vector_index ] = output_data[ data_index ];
		}
		else
		{
			initialization_vector[ *initialization_vector_index ] = input_data[ data_index ];
		}
		*initialization_vector_index = ( *initialization_vector_index + 1 ) & 0x0f;
	}
	return( 1 );
}

#ifdef TODO_REMOVE
void aes_ccm_decrypt(
     libbde_aes_context_t *context,
      int mode,
      unsigned char *iv,
      uint32_t iv_length,
      unsigned char *input,
      uint32_t input_length,
      unsigned char *output);

char datum_data[] = {
/*0x5C,0x00,0x00,0x00,0x03,0x00,0x01,0x00,0x00,0x10,0x00,0x00,0x94,0xE9,0xA7,0x51
,0xC0,0xA8,0xBF,0xFD,0x7E,0xB4,0xEC,0xF3,0xB9,0x70,0x4C,0xA9, */
0x38,0x00,0x00,0x00
,0x05,0x00,0x01,0x00,0x40,0x78,0x88,0x8D,0xD0,0x23,0xC8,0x01,0x09,0x00,0x00,0x00
,0x07,0x41,0xF6,0xC3,0x2C,0xB0,0x65,0x6E,0x18,0xAA,0x33,0xD1,0x80,0xA2,0x82,0xAE
,0x98,0x94,0x2E,0x0A,0xF3,0xE8,0x46,0xDD,0x3D,0x26,0x26,0xC0,0xCE,0x19,0x7B,0x8D
,0xF3,0xFA,0x18,0xF3//,0x4C,0x09,0xDF,0x74,0xF7,0x69,0xDE,0x1D 
};

uint8_t datum_encrypted[] = {
0x5c,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
0x40,0x78,0x88,0x8d,0xd0,0x23,0xc8,0x01,
0x09,0x00,0x00,0x00,0x85,0x68,0x02,0x2c,0xc2,0x47,0x1b,0x9e,0xdf,0x5e,0x4b,0x1d,
0xca,0x8e,0x2a,0xce,0x14,0x58,0x39,0xd8,0x38,0xbc,0xdd,0xe5,0x9f,0x0e,0x82,0x89,
0x01,0xbf,0xac,0x5d,0x61,0xfa,0xd0,0x4b,0x2c,0x1e,0x46,0xb9,0xa0,0x35,0x5e,0x6f,
0x32,0xf3,0x3a,0xa3,0xb7,0xa5,0x48,0x51,0xab,0x22,0xc3,0xc8,0x66,0x4f,0x14,0x6b,
0x88,0x7d,0x17,0x3d,0xe1,0x5f,0xc5,0x40,0xec,0xfc,0xd3,0x6d 
,0,0,0,0,0,0,0,0
// alignment
};

uint8_t datum_password[] = {
	0x2C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0x06, 0x00, 0x4d, 0xf7,
	0xa8, 0x28, 0xfa, 0xcc, 0xcb, 0xb6, 0xc5, 0x0f,
	0xf2, 0x52, 0xe1, 0x9a, 0x32, 0x39, 0xce, 0x6a,
	0x3c, 0xf9, 0xfb, 0xb9, 0x2d, 0x7f, 0x8e, 0xd9,
	0x54, 0x5d, 0x8a, 0xc0
};

uint8_t datum_password_testing[] = {
	0x2C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0xA8, 0x18, 0x56, 0x08,
	0x9C, 0x83, 0x82, 0x35, 0x15, 0x75, 0x62, 0x87,
	0xD5, 0xF7, 0xD3, 0xCC, 0x6C, 0xC9, 0x90, 0x83,
	0x1B, 0xB5, 0x7E, 0x81, 0x64, 0xF7, 0x89, 0xFC,
	0xA3, 0x24, 0x07, 0xFB
};

uint8_t datum_encrypted_testing[] = {
	0x5C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x70, 0x5D, 0x8B, 0x29,
	0xDA, 0x1F, 0xC8, 0x01, 0x03, 0x00, 0x00, 0x00,
	0x1F, 0x63, 0x25, 0x85, 0xC5, 0xDF, 0xCE, 0x83,
	0x28, 0x42, 0x7F, 0x21, 0x3C, 0x8B, 0x7C, 0x4A,
	0xCA, 0x65, 0xA3, 0xFD, 0xA9, 0x16, 0xB6, 0x73,
	0x8B, 0x6A, 0x8B, 0x10, 0xE2, 0xA3, 0xBC, 0x9C,
	0x77, 0x02, 0xA8, 0x08, 0x55, 0xCC, 0xF3, 0xB7,
	0x91, 0x2D, 0xC6, 0x35, 0xA0, 0x90, 0x23, 0xFA,
	0x67, 0xE8, 0xD4, 0x9A, 0x19, 0xF4, 0x11, 0xC6,
	0x00, 0x18, 0xA8, 0xBB
};

uint8_t datum_encrypted_testing2[] = {
	0x50, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01, 0x00,
	0x70, 0x5D, 0x8B, 0x29, 0xDA, 0x1F, 0xC8, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x9F, 0x79, 0x5A, 0xB8,
	0x98, 0x38, 0x4C, 0x71, 0xA7, 0xCA, 0x62, 0x3A,
	0xDA, 0x49, 0x31, 0x42, 0x5D, 0x6C, 0xB0, 0xC8,
	0xE5, 0x56, 0x53, 0xC4, 0x6B, 0x7A, 0xBD, 0xED,
	0xC9, 0xBF, 0xD4, 0x5D, 0xFE, 0xD9, 0x01, 0xE4,
	0x1C, 0x36, 0x31, 0xE3, 0x42, 0xAA, 0xBD, 0xDA,
	0x7D, 0x10, 0xD7, 0x9F, 0x5F, 0x52, 0xA1, 0xB3,
	0x20, 0xFE, 0xBE, 0x2C, 0xC8, 0x41, 0xE5, 0x00 
};

#endif /* TODO_REMOVE */

