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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
#include <openssl/sha.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#endif

#include "libbde_aes.h"
#include "libbde_error_string.h"

#if defined( WINAPI )

#if !defined( PROV_RSA_AES )
#define PROV_RSA_AES		24
#endif

#endif /* defined( WINAPI ) */

#if !defined( LIBBDE_HAVE_AES_SUPPORT )

/* FIPS-197 compliant AES encryption functions
 *
 * The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 * http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 * http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#define libbde_aes_calculate_forward_substitution_value( value, index0, index1, index2, index3 ) \
	value   = libbde_aes_forward_substitution_box[ index3 ]; \
	value <<= 8; \
	value  |= libbde_aes_forward_substitution_box[ index2 ]; \
	value <<= 8; \
	value  |= libbde_aes_forward_substitution_box[ index1 ]; \
	value <<= 8; \
	value  |= libbde_aes_forward_substitution_box[ index0 ];

#define libbde_aes_calculate_forward_table_value( value, index0, index1, index2, index3 ) \
	value  = libbde_aes_forward_table0[ index0 ]; \
	value ^= libbde_aes_forward_table1[ index1 ]; \
	value ^= libbde_aes_forward_table2[ index2 ]; \
	value ^= libbde_aes_forward_table3[ index3 ];

#define libbde_aes_calculate_reverse_substitution_value( value, index0, index1, index2, index3 ) \
	value   = libbde_aes_reverse_substitution_box[ index3 ]; \
	value <<= 8; \
	value  |= libbde_aes_reverse_substitution_box[ index2 ]; \
	value <<= 8; \
	value  |= libbde_aes_reverse_substitution_box[ index1 ]; \
	value <<= 8; \
	value  |= libbde_aes_reverse_substitution_box[ index0 ];

#define libbde_aes_calculate_reverse_table_value( value, index0, index1, index2, index3 ) \
	value  = libbde_aes_reverse_table0[ index0 ]; \
	value ^= libbde_aes_reverse_table1[ index1 ]; \
	value ^= libbde_aes_reverse_table2[ index2 ]; \
	value ^= libbde_aes_reverse_table3[ index3 ];

#define libbde_aes_calculate_forward_substitution_round( round_keys, values_32bit, cipher_values_32bit, substitution_value ) \
	libbde_aes_calculate_forward_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 0 ], \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 24 ) ); \
\
	values_32bit[ 0 ] = round_keys[ 0 ] ^ substitution_value; \
\
	libbde_aes_calculate_forward_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 1 ], \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 24 ) ); \
\
	values_32bit[ 1 ] = round_keys[ 1 ] ^ substitution_value; \
\
	libbde_aes_calculate_forward_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 2 ], \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 24 ) ); \
\
	values_32bit[ 2 ] = round_keys[ 2 ] ^ substitution_value; \
\
	libbde_aes_calculate_forward_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 3 ], \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 24 ) ); \
\
	values_32bit[ 3 ] = round_keys[ 3 ] ^ substitution_value; 

#define libbde_aes_calculate_forward_table_round( round_keys, values_32bit, cipher_values_32bit, table_value ) \
	libbde_aes_calculate_forward_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 0 ], \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 24 ) ); \
\
	values_32bit[ 0 ] = round_keys[ 0 ] ^ table_value; \
\
	libbde_aes_calculate_forward_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 1 ], \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 24 ) ); \
\
	values_32bit[ 1 ] = round_keys[ 1 ] ^ table_value; \
\
	libbde_aes_calculate_forward_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 2 ], \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 24 ) ); \
\
	values_32bit[ 2 ] = round_keys[ 2 ] ^ table_value; \
\
	libbde_aes_calculate_forward_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 3 ], \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 24 ) ); \
\
	values_32bit[ 3 ] = round_keys[ 3 ] ^ table_value; 

#define libbde_aes_calculate_reverse_substitution_round( round_keys, values_32bit, cipher_values_32bit, substitution_value ) \
	libbde_aes_calculate_reverse_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 0 ], \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 24 ) ); \
\
	values_32bit[ 0 ] = round_keys[ 0 ] ^ substitution_value; \
\
	libbde_aes_calculate_reverse_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 1 ], \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 24 ) ); \
\
	values_32bit[ 1 ] = round_keys[ 1 ] ^ substitution_value; \
\
	libbde_aes_calculate_reverse_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 2 ], \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 24 ) ); \
\
	values_32bit[ 2 ] = round_keys[ 2 ] ^ substitution_value; \
\
	libbde_aes_calculate_reverse_substitution_value( \
	 substitution_value, \
	 (uint8_t) cipher_values_32bit[ 3 ], \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 24 ) ); \
\
	values_32bit[ 3 ] = round_keys[ 3 ] ^ substitution_value;

#define libbde_aes_calculate_reverse_table_round( round_keys, values_32bit, cipher_values_32bit, table_value ) \
	libbde_aes_calculate_reverse_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 0 ], \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 24 ) ); \
\
	values_32bit[ 0 ] = round_keys[ 0 ] ^ table_value; \
\
	libbde_aes_calculate_reverse_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 1 ], \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 24 ) ); \
\
	values_32bit[ 1 ] = round_keys[ 1 ] ^ table_value; \
\
	libbde_aes_calculate_reverse_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 2 ], \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 3 ] >> 24 ) ); \
\
	values_32bit[ 2 ] = round_keys[ 2 ] ^ table_value; \
\
	libbde_aes_calculate_reverse_table_value( \
	 table_value, \
	 (uint8_t) cipher_values_32bit[ 3 ], \
	 (uint8_t) ( cipher_values_32bit[ 2 ] >> 8 ), \
	 (uint8_t) ( cipher_values_32bit[ 1 ] >> 16 ), \
	 (uint8_t) ( cipher_values_32bit[ 0 ] >> 24 ) ); \
\
	values_32bit[ 3 ] = round_keys[ 3 ] ^ table_value;

/* Forward S-box & tables
 */
