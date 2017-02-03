/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <system_string.h>
#include <types.h>

#include "libbde_definitions.h"
#include "libbde_encryption.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfcache.h"
#include "libbde_libfdata.h"
#include "libbde_libfguid.h"
#include "libbde_sector_data.h"
#include "libbde_unused.h"

#include "bde_volume.h"

const uint8_t bde_boot_entry_point_vista[ 3 ] = { 0xeb, 0x52, 0x90 };
const uint8_t bde_boot_entry_point_win7[ 3 ]  = { 0xeb, 0x58, 0x90 };
const uint8_t bde_identifier[ 16 ]            = { 0x3b, 0xd6, 0x67, 0x49, 0x29, 0x2e, 0xd8, 0x4a, 0x83, 0x99, 0xf6, 0xa3, 0x39, 0xe3, 0xd0, 0x01 };

const char *bde_signature                         = "-FVE-FS-";
const char *bde_ntfs_volume_file_system_signature = "NTFS    ";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_initialize(
     libbde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbde_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libbde_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libbde_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->bytes_per_sector = 512;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_free(
     libbde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbde_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libbde_io_handle_clear(
		     *io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_clear(
     libbde_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbde_io_handle_clear";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->encryption_context != NULL )
	{
		if( libbde_encryption_free(
		     &( io_handle->encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context.",
			 function );

			result = -1;
		}
	}
	if( memory_set(
	     io_handle->full_volume_encryption_key,
	     0,
	     32 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear full volume encryption key.",
		 function );

		result = -1;
	}
	if( memory_set(
	     io_handle->tweak_key,
	     0,
	     32 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear tweak key.",
		 function );

		result = -1;
	}
	io_handle->bytes_per_sector = 512;

	return( result );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t *volume_header_data      = NULL;
	static char *function            = "libbde_io_handle_read_volume_header";
	size_t read_size                 = 512;
	ssize_t read_count               = 0;
	uint64_t total_number_of_sectors = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid      = NULL;
	uint64_t value_64bit             = 0;
	uint32_t value_32bit             = 0;
	uint16_t value_16bit             = 0;
	int result                       = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek volume header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	volume_header_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * read_size );

	if( volume_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              volume_header_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 volume_header_data,
		 read_size,
		 0 );
	}
#endif
	if( memory_compare(
	     volume_header_data,
	     bde_boot_entry_point_vista,
	     3 ) == 0 )
	{
		io_handle->version = LIBBDE_VERSION_WINDOWS_VISTA;
	}
	else if( memory_compare(
	          volume_header_data,
	          bde_boot_entry_point_win7,
	          3 ) == 0 )
	{
		if( memory_compare(
		     ( (bde_volume_header_windows_7_t *) volume_header_data )->identifier,
		     bde_identifier,
		     16 ) == 0 )
		{
			io_handle->version = LIBBDE_VERSION_WINDOWS_7;
		}
		else if( memory_compare(
		          ( (bde_volume_header_to_go_t *) volume_header_data )->identifier,
		          bde_identifier,
		          16 ) == 0 )
		{
			io_handle->version = LIBBDE_VERSION_TO_GO;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported identifier.",
			 function );

			goto on_error;
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume boot entry point.",
		 function );

		goto on_error;
	}
	if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	 || ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 ) )
	{
		if( memory_compare(
		     &( volume_header_data[ 3 ] ),
		     bde_signature,
		     8 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid volume signature.",
			 function );

			goto on_error;
		}
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_volume_header_windows_vista_t *) volume_header_data )->bytes_per_sector,
	 io_handle->bytes_per_sector );

	io_handle->sectors_per_cluster_block = ( (bde_volume_header_windows_vista_t *) volume_header_data )->sectors_per_cluster_block;

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_16bit,
	 total_number_of_sectors );

	if( total_number_of_sectors == 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_32bit,
		 total_number_of_sectors );
	}
	if( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->first_metadata_cluster_block_number,
		 io_handle->first_metadata_offset );

		if( total_number_of_sectors == 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_64bit,
			 total_number_of_sectors );
		}
	}
	else if( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) volume_header_data )->first_metadata_offset,
		 io_handle->first_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) volume_header_data )->second_metadata_offset,
		 io_handle->second_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) volume_header_data )->third_metadata_offset,
		 io_handle->third_metadata_offset );
	}
	else if( io_handle->version == LIBBDE_VERSION_TO_GO )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) volume_header_data )->first_metadata_offset,
		 io_handle->first_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) volume_header_data )->second_metadata_offset,
		 io_handle->second_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) volume_header_data )->third_metadata_offset,
		 io_handle->third_metadata_offset );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot entry point:\n",
		 function );
		libcnotify_print_data(
		 volume_header_data,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 volume_header_data[ 3 ],
		 volume_header_data[ 4 ],
		 volume_header_data[ 5 ],
		 volume_header_data[ 6 ],
		 volume_header_data[ 7 ],
		 volume_header_data[ 8 ],
		 volume_header_data[ 9 ],
		 volume_header_data[ 10 ] );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per cluster block\t\t: %" PRIu8 "\n",
		 function,
		 io_handle->sectors_per_cluster_block );

		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->unknown1,
		 5,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_16bit,
		 value_16bit );
		libcnotify_printf(
		 "%s: total number of sectors (16-bit)\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: media descriptor\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->media_descriptor );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->sectors_per_track,
		 value_16bit );
		libcnotify_printf(
		 "%s: sectors per track\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->number_of_heads,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of heads\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->number_of_hidden_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of hidden sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_32bit,
		 value_32bit );
		libcnotify_printf(
		 "%s: total number of sectors (32-bit)\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->unknown4,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
			 function,
			 value_32bit,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_64bit,
			 value_64bit );
			libcnotify_printf(
			 "%s: total number of sectors (64-bit)\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->mft_cluster_block_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: MFT cluster block number\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: first metadata cluster block\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->first_metadata_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->mft_entry_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: MFT entry size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->index_entry_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: index entry size\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->volume_serial_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: volume serial number\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->bootcode,
			 426,
			 0 );
		}
		else if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
		      || ( io_handle->version == LIBBDE_VERSION_TO_GO ) )
		{
			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->unknown4,
			 31,
			 0 );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_serial_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: volume serial number\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: volume label\t\t\t: %c%c%c%c%c%c%c%c%c%c%c\n",
			 function,
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 0 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 1 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 2 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 3 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 4 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 5 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 6 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 7 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 8 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 9 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->volume_label[ 10 ] );

			libcnotify_printf(
			 "%s: file system signature\t\t: %c%c%c%c%c%c%c%c\n",
			 function,
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 0 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 1 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 2 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 3 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 4 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 5 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 6 ],
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->file_system_signature[ 7 ] );
		}
		if( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
		{
			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->bootcode,
			 47,
			 0 );

			if( libfguid_identifier_initialize(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create GUID.",
				 function );

				goto on_error;
			}
			if( libfguid_identifier_copy_from_byte_stream(
			     guid,
			     ( (bde_volume_header_windows_7_t *) volume_header_data )->identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to GUID.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfguid_identifier_copy_to_utf16_string(
				  guid,
				  (uint16_t *) guid_string,
				  48,
				  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				  error );
#else
			result = libfguid_identifier_copy_to_utf8_string(
				  guid,
				  (uint8_t *) guid_string,
				  48,
				  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy GUID to string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: identifier\t\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 guid_string );

			if( libfguid_identifier_free(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free GUID.",
				 function );

				goto on_error;
			}
		}
		else if( io_handle->version == LIBBDE_VERSION_TO_GO )
		{
			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_to_go_t *) volume_header_data )->bootcode,
			 335,
			 0 );

			if( libfguid_identifier_initialize(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create GUID.",
				 function );

				goto on_error;
			}
			if( libfguid_identifier_copy_from_byte_stream(
			     guid,
			     ( (bde_volume_header_to_go_t *) volume_header_data )->identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to GUID.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfguid_identifier_copy_to_utf16_string(
				  guid,
				  (uint16_t *) guid_string,
				  48,
				  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				  error );
#else
			result = libfguid_identifier_copy_to_utf8_string(
				  guid,
				  (uint8_t *) guid_string,
				  48,
				  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy GUID to string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: identifier\t\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 guid_string );

			if( libfguid_identifier_free(
			     &guid,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free GUID.",
				 function );

				goto on_error;
			}
		}
		if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
		 || ( io_handle->version == LIBBDE_VERSION_TO_GO ) )
		{
			libcnotify_printf(
			 "%s: first metadata offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->first_metadata_offset );

			libcnotify_printf(
			 "%s: second metadata offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->second_metadata_offset );

			libcnotify_printf(
			 "%s: third metadata offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->third_metadata_offset );
		}
		if( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) volume_header_data )->unknown5,
			 310,
			 0 );
		}
		else if( io_handle->version == LIBBDE_VERSION_TO_GO )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_to_go_t *) volume_header_data )->unknown5,
			 46,
			 0 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) volume_header_data )->sector_signature,
		 value_16bit );
		libcnotify_printf(
		 "%s: sector signature\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( total_number_of_sectors != 0 )
	{
		io_handle->volume_size  = total_number_of_sectors;
		io_handle->volume_size *= io_handle->bytes_per_sector;
	}
	if( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	{
		io_handle->first_metadata_offset *= io_handle->sectors_per_cluster_block;
		io_handle->first_metadata_offset *= io_handle->bytes_per_sector;

		io_handle->metadata_size = 16384;
	}
	else if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
	      || ( io_handle->version == LIBBDE_VERSION_TO_GO ) )
	{
		io_handle->metadata_size = 65536;
	}
	memory_free(
	 volume_header_data );

	volume_header_data = NULL;

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	if( volume_header_data != NULL )
	{
		memory_free(
		 volume_header_data );
	}
	return( -1 );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_sector(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index LIBBDE_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBBDE_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags LIBBDE_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBBDE_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libbde_sector_data_t *sector_data = NULL;
	static char *function             = "libbde_io_handle_read_sector";

	LIBBDE_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBBDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBBDE_UNREFERENCED_PARAMETER( element_data_flags );
	LIBBDE_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO handle virtual sectors, what about different sector sizes? */
	if( libbde_sector_data_initialize(
	     &sector_data,
	     (size_t) io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( libbde_sector_data_read(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     element_data_offset,
	     io_handle->encryption_context,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) sector_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libbde_sector_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data as element value.",
		 function );

		return( -1 );
	}
	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

/* Reads the unencrypted volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_unencrypted_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libbde_sector_data_t *sector_data = NULL;
	uint8_t *volume_header_data       = NULL;
	static char *function             = "libbde_io_handle_read_unencrypted_volume_header";
	off64_t volume_header_offset      = 0;
	uint64_t total_number_of_sectors  = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		goto on_error;
	}
	if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
	 || ( io_handle->version == LIBBDE_VERSION_TO_GO ) )
	{
		volume_header_offset = io_handle->volume_header_offset;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading unencrypted volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 volume_header_offset,
		 volume_header_offset );
	}
#endif
	if( libbde_sector_data_initialize(
	     &sector_data,
	     io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( libbde_sector_data_read(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     volume_header_offset,
	     io_handle->encryption_context,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header sector data.",
		 function );

		goto on_error;
	}
	volume_header_data = sector_data->data;

	if( io_handle->volume_size == 0 )
	{
		if( memory_compare(
		     &( volume_header_data[ 3 ] ),
		     bde_ntfs_volume_file_system_signature,
		     8 ) == 0 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_16bit,
			 total_number_of_sectors );

			if( total_number_of_sectors == 0 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_32bit,
				 total_number_of_sectors );
			}
			if( total_number_of_sectors == 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 ( (bde_volume_header_windows_vista_t *) volume_header_data )->total_number_of_sectors_64bit,
				 total_number_of_sectors );
			}
			if( total_number_of_sectors == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing total number of sectors.",
				 function );

				goto on_error;
			}
			io_handle->volume_size  = total_number_of_sectors;
			io_handle->volume_size *= io_handle->bytes_per_sector;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine volume size.",
			 function );

			goto on_error;
		}
	}
	if( libbde_sector_data_free(
	     &sector_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sector data.",
		 function );

		goto on_error;
	}
	sector_data = NULL;

	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

