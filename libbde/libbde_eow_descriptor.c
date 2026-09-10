/*
 * Encrypt-on-Write (EOW) descriptor functions
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
#include "libbde_eow_descriptor.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"

#include "bde_eow.h"

/* Creates an Encrypt-on-Write (EOW) descriptor
 * Make sure the value eow_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_descriptor_initialize(
     libbde_eow_descriptor_t **eow_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_descriptor_initialize";

	if( eow_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) descriptor.",
		 function );

		return( -1 );
	}
	if( *eow_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid Encrypt-on-Write (EOW) descriptor value already set.",
		 function );

		return( -1 );
	}
	*eow_descriptor = memory_allocate_structure(
	                   libbde_eow_descriptor_t );

	if( *eow_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create Encrypt-on-Write (EOW) descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *eow_descriptor,
	     0,
	     sizeof( libbde_eow_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear Encrypt-on-Write (EOW) descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *eow_descriptor != NULL )
	{
		memory_free(
		 *eow_descriptor );

		*eow_descriptor = NULL;
	}
	return( -1 );
}

/* Frees an Encrypt-on-Write (EOW) descriptor
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_descriptor_free(
     libbde_eow_descriptor_t **eow_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_descriptor_free";

	if( eow_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) descriptor.",
		 function );

		return( -1 );
	}
	if( *eow_descriptor != NULL )
	{
		memory_free(
		 *eow_descriptor );

		*eow_descriptor = NULL;
	}
	return( 1 );
}

/* Reads an Encrypt-on-Write (EOW) descriptor
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_descriptor_read_data(
     libbde_eow_descriptor_t *eow_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function          = "libbde_eow_descriptor_read_data";
	size_t data_offset             = 0;
	uint64_t block_map_area_offset = 0;
	uint32_t offset_index          = 0;
	uint16_t header_size           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit           = 0;
	uint32_t value_32bit           = 0;
#endif

	if( eow_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) descriptor.",
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
	if( ( data_size < sizeof( bde_eow_descriptor_t ) )
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
		 "%s: Encrypt-on-Write (EOW) descriptor:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_eow_descriptor_t ),
		 1 );
	}
#endif
	if( memory_compare(
	     data,
	     "FVE-EOW\x00",
	     8 ) != 0 )
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
	 ( (bde_eow_descriptor_t *) data )->header_size,
	 header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_descriptor_t *) data )->physical_sector_size,
	 eow_descriptor->physical_sector_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_descriptor_t *) data )->relocation_block_size,
	 eow_descriptor->relocation_block_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_descriptor_t *) data )->relocation_log_area_size,
	 eow_descriptor->relocation_log_area_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_descriptor_t *) data )->number_of_offsets,
	 eow_descriptor->number_of_offsets );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c\\x%02" PRIx8 "\n",
		 function,
		 data[ 0 ],
		 data[ 1 ],
		 data[ 2 ],
		 data[ 3 ],
		 data[ 4 ],
		 data[ 5 ],
		 data[ 6 ],
		 data[ 7 ] );

		libcnotify_printf(
		 "%s: header size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 header_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_descriptor_t *) data )->data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_descriptor_t *) data )->logical_sector_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: logical sector size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: physical sector size\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_descriptor->physical_sector_size );

		libcnotify_printf(
		 "%s: relocation block size\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_descriptor->relocation_block_size );

		libcnotify_printf(
		 "%s: relocation log area size\t\t: %" PRIu32 "\n",
		 function,
		 eow_descriptor->relocation_log_area_size );

		libcnotify_printf(
		 "%s: number of offsets\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_descriptor->number_of_offsets );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_descriptor_t *) data )->checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_eow_descriptor_t *) data )->first_eow_descriptor_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: first EOW descriptor offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_eow_descriptor_t *) data )->second_eow_descriptor_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: second EOW descriptor offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( header_size != 56 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header size.",
		 function );

		return( -1 );
	}
	if( ( eow_descriptor->number_of_offsets > 64 )
	 || ( eow_descriptor->number_of_offsets > ( ( data_size - 56 ) / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of offset size value out of bounds: %" PRIu32 ".",
		 function,
		 eow_descriptor->number_of_offsets );

		return( -1 );
	}
	data_offset = 56;

	for( offset_index = 0;
	     offset_index < eow_descriptor->number_of_offsets;
	     offset_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_offset ] ),
		 block_map_area_offset );

		data_offset += 8;

		eow_descriptor->offsets[ offset_index ] = (off64_t) block_map_area_offset;
	}
	return( 1 );
}

/* Reads a Encrypt-on-Write (EOW) descriptor
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_descriptor_read_file_io_handle(
     libbde_eow_descriptor_t *eow_descriptor,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t eow_descriptor_data[ sizeof( bde_eow_descriptor_t ) ];

	static char *function = "libbde_eow_descriptor_read_file_io_handle";
	ssize_t read_count    = 0;

	if( eow_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) descriptor.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              eow_descriptor_data,
	              sizeof( bde_eow_descriptor_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( bde_eow_descriptor_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) descriptor data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libbde_eow_descriptor_read_data(
	     eow_descriptor,
	     eow_descriptor_data,
	     sizeof( bde_eow_descriptor_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) descriptor.",
		 function );

		return( -1 );
	}
	return( 1 );
}

