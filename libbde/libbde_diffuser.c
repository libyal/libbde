/*
 * Elephant diffuser encryption functions
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

#include "libbde_diffuser.h"

/* De- or encrypts a block of data using diffuser A
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_a_crypt(
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	uint32_t *values_32bit        = NULL;
	static char *function         = "libbde_diffuser_a_crypt";
	size_t data_index             = 0;
	size_t number_of_iterations   = 0;
	size_t number_of_values_32bit = 0;
	size_t value_32bit_index1     = 0;
	size_t value_32bit_index2     = 0;
	size_t value_32bit_index3     = 0;

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
	if( ( input_data_size % 4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported input data size - not a multitude of 4.",
		 function );

		return( -1 );
	}
	number_of_values_32bit = input_data_size / 4;

	values_32bit = (uint32_t *) output_data;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( input_data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	number_of_iterations = 5;

	while( number_of_iterations > 0 )
	{
		value_32bit_index1 = 0;

/* TODO test first then refactor out most of the modulus calculus */
		while( value_32bit_index1 < ( number_of_values_32bit - 1 ) )
		{
			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ byte_stream_bit_rotate_left_32bit(
			                                      output_data[ value_32bit_index3 ],
			                                      9 );

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ output_data[ value_32bit_index3 ];

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ byte_stream_bit_rotate_left_32bit(
			                                      output_data[ value_32bit_index3 ],
			                                      13 );

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ output_data[ value_32bit_index3 ];

			value_32bit_index1++;
		}
		number_of_iterations--;
	}
	return( 1 );
}

/* De- or encrypts a block of data using diffuser B
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_b_crypt(
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	uint32_t *values_32bit        = NULL;
	static char *function         = "libbde_diffuser_b_crypt";
	size_t data_index             = 0;
	size_t number_of_iterations   = 0;
	size_t number_of_values_32bit = 0;
	size_t value_32bit_index1     = 0;
	size_t value_32bit_index2     = 0;
	size_t value_32bit_index3     = 0;


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
	if( ( input_data_size % 4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported input data size - not a multitude of 4.",
		 function );

		return( -1 );
	}
	number_of_values_32bit = input_data_size / 4;

	values_32bit = (uint32_t *) output_data;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( input_data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	number_of_iterations = 5;

	while( number_of_iterations > 0 )
	{
		value_32bit_index1 = 0;

/* TODO test first then refactor out most of the modulus calculus */
		while( value_32bit_index1 < ( number_of_values_32bit - 1 ) )
		{
			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ output_data[ value_32bit_index3 ];

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ byte_stream_bit_rotate_left_32bit(
			                                      output_data[ value_32bit_index3 ],
			                                      10 );

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ output_data[ value_32bit_index3 ];

			value_32bit_index1++;

			value_32bit_index2 = ( value_32bit_index1 - 2 ) % number_of_values_32bit;
			value_32bit_index3 = ( value_32bit_index1 - 5 ) % number_of_values_32bit;

			output_data[ value_32bit_index1 ] += output_data[ value_32bit_index2 ]
			                                   ^ byte_stream_bit_rotate_left_32bit(
			                                      output_data[ value_32bit_index3 ],
			                                      25 );

			value_32bit_index1++;
		}
		number_of_iterations--;
	}
	return( 1 );
}

