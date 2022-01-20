/*
 * Password functions
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

#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libhmac.h"
#include "libbde_libuna.h"
#include "libbde_password.h"

/* Calculates the SHA256 hash of an UTF-8 formatted password
 * Returns 1 if successful or -1 on error
 */
int libbde_utf8_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t *password_hash,
     size_t password_hash_size,
     libcerror_error_t **error )
{
	uint8_t *utf16_stream    = NULL;
	static char *function    = "libbde_utf8_password_calculate_hash";
	size_t utf16_stream_size = 0;

	if( password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password hash.",
		 function );

		return( -1 );
	}
	if( password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_length,
	     &utf16_stream_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 stream size.",
		 function );

		goto on_error;
	}
	if( ( utf16_stream_size == 0 )
	 || ( utf16_stream_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 stream size value out of bounds.",
		 function );

		goto on_error;
	}
	utf16_stream = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * utf16_stream_size );

	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-8 string to UTF-16 stream size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: password:\n",
			 function );
			libcnotify_print_data(
			 utf16_stream,
			 utf16_stream_size,
			 0 );
		}
#endif
	/* Ignore the byte-order mark in the UTF16 stream
	 */
	if( libhmac_sha256_calculate(
	     &( utf16_stream[ 2 ] ),
	     utf16_stream_size - 2,
	     password_hash,
	     password_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_calculate(
	     password_hash,
	     password_hash_size,
	     password_hash,
	     password_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     utf16_stream,
	     0,
	     utf16_stream_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
     uint8_t *password_hash,
     size_t password_hash_size,
     libcerror_error_t **error )
{
	uint8_t *utf16_stream    = NULL;
	static char *function    = "libbde_utf16_password_calculate_hash";
	size_t utf16_stream_size = 0;

	if( password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password hash.",
		 function );

		return( -1 );
	}
	if( password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_stream_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     &utf16_stream_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 stream size.",
		 function );

		goto on_error;
	}
	if( ( utf16_stream_size == 0 )
	 || ( utf16_stream_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UTF-16 stream size value out of bounds.",
		 function );

		goto on_error;
	}
	utf16_stream = (uint8_t *) memory_allocate(
	                            sizeof( uint8_t ) * utf16_stream_size );

	if( utf16_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UTF-16 string to UTF-16 stream size.",
		 function );

		goto on_error;
	}
	/* Ignore the byte-order mark in the UTF16 stream
	 */
	if( libhmac_sha256_calculate(
	     &( utf16_stream[ 2 ] ),
	     utf16_stream_size - 2,
	     password_hash,
	     password_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( libhmac_sha256_calculate(
	     password_hash,
	     password_hash_size,
	     password_hash,
	     password_hash_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate password hash.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     utf16_stream,
	     0,
	     utf16_stream_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
     const uint8_t *password_hash,
     size_t password_hash_size,
     const uint8_t *salt,
     size_t salt_size,
     uint8_t *key,
     size_t key_size,
     libcerror_error_t **error )
{
	libbde_password_key_data_t password_key_data;

	static char *function = "libbde_password_calculate_key";

	if( password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password hash.",
		 function );

		return( -1 );
	}
	if( password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( salt == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid salt.",
		 function );

		return( -1 );
	}
	if( salt_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: salt size value out of bounds.",
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
	if( key_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: key size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &password_key_data,
	     0,
	     sizeof( libbde_password_key_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear password key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( password_key_data.initial_sha256_hash ),
	     password_hash,
	     password_hash_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy password hash to password key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( password_key_data.salt ),
	     salt,
	     salt_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to password key data.",
		 function );

		return( -1 );
	}
	/* The password key is the SHA256 digest hash after 0x100000 key iterations
	 */
	for( password_key_data.iteration_count = 0;
	     password_key_data.iteration_count < 0x000fffffUL;
	     password_key_data.iteration_count += 1 )
	{
		if( libhmac_sha256_calculate(
		     (uint8_t *) &password_key_data,
		     sizeof( libbde_password_key_data_t ),
		     password_key_data.last_sha256_hash,
		     LIBHMAC_SHA256_HASH_SIZE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate SHA256.",
			 function );

			return( -1 );
		}
	}
	if( libhmac_sha256_calculate(
	     (uint8_t *) &password_key_data,
	     sizeof( libbde_password_key_data_t ),
	     key,
	     key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate SHA256.",
		 function );

		return( -1 );
	}
	return( 1 );
}

