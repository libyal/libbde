/*
 * Password functions
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
	     iteration_count < 0xffff;
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