static uint8_t libbde_aes_forward_substitution_box[ 256 ];

static uint32_t libbde_aes_forward_table0[ 256 ]; 
static uint32_t libbde_aes_forward_table1[ 256 ]; 
static uint32_t libbde_aes_forward_table2[ 256 ]; 
static uint32_t libbde_aes_forward_table3[ 256 ]; 

/* Reverse S-box & tables
 */
static uint8_t libbde_aes_reverse_substitution_box[ 256 ];

static uint32_t libbde_aes_reverse_table0[ 256 ];
static uint32_t libbde_aes_reverse_table1[ 256 ];
static uint32_t libbde_aes_reverse_table2[ 256 ];
static uint32_t libbde_aes_reverse_table3[ 256 ];

/* Round constants
 */
static uint32_t libbde_aes_round_constants[ 10 ];

static int libbde_aes_tables_initialized = 0;

int libbde_aes_initialize_tables(
     liberror_error_t **error )
{
	uint8_t logs_table[ 256 ];
	uint8_t powers_table[ 256 ];

	static char *function        = "libbde_aes_initialize_tables";
	uint16_t byte_index          = 0;
	uint16_t table_index         = 0;
	uint8_t byte_value           = 0;
	uint8_t round_constant_index = 0;
	uint8_t substitution_value   = 0;

	if( memory_set(
	     logs_table,
	     0,
	     sizeof( uint8_t ) * 256 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear logs table.",
		 function );

		return( -1 );
	}
	/* Fill the powers and logs tables over GF( 2^8 )
	 */
	byte_value = 1;

	for( byte_index = 0;
	     byte_index < 256;
	     byte_index++ )
	{
		powers_table[ byte_index ] = byte_value;
		logs_table[ byte_value ]   = byte_index;

		if( ( byte_value & 0x80 ) == 0 )
		{
			byte_value ^= byte_value << 1;
		}
		else
		{
			byte_value ^= ( byte_value << 1 ) ^ 0x1b;
		}
	}
	/* Fill the round constants
	 */
	byte_value = 1;

	for( round_constant_index = 0;
	     round_constant_index < 10;
	     round_constant_index++ )
	{
		libbde_aes_round_constants[ round_constant_index ] = (uint32_t) byte_value;

		if( ( byte_value & 0x80 ) == 0 )
		{
			byte_value = byte_value << 1;
		}
		else
		{
			byte_value = ( byte_value << 1 ) ^ 0x1b;
		}
	}
	/* Fill the forward and reverse S-boxes
	 */
	libbde_aes_forward_substitution_box[ 0x00 ] = 0x63;
	libbde_aes_reverse_substitution_box[ 0x63 ] = 0x00;

	for( byte_index = 1;
	     byte_index < 256;
	     byte_index++ )
	{
		table_index = 255 - logs_table[ byte_index ];
		byte_value  = powers_table[ table_index ];

		substitution_value = ( byte_value << 1 )
		                   | ( byte_value >> 7 );

		byte_value ^= substitution_value;

		substitution_value = ( substitution_value << 1 )
		                   | ( substitution_value >> 7 );

		byte_value ^= substitution_value;

		substitution_value = ( substitution_value << 1 )
		                   | ( substitution_value >> 7 );

		byte_value ^= substitution_value;

		substitution_value = ( substitution_value << 1 )
		                   | ( substitution_value >> 7 );

		substitution_value ^= byte_value ^ 0x63;

		libbde_aes_forward_substitution_box[ byte_index ]         = substitution_value;
		libbde_aes_reverse_substitution_box[ substitution_value ] = byte_index;
	}
	/* Fill the forward and reverse tables
	 */
	for( byte_index = 0;
	     byte_index < 256;
	     byte_index++ )
	{
		byte_value = libbde_aes_forward_substitution_box[ byte_index ];

		if( ( byte_value & 0x80 ) == 0 )
		{
			substitution_value = byte_value << 1;
		}
		else
		{
			substitution_value = ( byte_value << 1 ) ^ 0x1b;
		}
		libbde_aes_forward_table0[ byte_index ]   = byte_value ^ substitution_value;
		libbde_aes_forward_table0[ byte_index ] <<= 8;
		libbde_aes_forward_table0[ byte_index ]  |= byte_value;
		libbde_aes_forward_table0[ byte_index ] <<= 8;
		libbde_aes_forward_table0[ byte_index ]  |= byte_value;
		libbde_aes_forward_table0[ byte_index ] <<= 8;
		libbde_aes_forward_table0[ byte_index ]  |= substitution_value;

		libbde_aes_forward_table1[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_forward_table0[ byte_index ],
		                                           8 );

		libbde_aes_forward_table2[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_forward_table1[ byte_index ],
		                                           8 );

		libbde_aes_forward_table3[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_forward_table2[ byte_index ],
		                                           8 );

		substitution_value = libbde_aes_reverse_substitution_box[ byte_index ];

		libbde_aes_reverse_table0[ byte_index ] = 0;

		if( substitution_value != 0 )
		{
			table_index  = logs_table[ 0x0b ];
			table_index += logs_table[ substitution_value ];
			table_index %= 255;

			libbde_aes_reverse_table0[ byte_index ] ^= powers_table[ table_index ];
			libbde_aes_reverse_table0[ byte_index ] <<= 8;

			table_index  = logs_table[ 0x0d ];
			table_index += logs_table[ substitution_value ];
			table_index %= 255;

			libbde_aes_reverse_table0[ byte_index ] ^= powers_table[ table_index ];
			libbde_aes_reverse_table0[ byte_index ] <<= 8;

			table_index  = logs_table[ 0x09 ];
			table_index += logs_table[ substitution_value ];
			table_index %= 255;

			libbde_aes_reverse_table0[ byte_index ] ^= powers_table[ table_index ];
			libbde_aes_reverse_table0[ byte_index ] <<= 8;

			table_index  = logs_table[ 0x0e ];
			table_index += logs_table[ substitution_value ];
			table_index %= 255;

			libbde_aes_reverse_table0[ byte_index ] ^= powers_table[ table_index ];
		}
		libbde_aes_reverse_table1[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_reverse_table0[ byte_index ],
		                                           8 );

		libbde_aes_reverse_table2[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_reverse_table1[ byte_index ],
		                                           8 );

		libbde_aes_reverse_table3[ byte_index ] = byte_stream_bit_rotate_left(
		                                           libbde_aes_reverse_table2[ byte_index ],
		                                           8 );
	}
	return( 1 );
}

