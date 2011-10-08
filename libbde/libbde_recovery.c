/*
 * Recovery functions
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

#include "libbde_libfvalue.h"
#include "libbde_libhmac.h"
#include "libbde_recovery.h"

/* Calculates the SHA256 hash of an UTF-8 formatted recovery password
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_utf8_recovery_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     liberror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf8_string_t *split_string = NULL;
	uint8_t *string_segment                     = NULL;
	static char *function                       = "libbde_utf8_recovery_password_calculate_hash";
	size_t string_segment_size                  = 0;
	uint32_t value_32bit                        = 0;
	int number_of_segments                      = 0;
	int result                                  = 0;
	int segment_index                           = 0;

	if( recovery_password_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: recovery password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     utf8_string,
	     utf8_string_length + 1,
	     (uint8_t) '-',
 	     &split_string,
 	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments in split string.",
		 function );

		goto on_error;
	}
	/* The recovery password consists of 8 segments
	 */
	if( number_of_segments == 8 )
	{
		for( segment_index = 0;
		     segment_index < number_of_segments;
		     segment_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     split_string,
			     segment_index,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			if( libfvalue_utf8_string_decimal_copy_to_32bit(
			     string_segment,
			     string_segment_size,
			     &value_32bit,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_32bit % 11 ) != 0 )
			{
				break;
			}
			value_32bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_32bit > (uint32_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_32bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf8_string_free(
	     &split_string,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libnotify_print_data(
			 binary_recovery_password,
			 16 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear binary recovery password.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &split_string,
		 NULL );
	}
	memory_set(
	 binary_recovery_password,
	 0,
	 16 );

	return( -1 );
}

/* Calculates the SHA256 hash of an UTF-16 formatted recovery password
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_utf16_recovery_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     liberror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf16_string_t *split_string = NULL;
	uint16_t *string_segment                     = NULL;
	static char *function                        = "libbde_utf16_recovery_password_calculate_hash";
	size_t string_segment_size                   = 0;
	uint32_t value_32bit                         = 0;
	int number_of_segments                       = 0;
	int result                                   = 0;
	int segment_index                            = 0;

	if( recovery_password_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: recovery password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_split(
	     utf16_string,
	     utf16_string_length + 1,
	     (uint16_t) '-',
 	     &split_string,
 	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf16_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments in split string.",
		 function );

		goto on_error;
	}
	/* The recovery password consists of 8 segments
	 */
	if( number_of_segments == 8 )
	{
		for( segment_index = 0;
		     segment_index < number_of_segments;
		     segment_index++ )
		{
			if( libfvalue_split_utf16_string_get_segment_by_index(
			     split_string,
			     segment_index,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			if( libfvalue_utf16_string_decimal_copy_to_32bit(
			     string_segment,
			     string_segment_size,
			     &value_32bit,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_32bit % 11 ) != 0 )
			{
				break;
			}
			value_32bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_32bit > (uint32_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_32bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf16_string_free(
	     &split_string,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libnotify_print_data(
			 binary_recovery_password,
			 16 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear binary recovery password.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf16_string_free(
		 &split_string,
		 NULL );
	}
	memory_set(
	 binary_recovery_password,
	 0,
	 16 );

	return( -1 );
}

/* Calculates the recovery key for a certain recovery password and salt
 * Returns 1 if successful or -1 on error
 */
int libbde_recovery_calculate_key(
     const uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     const uint8_t *salt,
     size_t salt_size,
     uint8_t *key,
     size_t key_size,
     liberror_error_t **error )
{
	libbde_recovery_key_data_t recovery_key_data;

	static char *function = "libbde_recovery_calculate_key";

	if( recovery_password_hash == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: recovery password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( salt == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid salt.",
		 function );

		return( -1 );
	}
	if( salt_size != 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: salt size value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( key_size != 32 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: key size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     &recovery_key_data,
	     0,
	     sizeof( libbde_recovery_key_data_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear recovery key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( recovery_key_data.initial_sha256_hash ),
	     recovery_password_hash,
	     recovery_password_hash_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy recovery password hash to recovery key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( recovery_key_data.salt ),
	     salt,
	     salt_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to recovery key data.",
		 function );

		return( -1 );
	}
	/* The recovery key is the SHA256 digest hash after 0x100000 key iterations
	 */
	for( recovery_key_data.iteration_count = 0;
	     recovery_key_data.iteration_count < 0x000fffffUL;
	     recovery_key_data.iteration_count += 1 )
	{
		if( libhmac_sha256_calculate(
		     (uint8_t *) &recovery_key_data,
		     sizeof( libbde_recovery_key_data_t ),
		     recovery_key_data.last_sha256_hash,
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
	     (uint8_t *) &recovery_key_data,
	     sizeof( libbde_recovery_key_data_t ),
	     key,
	     key_size,
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

