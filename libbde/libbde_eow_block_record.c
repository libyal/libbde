/*
 * Encrypt-on-Write (EOW) block record functions
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "libbde_debug.h"
#include "libbde_eow_block_record.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_sector_range.h"

#include "bde_eow.h"

/* Creates an Encrypt-on-Write (EOW) block record
 * Make sure the value eow_block_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_record_initialize(
     libbde_eow_block_record_t **eow_block_record,
     uint32_t bytes_per_bit,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_block_record_initialize";

	if( eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block record.",
		 function );

		return( -1 );
	}
	if( *eow_block_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid Encrypt-on-Write (EOW) block record value already set.",
		 function );

		return( -1 );
	}
	*eow_block_record = memory_allocate_structure(
	                     libbde_eow_block_record_t );

	if( *eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create Encrypt-on-Write (EOW) block record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *eow_block_record,
	     0,
	     sizeof( libbde_eow_block_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear Encrypt-on-Write (EOW) block record.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *eow_block_record )->ranges_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create ranges array.",
		 function );

		goto on_error;
	}
	( *eow_block_record )->bytes_per_bit = bytes_per_bit;

	return( 1 );

on_error:
	if( *eow_block_record != NULL )
	{
		memory_free(
		 *eow_block_record );

		*eow_block_record = NULL;
	}
	return( -1 );
}

/* Frees an Encrypt-on-Write (EOW) block record
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_record_free(
     libbde_eow_block_record_t **eow_block_record,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_block_record_free";
	int result            = 1;

	if( eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block record.",
		 function );

		return( -1 );
	}
	if( *eow_block_record != NULL )
	{
		if( libcdata_array_free(
		     &( ( *eow_block_record )->ranges_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libbde_sector_range_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free ranges array.",
			 function );

			result = -1;
		}
		memory_free(
		 *eow_block_record );

		*eow_block_record = NULL;
	}
	return( result );
}

/* Reads the bitmap
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_record_read_bitmap(
     libbde_eow_block_record_t *eow_block_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libbde_sector_range_t *sector_range = NULL;
	static char *function               = "libbde_eow_block_record_read_bitmap";
	size64_t range_size                 = 0;
	size_t data_offset                  = 0;
	off64_t range_offset                = 0;
	uint8_t bit_index                   = 0;
	uint8_t byte_value                  = 0;
	uint8_t element_value               = 0;
	uint8_t first_element_value         = 0;
	int element_index                   = 0;
	int entry_index                     = 0;
	int first_element_index             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	char *range_type                    = NULL;
#endif

	if( eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block record.",
		 function );

		return( -1 );
	}
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
	if( ( data_size < 1 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bitmap data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	first_element_value = data[ data_offset ] & 0x01;

	while( data_offset < data_size )
	{
		byte_value = data[ data_offset ];

		data_offset++;

		for( bit_index = 0;
		     bit_index < 8;
		     bit_index++ )
		{
			element_value = byte_value & 0x01;
			byte_value  >>= 1;

			if( element_value != first_element_value )
			{
				range_offset = (off64_t) first_element_index * eow_block_record->bytes_per_bit;
				range_size   = ( (size64_t) element_index - first_element_index ) * eow_block_record->bytes_per_bit;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( first_element_value == 0 )
					{
						range_type = "unencrypted";
					}
					else
					{
						range_type = "encrypted";
					}
					libcnotify_printf(
					 "%s: %s sector range\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ")\n",
					 function,
					 range_type,
					 range_offset,
					 range_offset + range_size,
					 range_size );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				if( libbde_sector_range_initialize(
				     &sector_range,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create sector range.",
					 function );

					goto on_error;
				}
				sector_range->start_offset = range_offset;
				sector_range->end_offset   = range_offset + range_size;
				sector_range->value        = first_element_value;

				if( libcdata_array_append_entry(
				     eow_block_record->ranges_array,
				     &entry_index,
				     (intptr_t *) sector_range,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append sector range to array.",
					 function );

					goto on_error;
				}
				sector_range = NULL;

				first_element_value = element_value;
				first_element_index = element_index;
			}
			element_index++;
		}
	}
	range_offset = (off64_t) first_element_index * eow_block_record->bytes_per_bit;
	range_size   = ( (size64_t) element_index - first_element_index ) * eow_block_record->bytes_per_bit;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( first_element_value == 0 )
		{
			range_type = "unencrypted";
		}
		else
		{
			range_type = "encrypted";
		}
		libcnotify_printf(
		 "%s: %s sector range\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%" PRIu64 ")\n",
		 function,
		 range_type,
		 range_offset,
		 range_offset + range_size,
		 range_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libbde_sector_range_initialize(
	     &sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector range.",
		 function );

		goto on_error;
	}
	sector_range->start_offset = range_offset;
	sector_range->end_offset   = range_offset + range_size;
	sector_range->value        = first_element_value;

	if( libcdata_array_append_entry(
	     eow_block_record->ranges_array,
	     &entry_index,
	     (intptr_t *) sector_range,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append sector range to array.",
		 function );

		goto on_error;
	}
	sector_range = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( sector_range != NULL )
	{
		libbde_sector_range_free(
		 &sector_range,
		 NULL );
	}
	return( -1 );
}

/* Reads an Encrypt-on-Write (EOW) block record
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_record_read_data(
     libbde_eow_block_record_t *eow_block_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libbde_eow_block_record_read_data";
	size_t bitmap_size      = 0;
	uint32_t number_of_bits = 0;
	uint16_t header_size    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit    = 0;
#endif

	if( eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block record.",
		 function );

		return( -1 );
	}
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
	if( ( data_size < sizeof( bde_eow_block_record_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: Encrypt-on-Write (EOW) block record:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_eow_block_record_t ),
		 1 );
	}
#endif
	if( memory_compare(
	     data,
	     "FVE-EOWBR\x00",
	     10 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_eow_block_record_t *) data )->header_size,
	 header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_record_t *) data )->number_of_bits,
	 number_of_bits );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_record_t *) data )->sequence_number,
	 eow_block_record->sequence_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c%c%c\\x%02" PRIx8 "\n",
		 function,
		 data[ 0 ],
		 data[ 1 ],
		 data[ 2 ],
		 data[ 3 ],
		 data[ 4 ],
		 data[ 5 ],
		 data[ 6 ],
		 data[ 7 ],
		 data[ 8 ],
		 data[ 9 ] );

		libcnotify_printf(
		 "%s: header size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 header_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_record_t *) data )->physical_sector_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: physical sector size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of bits\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_bits );

		libcnotify_printf(
		 "%s: sequence number\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_block_record->sequence_number );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_record_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_record_t *) data )->flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_record_t *) data )->checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( header_size != 36 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header size.",
		 function );

		return( -1 );
	}
	bitmap_size = number_of_bits / 8;

	if( ( number_of_bits % 8 ) != 0 )
	{
		bitmap_size += 1;
	}
	if( bitmap_size > ( data_size - 36 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of bits value out of bounds: %" PRIu32 ".",
		 function,
		 number_of_bits );

		return( -1 );
	}
	if( libbde_eow_block_record_read_bitmap(
	     eow_block_record,
	     &( data[ 36 ] ),
	     bitmap_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bitmap.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a Encrypt-on-Write (EOW) block record
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_record_read_file_io_handle(
     libbde_eow_block_record_t *eow_block_record,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_record_size,
     libcerror_error_t **error )
{
	uint8_t *eow_block_record_data = NULL;
	static char *function          = "libbde_eow_block_record_read_file_io_handle";
	ssize_t read_count             = 0;

	if( eow_block_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block record.",
		 function );

		return( -1 );
	}
	if( ( block_record_size < sizeof( bde_eow_block_record_t ) )
	 || ( block_record_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block record size value out of bounds.",
		 function );

		return( -1 );
	}
	eow_block_record_data = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * (size_t) block_record_size );

	if( eow_block_record_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block record data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              eow_block_record_data,
	              (size_t) block_record_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) block_record_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) block record data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libbde_eow_block_record_read_data(
	     eow_block_record,
	     eow_block_record_data,
	     (size_t) block_record_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) block record.",
		 function );

		goto on_error;
	}
	memory_free(
	 eow_block_record_data );

	return( 1 );

on_error:
	if( eow_block_record_data != NULL )
	{
		memory_free(
		 eow_block_record_data );
	}
	return( -1 );
}

