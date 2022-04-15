/*
 * Metadata block header functions
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

#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_metadata_block_header.h"

#include "bde_metadata.h"

/* Creates a metadata block header
 * Make sure the value metadata block header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_block_header_initialize(
     libbde_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_block_header_initialize";

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
		 function );

		return( -1 );
	}
	if( *metadata_block_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata block header value already set.",
		 function );

		return( -1 );
	}
	*metadata_block_header = memory_allocate_structure(
	                          libbde_metadata_block_header_t );

	if( *metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_block_header,
	     0,
	     sizeof( libbde_metadata_block_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_block_header != NULL )
	{
		memory_free(
		 *metadata_block_header );

		*metadata_block_header = NULL;
	}
	return( -1 );
}

/* Frees a metadata block header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_block_header_free(
     libbde_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_block_header_free";

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
		 function );

		return( -1 );
	}
	if( *metadata_block_header != NULL )
	{
		memory_free(
		 *metadata_block_header );

		*metadata_block_header = NULL;
	}
	return( 1 );
}

/* Reads a metadata block header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_block_header_read_data(
     libbde_metadata_block_header_t *metadata_block_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libbde_metadata_block_header_read_data";
	uint64_t safe_volume_header_offset = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
	uint16_t value_16bit               = 0;
#endif

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
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
	if( ( data_size < sizeof( bde_metadata_block_header_v1_t ) )
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
		 "%s: FVE metadata block header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_metadata_block_header_v1_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     data,
	     bde_signature,
	     8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid metadata block signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_block_header_v1_t *) data )->version,
	 metadata_block_header->version );

	if( metadata_block_header->version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v1_t *) data )->mft_mirror_cluster_block,
		 metadata_block_header->mft_mirror_cluster_block_number );
	}
	else if( metadata_block_header->version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v2_t *) data )->encrypted_volume_size,
		 metadata_block_header->encrypted_volume_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v2_t *) data )->volume_header_offset,
		 safe_volume_header_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_block_header_v2_t *) data )->number_of_volume_header_sectors,
		metadata_block_header->number_of_volume_header_sectors );
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) data )->first_metadata_offset,
	 metadata_block_header->first_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) data )->second_metadata_offset,
	 metadata_block_header->second_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) data )->third_metadata_offset,
	 metadata_block_header->third_metadata_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 data[ 0 ],
		 data[ 1 ],
		 data[ 2 ],
		 data[ 3 ],
		 data[ 4 ],
		 data[ 5 ],
		 data[ 6 ],
		 data[ 7 ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) data )->size,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: format version\t\t\t: %" PRIu16 "\n",
		 function,
		 metadata_block_header->version );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( metadata_block_header->version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_metadata_block_header_v1_t *) data )->unknown3,
			 16,
			 0 );
		}
		else if( metadata_block_header->version == 2 )
		{
			libcnotify_printf(
			 "%s: encrypted volume size\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_header->encrypted_volume_size );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_metadata_block_header_v2_t *) data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: number of volume header sectors\t: %" PRIu32 "\n",
			 function,
			 metadata_block_header->number_of_volume_header_sectors );
		}
		libcnotify_printf(
		 "%s: first metadata offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 metadata_block_header->first_metadata_offset );

		libcnotify_printf(
		 "%s: second metadata offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 metadata_block_header->second_metadata_offset );

		libcnotify_printf(
		 "%s: third metadata offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 metadata_block_header->third_metadata_offset );

		if( metadata_block_header->version == 1 )
		{
			libcnotify_printf(
			 "%s: MFT mirror cluster block\t\t: 0x%08" PRIx64 "\n",
			 function,
			 metadata_block_header->mft_mirror_cluster_block_number );
		}
		else if( metadata_block_header->version == 2 )
		{
			libcnotify_printf(
			 "%s: volume header offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_volume_header_offset );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	 metadata_block_header->volume_header_offset = (off64_t) safe_volume_header_offset;

	if( ( metadata_block_header->version != 1 )
	 && ( metadata_block_header->version != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a metadata block header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_block_header_read_file_io_handle(
     libbde_metadata_block_header_t *metadata_block_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t metadata_block_header_data[ sizeof( bde_metadata_block_header_v1_t ) ];

	static char *function = "libbde_metadata_block_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( metadata_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block header.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              metadata_block_header_data,
	              sizeof( bde_metadata_block_header_v1_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( bde_metadata_block_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata block header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libbde_metadata_block_header_read_data(
	     metadata_block_header,
	     metadata_block_header_data,
	     sizeof( bde_metadata_block_header_v1_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata block header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

