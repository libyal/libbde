/*
 * Encryption functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libbde_definitions.h"
#include "libbde_diffuser.h"
#include "libbde_encryption_context.h"
#include "libbde_libcaes.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"

/* Creates an encryption context
 * Make sure the value encryption context is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_context_initialize(
     libbde_encryption_context_t **context,
     uint16_t method,
     libcerror_error_t **error )
{
	static char *function = "libbde_encryption_context_initialize";

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	if( ( method != LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_AES_256_XTS )
	 && ( method != LIBBDE_ENCRYPTION_METHOD_NONE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported method.",
		 function );

		return( -1 );
	}
	*context = memory_allocate_structure(
	            libbde_encryption_context_t );

	if( *context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *context,
	     0,
	     sizeof( libbde_encryption_context_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 *context );

		*context = NULL;

		return( -1 );
	}
	if( ( method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 || ( method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	 || ( method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( libcaes_context_initialize(
		     &( ( *context )->fvek_decryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize FVEK decryption context.",
			 function );

			goto on_error;
		}
		if( libcaes_context_initialize(
		     &( ( *context )->fvek_encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize FVEK encryption context.",
			 function );

			goto on_error;
		}
	}
	if( ( method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( libcaes_context_initialize(
		     &( ( *context )->tweak_decryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize TWEAK decryption context.",
			 function );

			goto on_error;
		}
		if( libcaes_context_initialize(
		     &( ( *context )->tweak_encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize TWEAK encryption context.",
			 function );

			goto on_error;
		}
	}
	if( ( method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
	 || ( method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
	{
		if( libcaes_tweaked_context_initialize(
		     &( ( *context )->fvek_decryption_tweaked_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize FVEK decryption tweaked context.",
			 function );

			goto on_error;
		}
		if( libcaes_tweaked_context_initialize(
		     &( ( *context )->fvek_encryption_tweaked_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize FVEK encryption tweaked context.",
			 function );

			goto on_error;
		}
	}
	( *context )->method = method;

	return( 1 );

on_error:
	if( *context != NULL )
	{
		if( ( *context )->fvek_encryption_tweaked_context != NULL )
		{
			libcaes_tweaked_context_free(
			 &( ( *context )->fvek_encryption_tweaked_context ),
			 NULL );
		}
		if( ( *context )->fvek_decryption_tweaked_context != NULL )
		{
			libcaes_tweaked_context_free(
			 &( ( *context )->fvek_decryption_tweaked_context ),
			 NULL );
		}
		if( ( *context )->tweak_encryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->tweak_encryption_context ),
			 NULL );
		}
		if( ( *context )->tweak_decryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->tweak_decryption_context ),
			 NULL );
		}
		if( ( *context )->fvek_encryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->fvek_encryption_context ),
			 NULL );
		}
		if( ( *context )->fvek_decryption_context != NULL )
		{
			libcaes_context_free(
			 &( ( *context )->fvek_decryption_context ),
			 NULL );
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( -1 );
}

/* Frees an encryption context
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_context_free(
     libbde_encryption_context_t **context,
     libcerror_error_t **error )
{
	static char *function = "libbde_encryption_context_free";
	int result            = 1;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		if( ( *context )->fvek_decryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->fvek_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free FVEK decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->fvek_encryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->fvek_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free FVEK encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->tweak_decryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->tweak_decryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free TWEAK decryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->tweak_encryption_context != NULL )
		{
			if( libcaes_context_free(
			     &( ( *context )->tweak_encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free TWEAK encryption context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->fvek_decryption_tweaked_context != NULL )
		{
			if( libcaes_tweaked_context_free(
			     &( ( *context )->fvek_decryption_tweaked_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free FVEK decryption tweaked context.",
				 function );

				result = -1;
			}
		}
		if( ( *context )->fvek_encryption_tweaked_context != NULL )
		{
			if( libcaes_tweaked_context_free(
			     &( ( *context )->fvek_encryption_tweaked_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free FVEK encryption tweaked context.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( result );
}

/* Sets the de- and encryption keys
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_context_set_keys(
     libbde_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_encryption_context_set_keys";
	size_t key_bit_size   = 0;
	size_t key_byte_size  = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( tweak_key == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid tweak key.",
			 function );

			return( -1 );
		}
		if( tweak_key_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid tweak key size value exceeds maximum.",
			 function );

			return( -1 );
		}
	}
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER ) )
	{
		key_byte_size = 16;
	}
	else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		key_byte_size = 32;
	}
	else if( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
	{
		key_byte_size = 32;
	}
	else if( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS )
	{
		key_byte_size = 64;
	}
	if( key_size < key_byte_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid key value too small.",
		 function );

		return( -1 );
	}
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( tweak_key_size < key_byte_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid tweak key value too small.",
			 function );

			return( -1 );
		}
	}
	key_bit_size = key_byte_size * 8;

	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( libcaes_context_set_key(
		     context->fvek_decryption_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     key,
		     key_bit_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key in decryption context.",
			 function );

			return( -1 );
		}
		if( libcaes_context_set_key(
		     context->fvek_encryption_context,
		     LIBCAES_CRYPT_MODE_ENCRYPT,
		     key,
		     key_bit_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key in encryption context.",
			 function );

			return( -1 );
		}
		/* The TWEAK key is only used with diffuser
		 */
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			if( libcaes_context_set_key(
			     context->tweak_decryption_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     tweak_key,
			     key_bit_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set tweak key in decryption context.",
				 function );

				return( -1 );
			}
			if( libcaes_context_set_key(
			     context->tweak_encryption_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     tweak_key,
			     key_bit_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set tweak key in encryption context.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
	      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
	{
		key_byte_size /= 2;
		key_bit_size  /= 2;

		if( libcaes_tweaked_context_set_keys(
		     context->fvek_decryption_tweaked_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     key,
		     key_bit_size,
		     &( key[ key_byte_size ] ),
		     key_bit_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in decryption tweaked context.",
			 function );

			return( -1 );
		}
		if( libcaes_tweaked_context_set_keys(
		     context->fvek_encryption_tweaked_context,
		     LIBCAES_CRYPT_MODE_ENCRYPT,
		     key,
		     key_bit_size,
		     &( key[ key_byte_size ] ),
		     key_bit_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in encryption tweaked context.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* De- or encrypts a block of data
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_context_crypt(
     libbde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t block_key,
     libcerror_error_t **error )
{
	uint8_t block_key_data[ 16 ];
	uint8_t initialization_vector[ 16 ];
	uint8_t sector_key_data[ 32 ];

	static char *function        = "libbde_encryption_context_crypt";
	size_t data_index            = 0;
	size_t sector_key_data_index = 0;

	if( context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( ( mode != LIBBDE_ENCRYPTION_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBBDE_ENCRYPTION_CRYPT_MODE_ENCRYPT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     initialization_vector,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear initialization vector.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     block_key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block key data.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     sector_key_data,
	     0,
	     32 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector key data.",
		 function );

		return( -1 );
	}
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		byte_stream_copy_from_uint64_little_endian(
		 block_key_data,
		 block_key );

		/* The block key for the initialization vector is encrypted
		 * with the FVEK
		 */
		if( libcaes_crypt_ecb(
		     context->fvek_encryption_context,
		     LIBCAES_CRYPT_MODE_ENCRYPT,
		     block_key_data,
		     16,
		     initialization_vector,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt initialization vector.",
			 function );

			goto on_error;
		}
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			/* The block key for the sector key data is encrypted
			 * with the TWEAK key
			 */
			if( libcaes_crypt_ecb(
			     context->tweak_encryption_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     block_key_data,
			     16,
			     sector_key_data,
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to encrypt sector key data.",
				 function );

				goto on_error;
			}
			/* Set the last byte to contain 0x80 (128)
			 */
			block_key_data[ 15 ] = 0x80;

			if( libcaes_crypt_ecb(
			     context->tweak_encryption_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     block_key_data,
			     16,
			     &( sector_key_data[ 16 ] ),
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to encrypt sector key data.",
				 function );

				goto on_error;
			}
		}
	}
	else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
	      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
	{
		byte_stream_copy_from_uint64_little_endian(
		 initialization_vector,
		 block_key );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: initialization vector:\n",
		 function );
		libcnotify_print_data(
		 initialization_vector,
		 16,
		 0 );
	}
#endif
	if( mode == LIBBDE_ENCRYPTION_CRYPT_MODE_ENCRYPT )
	{
/* TODO safe guard input data ? */
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			sector_key_data_index = 0;

			for( data_index = 0;
			     data_index < input_data_size;
			     data_index++ )
			{
				output_data[ data_index ] ^= sector_key_data[ sector_key_data_index ];

				sector_key_data_index++;

				if( sector_key_data_index >= 32 )
				{
					sector_key_data_index -= 32;
				}
			}
			if( libbde_diffuser_encrypt(
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to encrypt data using Diffuser.",
				 function );

				goto on_error;
			}
		}
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			if( libcaes_crypt_cbc(
			     context->fvek_encryption_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     initialization_vector,
			     16,
			     input_data,
			     input_data_size,
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to AES-CBC encrypt output data.",
				 function );

				goto on_error;
			}
		}
		else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
		      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
		{
			if( libcaes_crypt_xts(
			     context->fvek_encryption_tweaked_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     initialization_vector,
			     16,
			     input_data,
			     input_data_size,
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to AES-XTS decrypt output data.",
				 function );

				goto on_error;
			}
		}
	}
	else
	{
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			if( libcaes_crypt_cbc(
			     context->fvek_decryption_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     initialization_vector,
			     16,
			     input_data,
			     input_data_size,
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to AES-CBC decrypt output data.",
				 function );

				goto on_error;
			}
		}
		else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
		      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
		{
			if( libcaes_crypt_xts(
			     context->fvek_decryption_tweaked_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     initialization_vector,
			     16,
			     input_data,
			     input_data_size,
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to AES-XTS decrypt output data.",
				 function );

				goto on_error;
			}
		}
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			if( libbde_diffuser_decrypt(
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
				 "%s: unable to decrypt data using Diffuser.",
				 function );

				goto on_error;
			}
			sector_key_data_index = 0;

			for( data_index = 0;
			     data_index < input_data_size;
			     data_index++ )
			{
				output_data[ data_index ] ^= sector_key_data[ sector_key_data_index ];

				sector_key_data_index++;

				if( sector_key_data_index >= 32 )
				{
					sector_key_data_index -= 32;
				}
			}
		}
	}
	return( 1 );

on_error:
	memory_set(
	 sector_key_data,
	 0,
	 32 );

	memory_set(
	 block_key_data,
	 0,
	 16 );

	memory_set(
	 initialization_vector,
	 0,
	 16 );

	return( -1 );
}

