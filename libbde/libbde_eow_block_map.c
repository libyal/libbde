/*
 * Encrypt-on-Write (EOW) block map functions
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
#include "libbde_eow_block_map.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"

#include "bde_eow.h"

/* Creates an Encrypt-on-Write (EOW) block map
 * Make sure the value eow_block_map is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_map_initialize(
     libbde_eow_block_map_t **eow_block_map,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_block_map_initialize";

	if( eow_block_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block map.",
		 function );

		return( -1 );
	}
	if( *eow_block_map != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid Encrypt-on-Write (EOW) block map value already set.",
		 function );

		return( -1 );
	}
	*eow_block_map = memory_allocate_structure(
	                  libbde_eow_block_map_t );

	if( *eow_block_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create Encrypt-on-Write (EOW) block map.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *eow_block_map,
	     0,
	     sizeof( libbde_eow_block_map_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear Encrypt-on-Write (EOW) block map.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *eow_block_map != NULL )
	{
		memory_free(
		 *eow_block_map );

		*eow_block_map = NULL;
	}
	return( -1 );
}

/* Frees an Encrypt-on-Write (EOW) block map
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_map_free(
     libbde_eow_block_map_t **eow_block_map,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_block_map_free";

	if( eow_block_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block map.",
		 function );

		return( -1 );
	}
	if( *eow_block_map != NULL )
	{
		memory_free(
		 *eow_block_map );

		*eow_block_map = NULL;
	}
	return( 1 );
}

/* Reads an Encrypt-on-Write (EOW) block map
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_map_read_data(
     libbde_eow_block_map_t *eow_block_map,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_eow_block_map_read_data";
	uint16_t header_size  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( eow_block_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block map.",
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
	if( ( data_size < sizeof( bde_eow_block_map_t ) )
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
		 "%s: Encrypt-on-Write (EOW) block map:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_eow_block_map_t ),
		 1 );
	}
#endif
	if( memory_compare(
	     data,
	     "FVE-EOWBM\x00",
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
	 ( (bde_eow_block_map_t *) data )->header_size,
	 header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_map_t *) data )->block_map_size,
	 eow_block_map->block_map_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_eow_block_map_t *) data )->volume_region_offset,
	 eow_block_map->volume_region_offset );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_eow_block_map_t *) data )->volume_region_size,
	 eow_block_map->volume_region_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_eow_block_map_t *) data )->relocation_log_area_offset,
	 eow_block_map->relocation_log_area_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_map_t *) data )->first_block_record_offset,
	 eow_block_map->first_block_record_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_map_t *) data )->second_block_record_offset,
	 eow_block_map->second_block_record_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_eow_block_map_t *) data )->block_record_size,
	 eow_block_map->block_record_size );

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

		libcnotify_printf(
		 "%s: block map size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_block_map->block_map_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_map_t *) data )->block_map_index,
		 value_32bit );
		libcnotify_printf(
		 "%s: block map index\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: volume region offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 eow_block_map->volume_region_offset );

		libcnotify_printf(
		 "%s: volume region size\t\t\t: %" PRIu64 "\n",
		 function,
		 eow_block_map->volume_region_size );

		libcnotify_printf(
		 "%s: relocation log area offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 eow_block_map->relocation_log_area_offset );

		libcnotify_printf(
		 "%s: first block record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 eow_block_map->first_block_record_offset );

		libcnotify_printf(
		 "%s: second block record offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 eow_block_map->second_block_record_offset );

		libcnotify_printf(
		 "%s: block record size\t\t\t: %" PRIu32 "\n",
		 function,
		 eow_block_map->block_record_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_eow_block_map_t *) data )->checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( header_size != 60 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a Encrypt-on-Write (EOW) block map
 * Returns 1 if successful or -1 on error
 */
int libbde_eow_block_map_read_file_io_handle(
     libbde_eow_block_map_t *eow_block_map,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_map_size,
     libcerror_error_t **error )
{
	uint8_t *eow_block_map_data = NULL;
	static char *function       = "libbde_eow_block_map_read_file_io_handle";
	ssize_t read_count          = 0;

	if( eow_block_map == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid Encrypt-on-Write (EOW) block map.",
		 function );

		return( -1 );
	}
	if( ( block_map_size < sizeof( bde_eow_block_map_t ) )
	 || ( block_map_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block map size value out of bounds.",
		 function );

		return( -1 );
	}
	eow_block_map_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * (size_t) block_map_size );

	if( eow_block_map_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block map data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              eow_block_map_data,
	              (size_t) block_map_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) block_map_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) block map data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	if( libbde_eow_block_map_read_data(
	     eow_block_map,
	     eow_block_map_data,
	     sizeof( bde_eow_block_map_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read Encrypt-on-Write (EOW) block map.",
		 function );

		goto on_error;
	}
	memory_free(
	 eow_block_map_data );

	return( 1 );

on_error:
	if( eow_block_map_data != NULL )
	{
		memory_free(
		 eow_block_map_data );
	}
	return( -1 );
}

