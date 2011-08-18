/*
 * Recovery functions
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

#include "libbde_libfvalue.h"
#include "libbde_libhmac.h"
#include "libbde_recovery.h"

/* Copies an UTF-8 formatted recovery password to a binary format
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_recovery_password_copy_utf8_to_binary(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t binary_recovery_password[ 16 ],
     liberror_error_t **error )
{
	libfvalue_split_utf8_string_t *split_string = NULL;
	uint8_t *string_segment                     = NULL;
	static char *function                       = "libbde_recovery_password_copy_utf8_to_binary";
	size_t string_segment_size                  = 0;
	uint32_t value_32bit                        = 0;
	int number_of_segments                      = 0;
	int result                                  = 0;
	int segment_index                           = 0;

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
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &split_string,
		 NULL );
	}
	return( -1 );
}

/* Copies an UTF-16 formatted recovery password to a binary format
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_recovery_password_copy_utf16_to_binary(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t binary_recovery_password[ 16 ],
     liberror_error_t **error )
{
	libfvalue_split_utf16_string_t *split_string = NULL;
	uint16_t *string_segment                     = NULL;
	static char *function                        = "libbde_recovery_password_copy_utf16_to_binary";
	size_t string_segment_size                   = 0;
	uint32_t value_32bit                         = 0;
	int number_of_segments                       = 0;
	int result                                   = 0;
	int segment_index                            = 0;

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
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf16_string_free(
		 &split_string,
		 NULL );
	}
	return( -1 );
}

/* Calculates the recovery key for a certain recovery password and salt
 * Returns 1 if successful or -1 on error
 */
int libbde_recovery_calculate_key(
     const uint8_t binary_recovery_password[ 16 ],
     const uint8_t salt[ 16 ],
     uint8_t key[ 32 ],
     liberror_error_t **error )
{
	libbde_recovery_key_data_t recovery_key_data;

	static char *function = "libbde_recovery_calculate_key";

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
	if( libhmac_sha256_calculate(
	     binary_recovery_password,
	     16,
	     recovery_key_data.initial_sha256_hash,
	     LIBHMAC_SHA256_HASH_SIZE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to calculate initial SHA256.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( recovery_key_data.salt ),
	     salt,
	     16 ) == NULL )
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
	     recovery_key_data.iteration_count < 0xfffff;
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

