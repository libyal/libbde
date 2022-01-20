/*
 * Recovery functions
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
     libcerror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf8_string_t *split_string = NULL;
	uint8_t *string_segment                     = NULL;
	static char *function                       = "libbde_utf8_recovery_password_calculate_hash";
	size_t string_segment_index                 = 0;
	size_t string_segment_size                  = 0;
	uint64_t value_64bit                        = 0;
	int number_of_segments                      = 0;
	int result                                  = 0;
	int segment_index                           = 0;

	if( recovery_password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			string_segment_index = 0;

			if( libfvalue_utf8_string_with_index_copy_to_integer(
			     string_segment,
			     string_segment_size,
			     &string_segment_index,
			     &value_64bit,
			     16,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_64bit % 11 ) != 0 )
			{
				break;
			}
			value_64bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_64bit > (uint64_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_64bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf8_string_free(
	     &split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libcnotify_print_data(
			 binary_recovery_password,
			 16,
			 0 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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
     libcerror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf16_string_t *split_string = NULL;
	uint16_t *string_segment                     = NULL;
	static char *function                        = "libbde_utf16_recovery_password_calculate_hash";
	size_t string_segment_index                  = 0;
	size_t string_segment_size                   = 0;
	uint64_t value_64bit                         = 0;
	int number_of_segments                       = 0;
	int result                                   = 0;
	int segment_index                            = 0;

	if( recovery_password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf16_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			string_segment_index = 0;

			if( libfvalue_utf16_string_with_index_copy_to_integer(
			     string_segment,
			     string_segment_size,
			     &string_segment_index,
			     &value_64bit,
			     16,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_64bit % 11 ) != 0 )
			{
				break;
			}
			value_64bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_64bit > (uint64_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_64bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf16_string_free(
	     &split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libcnotify_print_data(
			 binary_recovery_password,
			 16,
			 0 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
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