#endif /* !defined( LIBBDE_HAVE_AES_SUPPORT ) */

#if defined( WINAPI )

/* Initializes the AES key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_key_initialize(
     libbde_aes_key_t **key,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_key_initialize";

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key == NULL )
	{
		*key = memory_allocate_structure(
		        libbde_aes_key_t );

		if( *key == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create key.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *key,
		     0,
		     sizeof( libbde_aes_key_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key.",
			 function );

			goto on_error;
		}
		( *key )->header.bType    = PLAINTEXTKEYBLOB;
		( *key )->header.bVersion = CUR_BLOB_VERSION;
	}
	return( 1 );

on_error:
	if( *key != NULL )
	{
		memory_free(
		 *key );

		*key = NULL;
	}
	return( -1 );
}

/* Sets the AES key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_key_set(
     libbde_aes_key_t *key,
     const uint8_t *key_data,
     size_t bit_size,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_key_set";

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key data.",
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
	if( bit_size == 128 )
	{
		key->header.aiKeyAlg = CALG_AES_128;
	}
	else if( bit_size == 192 )
	{
		key->header.aiKeyAlg = CALG_AES_192;
	}
	else if( bit_size == 256 )
	{
		key->header.aiKeyAlg = CALG_AES_256;
	}
	key->data_size = bit_size / 8;

	if( memory_copy(
	     key->data,
	     key_data,
	     key->data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Frees an AES key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_key_free(
     libbde_aes_key_t **key,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_key_free";
	int result            = 1;

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		if( memory_set(
		     *key,
		     0,
		     sizeof( libbde_aes_key_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key.",
			 function );

			result = -1;
		}
		memory_free(
		 *key );

		*key = NULL;
	}
	return( result );
}

#endif /* defined( WINAPI ) */

/* Initializes the AES context
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_initialize(
     libbde_aes_context_t **context,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_context_initialize";

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
	if( *context == NULL )
	{
		*context = memory_allocate_structure(
		            libbde_aes_context_t );

		if( *context == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *context,
		     0,
		     sizeof( libbde_aes_context_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear context.",
			 function );

			goto on_error;
		}
#if defined( WINAPI )
		/* Request the AES crypt provider, fail back to the RSA crypt provider
		*/
		if( CryptAcquireContext(
		     &( ( *context )->crypt_provider ),
		     NULL,
		     NULL,
		     PROV_RSA_AES,
		     CRYPT_VERIFYCONTEXT ) == 0 )
		{
			if( CryptAcquireContext(
			     &( ( *context )->crypt_provider ),
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

				goto on_error;
			}
		}
		if( ( *context )->crypt_provider == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: unable to create crypt provider.",
			 function );

			goto on_error;
		}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
		/* No additional initialization necessary */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
		EVP_CIPHER_CTX_init(
		 &( ( *context )->evp_context ) );

		if( EVP_CIPHER_CTX_set_padding(
		     &( ( *context )->evp_context ),
		     1 ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set padding in context.",
			 function );

			goto on_error;
		}

#else
		if( libbde_aes_tables_initialized == 0 )
		{
			if( libbde_aes_initialize_tables(
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize tables.",
				 function );

				goto on_error;
			}
			libbde_aes_tables_initialized = 1;
		}
