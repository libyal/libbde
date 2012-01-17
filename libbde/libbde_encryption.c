/*
 * Encryption functions
 *
 * Copyright (C) 2011-2012, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libbde_definitions.h"
#include "libbde_diffuser.h"
#include "libbde_encryption.h"
#include "libbde_libcaes.h"

/* Initialize an encryption context
 * Make sure the value encryption context is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_initialize(
     libbde_encryption_context_t **context,
     uint32_t method,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_initialize";

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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid context value already set.",
		 function );

		return( -1 );
	}
	*context = memory_allocate_structure(
	            libbde_encryption_context_t );

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
	     sizeof( libbde_encryption_context_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		memory_free(
		 *context );

		*context = NULL;

		return( -1 );
	}
	if( libcaes_context_initialize(
	     &( ( *context )->fvek_decryption_context ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize FVEK decryption context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &( ( *context )->fvek_encryption_context ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize FVEK encryption context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &( ( *context )->tweak_decryption_context ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize TWEAK decryption context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &( ( *context )->tweak_encryption_context ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize TWEAK encryption context.",
		 function );

		goto on_error;
	}
	( *context )->method = method;

	return( 1 );

on_error:
	if( *context != NULL )
	{
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
int libbde_encryption_free(
     libbde_encryption_context_t **context,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_free";
	int result            = 1;

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
		if( libcaes_context_free(
		     &( ( *context )->fvek_decryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free FVEK decryption context.",
			 function );

			result = -1;
		}
		if( libcaes_context_free(
		     &( ( *context )->fvek_encryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free FVEK encryption context.",
			 function );

			result = -1;
		}
		if( libcaes_context_free(
		     &( ( *context )->tweak_decryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free TWEAK decryption context.",
			 function );

			result = -1;
		}
		if( libcaes_context_free(
		     &( ( *context )->tweak_encryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free TWEAK encryption context.",
			 function );

			result = -1;
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
int libbde_encryption_set_keys(
     libbde_encryption_context_t *context,
     uint8_t full_volume_encryption_key[ 32 ],
     uint8_t tweak_key[ 32 ],
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_set_keys";
	size_t key_bit_size   = 0;

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
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER ) )
	{
		key_bit_size = 128;
	}
	else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC )
	      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		key_bit_size = 256;
	}
	/* The volume master key is always 256-bit in size
	 */
	if( libcaes_crypt_set_key(
	     context->fvek_decryption_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     full_volume_encryption_key,
	     key_bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set full volume encryption key in decryption context.",
		 function );

		return( -1 );
	}
	if( libcaes_crypt_set_key(
	     context->fvek_encryption_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     full_volume_encryption_key,
	     key_bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set full volume encryption key in encryption context.",
		 function );

		return( -1 );
	}
	/* The TWEAK key is only used with diffuser
	 */
	if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
	 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
	{
		if( libcaes_crypt_set_key(
		     context->tweak_decryption_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     tweak_key,
		     key_bit_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set tweak key in decryption context.",
			 function );

			return( -1 );
		}
		if( libcaes_crypt_set_key(
		     context->tweak_encryption_context,
		     LIBCAES_CRYPT_MODE_ENCRYPT,
		     tweak_key,
		     key_bit_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set tweak key in encryption context.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* De- or encrypts a block of data
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_crypt(
     libbde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t block_key,
     liberror_error_t **error )
{
	uint8_t block_key_data[ 16 ];
	uint8_t initialization_vector[ 16 ];
	uint8_t sector_key_data[ 32 ];

	static char *function        = "libbde_encryption_crypt";
	size_t data_index            = 0;
	size_t sector_key_data_index = 0;

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
	if( ( mode != LIBBDE_ENCYPTION_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBBDE_ENCYPTION_CRYPT_MODE_ENCRYPT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     initialization_vector,
	     0,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear initialization vector.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     block_key_data,
	     0,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block key data.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     sector_key_data,
	     0,
	     32 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector key data.",
		 function );

		return( -1 );
	}
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to encrypt initialization vector.",
		 function );

		return( -1 );
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt sector key data.",
			 function );

			return( -1 );
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt sector key data.",
			 function );

			return( -1 );
		}
	}
	if( mode == LIBBDE_ENCYPTION_CRYPT_MODE_ENCRYPT )
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to encrypt data using Diffuser.",
				 function );

				return( -1 );
			}
		}
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to AES-CBC encrypt output data.",
			 function );

			return( -1 );
		}
	}
	else
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to AES-CBC decrypt output data.",
			 function );

			return( -1 );
		}
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
			if( libbde_diffuser_decrypt(
			     output_data,
			     output_data_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
				 "%s: unable to decrypt data using Diffuser.",
				 function );

				return( -1 );
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
}

