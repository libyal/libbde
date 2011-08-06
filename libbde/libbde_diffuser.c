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
#include <libnotify.h>

#include "libbde_diffuser.h"

/* Applies Diffuser-A to data
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_a(
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	uint32_t *values_32bit        = NULL;
	static char *function         = "libbde_diffuser_a";
	size_t data_index             = 0;
	size_t number_of_cycles       = 0;
	size_t number_of_values_32bit = 0;
	size_t value_32bit_index1     = 0;
	size_t value_32bit_index2     = 0;
	size_t value_32bit_index3     = 0;

	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( data_size % 4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data size - not a multitude of 4.",
		 function );

		return( -1 );
	}
	number_of_values_32bit = data_size / 4;

	values_32bit = (uint32_t *) memory_allocate(
	                             data_size );

	if( values_32bit == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values 32-bit.",
		 function );

		return( -1 );
	}
	data_index = 0;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	number_of_cycles = 5;

	while( number_of_cycles > 0 )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = number_of_values_32bit - 2;
		value_32bit_index3 = number_of_values_32bit - 5;

		while( value_32bit_index1 < ( number_of_values_32bit - 1 ) )
		{
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       9 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values_32bit )
			{
				value_32bit_index3 -= number_of_values_32bit;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index2 >= number_of_values_32bit )
			{
				value_32bit_index2 -= number_of_values_32bit;
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
		number_of_cycles--;
	}
	data_index = 0;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_from_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	memory_free(
	 values_32bit );

	return( 1 );
}

/* Applies Diffuser-B to data
 * Returns 1 if successful or -1 on error
 */
int libbde_diffuser_b(
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	uint32_t *values_32bit        = NULL;
	static char *function         = "libbde_diffuser_b";
	size_t data_index             = 0;
	size_t number_of_cycles       = 0;
	size_t number_of_values_32bit = 0;
	size_t value_32bit_index1     = 0;
	size_t value_32bit_index2     = 0;
	size_t value_32bit_index3     = 0;

	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( data_size % 4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data size - not a multitude of 4.",
		 function );

		return( -1 );
	}
	number_of_values_32bit = data_size / 4;

	values_32bit = (uint32_t *) memory_allocate(
	                             data_size );

	if( values_32bit == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values 32-bit.",
		 function );

		return( -1 );
	}
	data_index = 0;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	number_of_cycles = 3;

	while( number_of_cycles > 0 )
	{
		value_32bit_index1 = 0;
		value_32bit_index2 = 2;
		value_32bit_index3 = 5;

		while( ( ( number_of_cycles == 0 )
		     &&  ( value_32bit_index1 < ( number_of_values_32bit - 1 ) ) )
		    || ( ( ( number_of_cycles != 0 )
		     &&  ( value_32bit_index1 < number_of_values_32bit ) ) ) )
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

			if( value_32bit_index2 >= number_of_values_32bit )
			{
				value_32bit_index2 -= number_of_values_32bit;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ values_32bit[ value_32bit_index3 ];

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;

			if( value_32bit_index3 >= number_of_values_32bit )
			{
				value_32bit_index3 -= number_of_values_32bit;
			}
			values_32bit[ value_32bit_index1 ] += values_32bit[ value_32bit_index2 ]
			                                    ^ byte_stream_bit_rotate_left_32bit(
			                                       values_32bit[ value_32bit_index3 ],
			                                       25 );

			value_32bit_index1++;
			value_32bit_index2++;
			value_32bit_index3++;
		}
		number_of_cycles--;
	}
	data_index = 0;

	for( value_32bit_index1 = 0;
	     value_32bit_index1 < number_of_values_32bit;
	     value_32bit_index1++ )
	{
		byte_stream_copy_from_uint32_little_endian(
		 &( data[ data_index ] ),
		 values_32bit[ value_32bit_index1 ] );

		data_index += sizeof( uint32_t );
	}
	memory_free(
	 values_32bit );

	return( 1 );
}

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

// this will apply an in place diffuser A decryption function
uint32_t Diffuser_A_Decrypt(unsigned char *input, uint32_t input_size)
{

	unsigned long temp_array[512];
	unsigned long loop_var1;
	unsigned long loop_var2;
	unsigned long loop_var3;
	unsigned long max_loop;

	unsigned long total_loop; // no . of times diffuser is applied to whole block

	//init  array with supplied data
	memcpy(temp_array,input,  input_size);
 

	max_loop = input_size / 4;

	total_loop = 5;  // the diffuser function is applied a total of 3 times

while ( total_loop) {
	// the below loop should be executed 
	loop_var1 = 0;
	loop_var2 = -2 % max_loop;
	loop_var3 = -5 % max_loop;

	loop_var2 = max_loop - 2;
	loop_var3 = max_loop - 5;

/*
	while( loop_var1 < (max_loop-1) )
*/
	while( ( ( total_loop == 0 )
	     &&  ( loop_var1 < (max_loop-1) ) )
	    || ( ( total_loop != 0 )
	     &&  ( loop_var1 < max_loop ) ) )
	{

		temp_array[loop_var1] += ( temp_array [ loop_var2 ] ^  ROTATE( (temp_array [ loop_var3 ]),9));
		loop_var1++;
		loop_var2++;
		loop_var3++;

		if( loop_var3 >= max_loop )
		{
			loop_var3 -= max_loop;
		}
		temp_array[loop_var1] += ( temp_array [ loop_var2 ] ^  temp_array [ loop_var3 ]);
		loop_var1++;
		loop_var2++;
		loop_var3++;

		if( loop_var2 >= max_loop )
		{
			loop_var2 -= max_loop;
		}
		temp_array[loop_var1] += ( temp_array [ loop_var2 ] ^  ROTATE( (temp_array [ loop_var3 ]),13));
		loop_var1++;
		loop_var2++;
		loop_var3++;

		temp_array[loop_var1] += ( temp_array [ loop_var2 ] ^  temp_array [ loop_var3 ]);
		loop_var1++;
		loop_var2++;
		loop_var3++;
	}

	total_loop-- ;
} // end total_loop



// now copy the output onto to the input
memcpy(input, temp_array, input_size);

return 0;

}