#endif
	}
	return( 1 );

on_error:
	if( *context != NULL )
	{
		memory_free(
		 *context );

		*context = NULL;
	}
	return( -1 );
}

/* Frees an AES context
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_free(
     libbde_aes_context_t **context,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_free";

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
	if( *context != NULL )
	{
#if defined( WINAPI )
		if( ( *context )->crypt_provider != 0 )
		{
			CryptReleaseContext(
			 ( *context )->crypt_provider,
			 0 );
		}
		if( ( *context )->key != 0 )
		{
			CryptDestroyKey(
			 ( *context )->key );
		}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
		/* No additional clean up necessary */

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
		EVP_CIPHER_CTX_cleanup(
		 &( ( *context )->evp_context ) );

#else
		/* No additional clean up necessary */
#endif
		memory_free(
		 *context );

		*context = NULL;
	}
	return( 1 );
}

/* Sets the AES decryption key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_set_decryption_key(
     libbde_aes_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error )
{
	static char *function           = "libbde_aes_set_decryption_key";

#if defined( WINAPI )
	libcstring_system_character_t error_string[ LIBBDE_ERROR_STRING_SIZE ];

	libbde_aes_key_t *wincrypt_key  = NULL;
	DWORD error_code                = 0;
	DWORD wincrypt_key_size         = 0;

#elif !defined( LIBBDE_HAVE_AES_SUPPORT )
	libbde_aes_context_t encryption_context;

	uint32_t *encryption_round_keys = NULL;
	uint32_t *round_keys            = NULL;
	uint8_t byte_value0             = 0;
	uint8_t byte_value1             = 0;
	uint8_t byte_value2             = 0;
	uint8_t byte_value3             = 0;
	int round_key_iterator          = 0;
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
	if( libbde_aes_key_initialize(
	     &wincrypt_key,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create wincrypt key.",
		 function );

		return( -1 );
	}
	if( libbde_aes_key_set(
	     wincrypt_key,
	     key,
	     bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set wincrypt key.",
		 function );

		libbde_aes_key_free(
		 &wincrypt_key,
		 NULL );

		return( -1 );
	}
	wincrypt_key_size = sizeof( libbde_aes_key_t ) - ( ( 256 - bit_size ) / 8 );

	if( CryptImportKey(
	     context->crypt_provider,
	     (CONST BYTE *) &wincrypt_key,
	     wincrypt_key_size,
	     (HCRYPTKEY) NULL,
	     0,
	     &( context->key ) ) == 0 )
	{
		error_code = GetLastError();

		if( libbde_error_string_copy_from_error_number(
		     error_string,
		     LIBBDE_ERROR_STRING_SIZE,
		     error_code,
		     error ) == 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key object with error: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key object.",
			 function );
		}
		libbde_aes_key_free(
		 &wincrypt_key,
		 NULL );

		return( -1 );
	}
	if( libbde_aes_key_free(
	     &wincrypt_key,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free wincrypt key.",
		 function );

		return( -1 );
	}

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
	context->round_keys = (uint32_t *) ( 16 + ( (intptr_t) context->round_keys_data & ~( 15 ) ) );

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
	/* Point to the end of the round keys
	 */
	encryption_round_keys = encryption_context.round_keys
	                      + ( encryption_context.number_of_round_keys * sizeof( uint32_t ) );

