/*
 * Password functions
 *
 * Copyright (C) 2011, Google Inc.
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libbde_libhmac.h"
#include "libbde_libuna.h"
#include "libbde_password.h"

/* Calculates the SHA256 hash of an UTF-8 formatted password
 * Returns 1 if successful or -1 on error
 */
int libbde_utf8_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t password_hash[ 32 ],
     liberror_error_t **error )
{
	uint8_t *utf16_stream    = NULL;
	static char *function    = "libbde_utf8_password_calculate_hash";
	size_t utf16_stream_size = 0;

	if( libuna_utf16_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_length,
	     &utf16_stream_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 stream size.",
		 function );

		goto on_error;
	}
	utf16_stream = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * utf16_stream_size );

	if( utf16_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-16 stream.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_copy_from_utf8(
	     utf16_stream,
	     utf16_stream_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string,
	     utf8_string_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-8 string to UTF-16 stream size.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_calculate(
	     &( utf16_stream[ 2 ] ),
	     utf16_stream_size - 4,
	     password_hash,
	     LIBHMAC_SHA256_HASH_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     utf16_stream,
	     0,
	     utf16_stream_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear UTF-16 stream.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf16_stream );

	return( 1 );

on_error:
	if( utf16_stream != NULL )
	{
		memory_set(
		 utf16_stream,
		 0,
		 utf16_stream_size );
		memory_free(
		 utf16_stream );
	}
	return( -1 );
}

/* Calculates the SHA256 hash of an UTF-16 formatted password
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libbde_utf16_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t password_hash[ 32 ],
     liberror_error_t **error )
{
	uint8_t *utf16_stream    = NULL;
	static char *function    = "libbde_utf16_password_calculate_hash";
	size_t utf16_stream_size = 0;

	if( libuna_utf16_stream_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     &utf16_stream_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 stream size.",
		 function );

		goto on_error;
	}
	utf16_stream = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * utf16_stream_size );

	if( utf16_stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create UTF-16 stream.",
		 function );

		goto on_error;
	}
	if( libuna_utf16_stream_copy_from_utf16(
	     utf16_stream,
	     utf16_stream_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-16 string to UTF-16 stream size.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_calculate(
	     &( utf16_stream[ 2 ] ),
	     utf16_stream_size - 4,
	     password_hash,
	     LIBHMAC_SHA256_HASH_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     utf16_stream,
	     0,
	     utf16_stream_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear UTF-16 stream.",
		 function );

		goto on_error;
	}
	memory_free(
	 utf16_stream );

	return( 1 );

on_error:
	if( utf16_stream != NULL )
	{
		memory_set(
		 utf16_stream,
		 0,
		 utf16_stream_size );
		memory_free(
		 utf16_stream );
	}
	return( -1 );
}

/* Calculates the password key for a certain password and salt
 * Returns 1 if successful or -1 on error
 */
int libbde_password_calculate_key(
     const uint8_t password_hash[ 32 ],
     const uint8_t salt[ 16 ],
     uint8_t key[ 32 ],
     liberror_error_t **error )
{
	libbde_password_key_data_t password_key_data;

	static char *function    = "libbde_password_calculate_key";
	uint32_t iteration_count = 0;

	if( memory_set(
	     &password_key_data,
	     0,
	     sizeof( libbde_password_key_data_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear password key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( password_key_data.sha256_hash ),
	     password_hash,
	     32 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy password hash to password key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( password_key_data.salt ),
	     salt,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to password key data.",
		 function );

		return( -1 );
	}
	/* The password key is the SHA256 digest hash after 0x10000 key iterations
	 */
	for( iteration_count = 0;
	     iteration_count < 0x0000ffffUL;
	     iteration_count += 1 )
	{
		if( libhmac_sha256_calculate(
		     (uint8_t *) &password_key_data,
		     sizeof( libbde_password_key_data_t ),
		     password_key_data.sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate SHA256.",
			 function );

			return( -1 );
		}
	}
	if( libhmac_sha256_calculate(
	     (uint8_t *) &password_key_data,
	     sizeof( libbde_password_key_data_t ),
	     key,
	     32,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate SHA256.",
		 function );

		return( -1 );
	}
	return( 1 );
}

