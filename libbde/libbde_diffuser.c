/*
 * Elephant diffuser encryption functions
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

#include "libbde_diffuser.h"
#include "libbde_libcerror.h"

/* Decrypts the data using Diffuser-A and B
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_decrypt(
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint32_t *values_32bit   = NULL;
	static char *function    = "libbde_diffuser_decrypt";
	size_t data_index        = 0;
	size_t number_of_values  = 0;
	size_t value_32bit_index = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	 || ( ( data_size % 4 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_values = data_size / 4;

	values_32bit = (uint32_t *) memory_allocate(
	                             data_size );

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values 32-bit.",
		 function );

		goto on_error;
	}
	data_index = 0;

	for( value_32bit_index = 0;
	     value_32bit_index < number_of_values;
	     value_32bit_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index ] );

		data_index += sizeof( uint32_t );
	}
	if( libbde_diffuser_b_decrypt(
	     values_32bit,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt data using Diffuser-B.",
		 function );

		goto on_error;
	}
	if( libbde_diffuser_a_decrypt(
	     values_32bit,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt data using Diffuser-A.",
		 function );

		goto on_error;
	}
	data_index = 0;

	for( value_32bit_index = 0;
	     value_32bit_index < number_of_values;
	     value_32bit_index++ )
	{
		byte_stream_copy_from_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index ] );

		data_index += sizeof( uint32_t );
	}
	if( memory_set(
	     values_32bit,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear values 32-bit.",
		 function );

		goto on_error;
	}
	memory_free(
	 values_32bit );

	return( 1 );

on_error:
	if( values_32bit != NULL )
	{
		memory_set(
		 values_32bit,
		 0,
		 data_size );

		memory_free(
		 values_32bit );
	}
	return( -1 );
}

/* Decrypts the data using Diffuser-A
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_a_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error )
{
	static char *function       = "libbde_diffuser_a_decrypt";
	size_t number_of_iterations = 0;
	size_t value_32bit_index1   = 0;
	size_t value_32bit_index2   = 0;
	size_t value_32bit_index3   = 0;

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values 32-bit.",
		 function );

		return( -1 );
	}
	if( number_of_values < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid number of values value too small.",
		 function );

		return( -1 );
	}
	if( number_of_values > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of values exceeds maximum.",
		 function );

		return( -1 );
	}
	for( number_of_iterations = 5;
	     number_of_iterations > 0;
	     number_of_iterations-- )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = number_of_values - 2;
		value_32bit_index3 = number_of_values - 5;

		while( value_32bit_index1 < ( number_of_values - 1 ) )
		{
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       9 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values )
			{
				value_32bit_index3 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index2 >= number_of_values )
			{
				value_32bit_index2 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       13 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;
		}
	}
	return( 1 );
}

/* Decrypts the data using Diffuser-B
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_b_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error )
{
	static char *function       = "libbde_diffuser_b_decrypt";
	size_t number_of_iterations = 0;
	size_t value_32bit_index1   = 0;
	size_t value_32bit_index2   = 0;
	size_t value_32bit_index3   = 0;

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values 32-bit.",
		 function );

		return( -1 );
	}
	if( number_of_values < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid number of values value too small.",
		 function );

		return( -1 );
	}
	if( number_of_values > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of values exceeds maximum.",
		 function );

		return( -1 );
	}
	for( number_of_iterations = 3;
	     number_of_iterations > 0;
	     number_of_iterations-- )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = 2;
		value_32bit_index3 = 5;

		while( value_32bit_index1 < ( number_of_values - 1 ) )
		{
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       10 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index2 >= number_of_values )
			{
				value_32bit_index2 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values )
			{
				value_32bit_index3 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       25 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;
		}
	}
	return( 1 );
}

/* Encrypts the data using Diffuser-A and B
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_encrypt(
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint32_t *values_32bit   = NULL;
	static char *function    = "libbde_diffuser_encrypt";
	size_t data_index        = 0;
	size_t number_of_values  = 0;
	size_t value_32bit_index = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	 || ( ( data_size % 4 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_values = data_size / 4;

	values_32bit = (uint32_t *) memory_allocate(
	                             data_size );

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values 32-bit.",
		 function );

		goto on_error;
	}
	data_index = 0;

	for( value_32bit_index = 0;
	     value_32bit_index < number_of_values;
	     value_32bit_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index ] );

		data_index += sizeof( uint32_t );
	}
	if( libbde_diffuser_a_encrypt(
	     values_32bit,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
		 "%s: unable to encrypt data using Diffuser-A.",
		 function );

		goto on_error;
	}
	if( libbde_diffuser_b_encrypt(
	     values_32bit,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
		 "%s: unable to encrypt data using Diffuser-B.",
		 function );

		goto on_error;
	}
	data_index = 0;

	for( value_32bit_index = 0;
	     value_32bit_index < number_of_values;
	     value_32bit_index++ )
	{
		byte_stream_copy_from_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index ] );

		data_index += sizeof( uint32_t );
	}
	if( memory_set(
	     values_32bit,
	     0,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear values 32-bit.",
		 function );

		goto on_error;
	}
	memory_free(
	 values_32bit );

	return( 1 );

on_error:
	if( values_32bit != NULL )
	{
		memory_set(
		 values_32bit,
		 0,
		 data_size );

		memory_free(
		 values_32bit );
	}
	return( -1 );
}

/* Encrypts the data using Diffuser-A
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_a_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error )
{
	static char *function       = "libbde_diffuser_a_encrypt";
	size_t number_of_iterations = 0;
	size_t value_32bit_index1   = 0;
	size_t value_32bit_index2   = 0;
	size_t value_32bit_index3   = 0;

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values 32-bit.",
		 function );

		return( -1 );
	}
	if( number_of_values < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid number of values value too small.",
		 function );

		return( -1 );
	}
	if( number_of_values > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of values exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO reverse loop
	number_of_iterations = number_of_values * 5;

	value_32bit_index1 = 0;
	value_32bit_index2 = number_of_values - 2;
	value_32bit_index3 = number_of_values - 5;
*/

	for( number_of_iterations = 5;
	     number_of_iterations > 0;
	     number_of_iterations-- )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = number_of_values - 2;
		value_32bit_index3 = number_of_values - 5;

		while( value_32bit_index1 < ( number_of_values - 1 ) )
		{
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       9 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values )
			{
				value_32bit_index3 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index2 >= number_of_values )
			{
				value_32bit_index2 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       13 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;
		}
	}
	return( 1 );
}

/* Encrypts the data using Diffuser-B
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_b_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error )
{
	static char *function       = "libbde_diffuser_b_encrypt";
	size_t number_of_iterations = 0;
	size_t value_32bit_index1   = 0;
	size_t value_32bit_index2   = 0;
	size_t value_32bit_index3   = 0;

	if( values_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values 32-bit.",
		 function );

		return( -1 );
	}
	if( number_of_values < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid number of values value too small.",
		 function );

		return( -1 );
	}
	if( number_of_values > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of values exceeds maximum.",
		 function );

		return( -1 );
	}
/* TODO reverse loop
	number_of_iterations = number_of_values * 3;

	value_32bit_index1 = 0;
	value_32bit_index2 = 2;
	value_32bit_index3 = 5;
*/

	for( number_of_iterations = 3;
	     number_of_iterations > 0;
	     number_of_iterations-- )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = 2;
		value_32bit_index3 = 5;

		while( value_32bit_index1 < ( number_of_values - 1 ) )
		{
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       10 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index2 >= number_of_values )
			{
				value_32bit_index2 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values )
			{
				value_32bit_index3 -= number_of_values;
			}
			values_32bit[ value_32bit_index1 ] -= values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       25 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;
		}
	}
	return( 1 );
}