/* TODO why doesn't this work
	encryption_round_keys = &( encryption_context.round_keys[ encryption_context.number_of_round_keys ] );
*/

	round_keys[ 0 ] = encryption_round_keys[ 0 ];
	round_keys[ 1 ] = encryption_round_keys[ 1 ];
	round_keys[ 2 ] = encryption_round_keys[ 2 ];
	round_keys[ 3 ] = encryption_round_keys[ 3 ];

	round_keys            += 4;
	encryption_round_keys -= 4;

	for( round_key_iterator = context->number_of_round_keys;
	     round_key_iterator > 1;
	     round_key_iterator-- )
	{
		byte_value0 = (uint8_t) encryption_round_keys[ 0 ];
		byte_value1 = (uint8_t) ( encryption_round_keys[ 0 ] >> 8 );
		byte_value2 = (uint8_t) ( encryption_round_keys[ 0 ] >> 16 );
		byte_value3 = (uint8_t) ( encryption_round_keys[ 0 ] >> 24 );

		byte_value0 = libbde_aes_forward_substitution_box[ byte_value0 ];
		byte_value1 = libbde_aes_forward_substitution_box[ byte_value1 ];
		byte_value2 = libbde_aes_forward_substitution_box[ byte_value2 ];
		byte_value3 = libbde_aes_forward_substitution_box[ byte_value3 ];

		round_keys[ 0 ] = libbde_aes_reverse_table0[ byte_value0 ]
			        ^ libbde_aes_reverse_table1[ byte_value1 ]
			        ^ libbde_aes_reverse_table2[ byte_value2 ]
			        ^ libbde_aes_reverse_table3[ byte_value3 ];

		byte_value0 = (uint8_t) encryption_round_keys[ 1 ];
		byte_value1 = (uint8_t) ( encryption_round_keys[ 1 ] >> 8 );
		byte_value2 = (uint8_t) ( encryption_round_keys[ 1 ] >> 16 );
		byte_value3 = (uint8_t) ( encryption_round_keys[ 1 ] >> 24 );

		byte_value0 = libbde_aes_forward_substitution_box[ byte_value0 ];
		byte_value1 = libbde_aes_forward_substitution_box[ byte_value1 ];
		byte_value2 = libbde_aes_forward_substitution_box[ byte_value2 ];
		byte_value3 = libbde_aes_forward_substitution_box[ byte_value3 ];

		round_keys[ 1 ] = libbde_aes_reverse_table0[ byte_value0 ]
			        ^ libbde_aes_reverse_table1[ byte_value1 ]
			        ^ libbde_aes_reverse_table2[ byte_value2 ]
			        ^ libbde_aes_reverse_table3[ byte_value3 ];

		byte_value0 = (uint8_t) encryption_round_keys[ 2 ];
		byte_value1 = (uint8_t) ( encryption_round_keys[ 2 ] >> 8 );
		byte_value2 = (uint8_t) ( encryption_round_keys[ 2 ] >> 16 );
		byte_value3 = (uint8_t) ( encryption_round_keys[ 2 ] >> 24 );

		byte_value0 = libbde_aes_forward_substitution_box[ byte_value0 ];
		byte_value1 = libbde_aes_forward_substitution_box[ byte_value1 ];
		byte_value2 = libbde_aes_forward_substitution_box[ byte_value2 ];
		byte_value3 = libbde_aes_forward_substitution_box[ byte_value3 ];

		round_keys[ 2 ] = libbde_aes_reverse_table0[ byte_value0 ]
			        ^ libbde_aes_reverse_table1[ byte_value1 ]
			        ^ libbde_aes_reverse_table2[ byte_value2 ]
			        ^ libbde_aes_reverse_table3[ byte_value3 ];

		byte_value0 = (uint8_t) encryption_round_keys[ 3 ];
		byte_value1 = (uint8_t) ( encryption_round_keys[ 3 ] >> 8 );
		byte_value2 = (uint8_t) ( encryption_round_keys[ 3 ] >> 16 );
		byte_value3 = (uint8_t) ( encryption_round_keys[ 3 ] >> 24 );

		byte_value0 = libbde_aes_forward_substitution_box[ byte_value0 ];
		byte_value1 = libbde_aes_forward_substitution_box[ byte_value1 ];
		byte_value2 = libbde_aes_forward_substitution_box[ byte_value2 ];
		byte_value3 = libbde_aes_forward_substitution_box[ byte_value3 ];

		round_keys[ 3 ] = libbde_aes_reverse_table0[ byte_value0 ]
			        ^ libbde_aes_reverse_table1[ byte_value1 ]
			        ^ libbde_aes_reverse_table2[ byte_value2 ]
			        ^ libbde_aes_reverse_table3[ byte_value3 ];

		round_keys            += 4;
		encryption_round_keys -= 4;
	}
	round_keys[ 0 ] = encryption_round_keys[ 0 ];
	round_keys[ 1 ] = encryption_round_keys[ 1 ];
	round_keys[ 2 ] = encryption_round_keys[ 2 ];
	round_keys[ 3 ] = encryption_round_keys[ 3 ];

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
	static char *function          = "libbde_aes_set_encryption_key";

#if defined( WINAPI )
	libcstring_system_character_t error_string[ LIBBDE_ERROR_STRING_SIZE ];

	libbde_aes_key_t *wincrypt_key = NULL;
	DWORD error_code               = 0;
	DWORD wincrypt_key_size        = 0;

#elif !defined( LIBBDE_HAVE_AES_SUPPORT )
	uint32_t *round_keys           = NULL;
	size_t key_index               = 0;
	int round_constant_index       = 0;
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
	if( libbde_aes_key_initialize(
	     &wincrypt_key,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create wincrypt key.",
		 function );

		return( -1 );
	}
	if( libbde_aes_key_set(
	     wincrypt_key,
	     key,
	     bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set wincrypt key.",
		 function );

		libbde_aes_key_free(
		 &wincrypt_key,
		 NULL );

		return( -1 );
	}
	wincrypt_key_size = sizeof( libbde_aes_key_t ) - ( ( 256 - bit_size ) / 8 );

	if( CryptImportKey(
	     context->crypt_provider,
	     (CONST BYTE *) &wincrypt_key,
	     wincrypt_key_size,
	     (HCRYPTKEY) NULL,
	     0,
	     &( context->key ) ) == 0 )
	{
		error_code = GetLastError();

		if( libbde_error_string_copy_from_error_number(
		     error_string,
		     LIBBDE_ERROR_STRING_SIZE,
		     error_code,
		     error ) == 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key object with error: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 error_string );
		}
		else
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key object.",
			 function );
		}
		libbde_aes_key_free(
		 &wincrypt_key,
		 NULL );

		return( -1 );
	}
	if( libbde_aes_key_free(
	     &wincrypt_key,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free wincrypt key.",
		 function );

		return( -1 );
	}

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

#else
	/* Align the buffer to next 16-byte blocks
	 */
	context->round_keys = (uint32_t *) ( 16 + ( (intptr_t) context->round_keys_data & ~( 15 ) ) );

	round_keys = context->round_keys;

	for( key_index = 0;
	     key_index < bit_size / 8;
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
			round_keys[ 4 ]  = libbde_aes_round_constants[ round_constant_index ];
			round_keys[ 4 ] ^= round_keys[ 0 ];
			round_keys[ 4 ] ^= (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 3 ] >> 8 ) & 0xff ];
			round_keys[ 4 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 3 ] >> 16 ) & 0xff ] ) << 8;
			round_keys[ 4 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 3 ] >> 24 ) & 0xff ] ) << 16;
			round_keys[ 4 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 3 ] ) & 0xff ] ) << 24;

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
			round_keys[ 6 ]  = libbde_aes_round_constants[ round_constant_index ];
			round_keys[ 6 ] ^= round_keys[ 0 ];
			round_keys[ 6 ] ^= (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 5 ] >> 8 ) & 0xff ];
			round_keys[ 6 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 5 ] >> 16 ) & 0xff ] ) << 8;
			round_keys[ 6 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 5 ] >> 24 ) & 0xff ] ) << 16;
			round_keys[ 6 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 5 ] ) & 0xff ] ) << 24;

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
			round_keys[ 8 ]  = libbde_aes_round_constants[ round_constant_index ];
			round_keys[ 8 ] ^= round_keys[ 0 ];
			round_keys[ 8 ] ^= (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 7 ] >> 8 ) & 0xff ];
			round_keys[ 8 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 7 ] >> 16 ) & 0xff ] ) << 8;
			round_keys[ 8 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 7 ] >> 24 ) & 0xff ] ) << 16;
			round_keys[ 8 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 7 ] ) & 0xff ] ) << 24;

			round_keys[ 9 ]  = round_keys[ 1 ] ^ round_keys[ 8 ];
			round_keys[ 10 ] = round_keys[ 2 ] ^ round_keys[ 9 ];
			round_keys[ 11 ] = round_keys[ 3 ] ^ round_keys[ 10 ];

			round_keys[ 12 ]  = round_keys[ 4 ];
			round_keys[ 12 ] ^= (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 11 ] ) & 0xff ];
			round_keys[ 12 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 11 ] >> 8 ) & 0xff ] ) << 8;
			round_keys[ 12 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 11 ] >> 16 ) & 0xff ] ) << 16;
			round_keys[ 12 ] ^= ( (uint32_t) libbde_aes_forward_substitution_box[ ( round_keys[ 11 ] >> 24 ) & 0xff ] ) << 24;

			round_keys[ 13 ] = round_keys[ 5 ] ^ round_keys[ 12 ];
			round_keys[ 14 ] = round_keys[ 6 ] ^ round_keys[ 13 ];
			round_keys[ 15 ] = round_keys[ 7 ] ^ round_keys[ 14 ];

			round_keys += 8;
		}
	}
#endif
	return( 1 );
}

/* De- or encrypts a block of data using AES-CBC (Cipher Block Chaining)
 * This function expects the input to be a multitude of 16 bytes
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_cbc_crypt(
     libbde_aes_context_t *context,
     int mode,
     uint8_t initialization_vector[ 16 ],
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	static char *function       = "libbde_aes_cbc_crypt";

#if defined( WINAPI )
	DWORD cipher_mode           = CRYPT_MODE_CBC;
	DWORD safe_output_data_size = 0;

#elif defined( HAVE_LIBCRYPTO ) && !defined( HAVE_OPENSSL_AES_H ) && defined( HAVE_OPENSSL_EVP_H )
	uint8_t block_data[ EVP_MAX_BLOCK_LENGTH ];

	const EVP_CIPHER *cipher    = NULL;
	int safe_output_data_size   = 0;

#elif !defined( LIBBDE_HAVE_AES_SUPPORT )
	size_t data_index           = 0;
	uint8_t block_index         = 0;
#endif

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
	if( output_data_size < input_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid ouput data size smaller than input data size.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( input_data_size > (size_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid input data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( CryptSetKeyParam(
	     context->key,
	     KP_MODE,
	     (BYTE*) &cipher_mode,
	     0 ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cipher mode key parameter.",
		 function );

		return( -1 );
	}
	if( CryptSetKeyParam(
	     context->key,
	     KP_IV,
	     (BYTE*) initialization_vector,
	     0 ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set initialization vector key parameter.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     output_data,
	     input_data,
	     input_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy input data to output data.",
		 function );

		return( -1 );
	}
	safe_output_data_size = (DWORD) input_data_size;

	if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
	{
		if( CryptEncrypt(
		     context->key,
		     (HCRYPTHASH) NULL,
		     TRUE,
		     0,
		     output_data,
		     &safe_output_data_size,
		     (DWORD) input_data_size ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
			 "%s: unable to encrypt output data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( CryptDecrypt(
		     context->key,
		     (HCRYPTHASH) NULL,
		     TRUE,
		     0,
		     output_data,
		     &safe_output_data_size ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt output data.",
			 function );

			return( -1 );
		}
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	AES_cbc_encrypt(
	 (unsigned char *) input_data,
	 (unsigned char *) output_data,
	 input_data_size,
	 &( context->key ),
	 (unsigned char *) initialization_vector,
	 mode );

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
	if( memory_set(
	     block_data,
	     0,
	     EVP_MAX_BLOCK_LENGTH ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear input block data.",
		 function );

		return( -1 );
	}
	if( context->bit_size == 128 )
	{
		cipher = EVP_aes_128_cbc();
	}
	else if( context->bit_size == 192 )
	{
		cipher = EVP_aes_192_cbc();
	}
	else if( context->bit_size == 256 )
	{
		cipher = EVP_aes_256_cbc();
	}
	if( EVP_CipherInit_ex(
	     &( context->evp_context ),
	     cipher,
	     NULL,
	     (unsigned char *) context->key,
	     (unsigned char *) initialization_vector,
	     mode ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize cipher.",
		 function );

		return( -1 );
	}
	if( EVP_CipherUpdate(
	     &( context->evp_context ),
	     (unsigned char *) output_data,
	     &safe_output_data_size,
	     (unsigned char *) input_data,
	     input_data_size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update cipher.",
		 function );

		return( -1 );
	}
	/* Just ignore the output of this function
	 */
	EVP_CipherFinal_ex(
	 &( context->evp_context ),
	 (unsigned char *) block_data,
	 &safe_output_data_size );

#else
	while( ( data_index + 16 ) <= input_data_size )
	{
		if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
		{
/* TODO test */
			if( memory_copy(
			     &( output_data[ data_index ] ),
			     &( input_data[ data_index ] ),
			     16 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy input data to output data.",
				 function );

				return( -1 );
			}
			for( block_index = 0;
			     block_index < 16;
			     block_index++ )
			{
				output_data[ data_index + block_index ] ^= initialization_vector[ block_index ];
			}
			if( libbde_aes_ecb_crypt(
			     context,
			     mode,
			     &( output_data[ data_index ] ),
			     16,
			     &( output_data[ data_index ] ),
			     16,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to encrypt output data.",
				 function );

				return( -1 );
			}
			if( memory_copy(
			     initialization_vector,
			     &( output_data[ data_index ] ),
			     16 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy enrypted output data to initialization vector.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libbde_aes_ecb_crypt(
			     context,
			     mode,
			     &( input_data[ data_index ] ),
			     16,
			     &( output_data[ data_index ] ),
			     16,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to decrypt output data.",
				 function );

				return( -1 );
			}
			for( block_index = 0;
			     block_index < 16;
			     block_index++ )
			{
				output_data[ data_index + block_index ] ^= initialization_vector[ block_index ];
			}
			if( memory_copy(
			     initialization_vector,
			     &( input_data[ data_index ] ),
			     16 ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy enrypted input data to initialization vector.",
				 function );

				return( -1 );
			}
		}
		data_index += 16;
  	}
#endif
	return( 1 );
}

/* De- or encrypts a block of data using AES-CCM (Counter with CBC-MAC)
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
	uint8_t internal_initialization_vector[ 16 ];
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
/* TODO encryption currently not supported */
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
	if( initialization_vector_size >= (size_t) 15 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid initialization vector size value out of bounds.",
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
	/* The internal IV consists of:
	 * 1 byte size value formatted as: 15 - IV size - 1
	 * a maximum of 14 bytes containing IV bytes
	 * 1 byte counter
	 */
	if( memory_set(
	     internal_initialization_vector,
	     0,
	     16 ) == NULL )
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
	internal_initialization_vector[ 0 ] = 15 - (uint8_t) initialization_vector_size - 1;

	while( data_index < input_data_size )
	{
		if( libbde_aes_ecb_crypt(
		     context,
		     LIBBDE_AES_CRYPT_MODE_ENCRYPT,
		     internal_initialization_vector,
		     16,
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

#ifdef TODO
/* De- or encrypts a block of data using AES-CFB (Cipher Feedback Mode)
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
	if( *initialization_vector_index >= 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid initialization vector index .",
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
	if( output_data_size > input_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid ouput data size value exceeds input data size.",
		 function );

		return( -1 );
	}
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
#endif /* defined( TODO ) */

/* De- or encrypts a 16-byte block using AES-ECB (Electronic CodeBook)
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
	static char *function       = "libbde_aes_ecb_crypt";
	int result                  = 1;

#if defined( WINAPI )
	DWORD cipher_mode           = CRYPT_MODE_ECB;
	DWORD safe_output_data_size = 0;

#elif defined( HAVE_LIBCRYPTO ) && !defined( HAVE_OPENSSL_AES_H ) && defined( HAVE_OPENSSL_EVP_H )
	uint8_t block_data[ EVP_MAX_BLOCK_LENGTH ];

	const EVP_CIPHER *cipher    = NULL;
	int safe_output_data_size   = 0;

#elif !defined( LIBBDE_HAVE_AES_SUPPORT )
	uint32_t cipher_values_32bit[ 4 ];
	uint32_t values_32bit[ 4 ];

	uint32_t *round_keys        = NULL;
	uint32_t substitution_value = 0;
	uint32_t table_value        = 0;
	int round_key_iterator      = 0;
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
	if( input_data_size < 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid input data size value too small.",
		 function );

		return( -1 );
	}
	if( output_data_size < 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid output data size value too small.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( CryptSetKeyParam(
	     context->key,
	     KP_MODE,
	     (BYTE*) &cipher_mode,
	     0 ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cipher mode key parameter.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     output_data,
	     input_data,
	     input_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy input data to output data.",
		 function );

		return( -1 );
	}
	safe_output_data_size = input_data_size;

	if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
	{
		if( CryptEncrypt(
		     context->key,
		     (HCRYPTHASH) NULL,
		     TRUE,
		     0,
		     output_data,
		     &safe_output_data_size,
		     (DWORD) input_data_size ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
			 "%s: unable to encrypt output data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( CryptDecrypt(
		     context->key,
		     (HCRYPTHASH) NULL,
		     TRUE,
		     0,
		     output_data,
		     &safe_output_data_size ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
			 "%s: unable to decrypt output data.",
			 function );

			return( -1 );
		}
	}


#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_AES_H )
	AES_ecb_encrypt(
	 (unsigned char *) input_data,
	 (unsigned char *) output_data,
	 &( context->key ),
	 mode );

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
	if( memory_set(
	     block_data,
	     0,
	     EVP_MAX_BLOCK_LENGTH ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block data.",
		 function );

		return( -1 );
	}
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
	if( EVP_CipherInit_ex(
	     &( context->evp_context ),
	     cipher,
	     NULL,
	     (unsigned char *) context->key,
	     NULL,
	     mode ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize cipher.",
		 function );

		return( -1 );
	}
	if( EVP_CipherUpdate(
	     &( context->evp_context ),
	     (unsigned char *) output_data,
	     &safe_output_data_size,
	     (unsigned char *) input_data,
	     16 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update cipher.",
		 function );

		return( -1 );
	}
	/* Just ignore the output of this function
	 */
	EVP_CipherFinal_ex(
	 &( context->evp_context ),
	 (unsigned char *) block_data,
	 &safe_output_data_size );

#else
	byte_stream_copy_to_uint32_little_endian(
	 &( input_data[ 0 ] ),
	 values_32bit[ 0 ] );

	byte_stream_copy_to_uint32_little_endian(
	 &( input_data[ 4 ] ),
	 values_32bit[ 1 ] );

	byte_stream_copy_to_uint32_little_endian(
	 &( input_data[ 8 ] ),
	 values_32bit[ 2 ] );

	byte_stream_copy_to_uint32_little_endian(
	 &( input_data[ 12 ] ),
	 values_32bit[ 3 ] );

	round_keys = context->round_keys;

	values_32bit[ 0 ] ^= round_keys[ 0 ];
	values_32bit[ 1 ] ^= round_keys[ 1 ];
	values_32bit[ 2 ] ^= round_keys[ 2 ];
	values_32bit[ 3 ] ^= round_keys[ 3 ];

	round_keys += 4;

	if( mode == LIBBDE_AES_CRYPT_MODE_ENCRYPT )
	{
		for( round_key_iterator = ( context->number_of_round_keys / 2 );
		     round_key_iterator > 1;
		     round_key_iterator-- )
		{
			libbde_aes_calculate_forward_table_round(
			 round_keys,
			 cipher_values_32bit,
			 values_32bit,
			 table_value );

			round_keys += 4;

			libbde_aes_calculate_forward_table_round(
			 round_keys,
			 values_32bit,
			 cipher_values_32bit,
			 table_value );

			round_keys += 4;
		}
		libbde_aes_calculate_forward_table_round(
		 round_keys,
		 cipher_values_32bit,
		 values_32bit,
		 table_value );

		round_keys += 4;

		libbde_aes_calculate_forward_substitution_round(
		 round_keys,
		 values_32bit,
		 cipher_values_32bit,
		 substitution_value );
	}
	else
	{
/* TODO test */
		for( round_key_iterator = ( context->number_of_round_keys / 2 );
		     round_key_iterator > 1;
		     round_key_iterator-- )
		{
			libbde_aes_calculate_reverse_table_round(
			 round_keys,
			 cipher_values_32bit,
			 values_32bit,
			 table_value );

			round_keys += 4;

			libbde_aes_calculate_reverse_table_round(
			 round_keys,
			 values_32bit,
			 cipher_values_32bit,
			 table_value );

			round_keys += 4;
		}
		libbde_aes_calculate_reverse_table_round(
		 round_keys,
		 cipher_values_32bit,
		 values_32bit,
		 table_value );

		round_keys += 4;

		libbde_aes_calculate_reverse_substitution_round(
		 round_keys,
		 values_32bit,
		 cipher_values_32bit,
		 table_value );
	}
	byte_stream_copy_from_uint32_little_endian(
	 &( output_data[ 0 ] ),
	 values_32bit[ 0 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( output_data[ 4 ] ),
	 values_32bit[ 1 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( output_data[ 8 ] ),
	 values_32bit[ 2 ] );

	byte_stream_copy_from_uint32_little_endian(
	 &( output_data[ 12 ] ),
	 values_32bit[ 3 ] );

	if( memory_set(
	     values_32bit,
	     0,
	     sizeof( uint32_t ) * 4 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear values 32-bit.",
		 function );

		result = -1;
	}
	if( memory_set(
	     cipher_values_32bit,
	     0,
	     sizeof( uint32_t ) * 4 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear cipher values 32-bit.",
		 function );

		result = -1;
	}
#endif
	return( result );
}

