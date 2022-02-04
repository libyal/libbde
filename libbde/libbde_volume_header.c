/*
 * The BDE volume header functions
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

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfguid.h"
#include "libbde_volume_header.h"

#include "bde_volume.h"

const uint8_t bde_boot_entry_point_vista[ 3 ] = {
	0xeb, 0x52, 0x90 };

const uint8_t bde_boot_entry_point_windows7[ 3 ] = {
	0xeb, 0x58, 0x90 };

const uint8_t bde_identifier[ 16 ] = {
	0x3b, 0xd6, 0x67, 0x49, 0x29, 0x2e, 0xd8, 0x4a, 0x83, 0x99, 0xf6, 0xa3, 0x39, 0xe3, 0xd0, 0x01 };

const uint8_t bde_identifier_used_disk_space_only[ 16 ] = {
	0x3b, 0x4d, 0xa8, 0x92, 0x80, 0xdd, 0x0e, 0x4d, 0x9e, 0x4e, 0xb1, 0xe3, 0x28, 0x4e, 0xae, 0xd8 };

const char *bde_signature = "-FVE-FS-";

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_header_initialize(
     libbde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_header_initialize";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libbde_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libbde_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_header_free(
     libbde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_header_free";

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_header_read_data(
     libbde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                = "libbde_volume_header_read_data";
	uint64_t safe_first_metadata_offset  = 0;
	uint64_t safe_second_metadata_offset = 0;
	uint64_t safe_third_metadata_offset  = 0;
	uint64_t total_number_of_sectors     = 0;
	uint32_t cluster_block_size          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                 = 0;
	uint32_t value_32bit                 = 0;
	uint16_t value_16bit                 = 0;
#endif

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 512 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 512,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     data,
	     bde_boot_entry_point_vista,
	     3 ) == 0 )
	{
		volume_header->version = LIBBDE_VERSION_WINDOWS_VISTA;
	}
	else if( memory_compare(
	          data,
	          bde_boot_entry_point_windows7,
	          3 ) == 0 )
	{
		if( memory_compare(
		     ( (bde_volume_header_windows_7_t *) data )->identifier,
		     bde_identifier,
		     16 ) == 0 )
		{
			volume_header->version = LIBBDE_VERSION_WINDOWS_7;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( memory_compare(
		          ( (bde_volume_header_windows_7_t *) data )->identifier,
		          bde_identifier_used_disk_space_only,
		          16 ) == 0 )
		{
			volume_header->version = LIBBDE_VERSION_WINDOWS_7;
		}
#endif
		else if( memory_compare(
		          ( (bde_volume_header_to_go_t *) data )->identifier,
		          bde_identifier,
		          16 ) == 0 )
		{
			volume_header->version = LIBBDE_VERSION_TO_GO;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported identifier.",
			 function );

			return( -1 );
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

		return( -1 );
	}
	if( ( volume_header->version == LIBBDE_VERSION_WINDOWS_VISTA )
	 || ( volume_header->version == LIBBDE_VERSION_WINDOWS_7 ) )
	{
		if( memory_compare(
		     &( data[ 3 ] ),
		     bde_signature,
		     8 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid volume signature.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_volume_header_windows_vista_t *) data )->bytes_per_sector,
	 volume_header->bytes_per_sector );

	volume_header->sectors_per_cluster_block = ( (bde_volume_header_windows_vista_t *) data )->sectors_per_cluster_block;

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_16bit,
	 total_number_of_sectors );

	if( total_number_of_sectors == 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_32bit,
		 total_number_of_sectors );
	}
	if( volume_header->version == LIBBDE_VERSION_WINDOWS_VISTA )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->first_metadata_cluster_block_number,
		 safe_first_metadata_offset );

		if( total_number_of_sectors == 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_64bit,
			 total_number_of_sectors );
		}
	}
	else if( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) data )->first_metadata_offset,
		 safe_first_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) data )->second_metadata_offset,
		 safe_second_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_windows_7_t *) data )->third_metadata_offset,
		 safe_third_metadata_offset );
	}
	else if( volume_header->version == LIBBDE_VERSION_TO_GO )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) data )->first_metadata_offset,
		 safe_first_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) data )->second_metadata_offset,
		 safe_second_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_to_go_t *) data )->third_metadata_offset,
		 safe_third_metadata_offset );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot entry point:\n",
		 function );
		libcnotify_print_data(
		 data,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 data[ 3 ],
		 data[ 4 ],
		 data[ 5 ],
		 data[ 6 ],
		 data[ 7 ],
		 data[ 8 ],
		 data[ 9 ],
		 data[ 10 ] );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
		 function,
		 volume_header->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per cluster block\t\t: %" PRIu8 "\n",
		 function,
		 volume_header->sectors_per_cluster_block );

		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 ( (bde_volume_header_windows_vista_t *) data )->unknown1,
		 5,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_16bit,
		 value_16bit );
		libcnotify_printf(
		 "%s: total number of sectors (16-bit)\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: media descriptor\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (bde_volume_header_windows_vista_t *) data )->media_descriptor );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->sectors_per_track,
		 value_16bit );
		libcnotify_printf(
		 "%s: sectors per track\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->number_of_heads,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of heads\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->number_of_hidden_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of hidden sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_32bit,
		 value_32bit );
		libcnotify_printf(
		 "%s: total number of sectors (32-bit)\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( volume_header->version == LIBBDE_VERSION_WINDOWS_VISTA )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->unknown4,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
			 function,
			 value_32bit,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->total_number_of_sectors_64bit,
			 value_64bit );
			libcnotify_printf(
			 "%s: total number of sectors (64-bit)\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->mft_cluster_block_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: MFT cluster block number\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: first metadata cluster block\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_first_metadata_offset );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->mft_entry_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: MFT entry size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->index_entry_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: index entry size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->volume_serial_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: volume serial number\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_vista_t *) data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_vista_t *) data )->bootcode,
			 426,
			 0 );
		}
		else if( ( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
		      || ( volume_header->version == LIBBDE_VERSION_TO_GO ) )
		{
			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) data )->unknown4,
			 31,
			 0 );

			byte_stream_copy_to_uint32_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 0 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4a\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
			 function,
			 value_32bit,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 4 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown4b\t\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 value_64bit,
			 value_64bit );

			byte_stream_copy_to_uint16_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 12 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown4c\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 value_16bit,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 14 ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown4d\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 value_16bit,
			 value_16bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown4e\t\t\t\t: 0x%08" PRIx64 " (%" PRIu64 ")\n",
			 function,
			 value_64bit,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 24 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4f\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
			 function,
			 value_32bit,
			 value_32bit );

			libcnotify_printf(
			 "%s: drive physical number\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 28 ] );

			libcnotify_printf(
			 "%s: unknown4g\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 29 ] );

			libcnotify_printf(
			 "%s: extended boot signature\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( ( (bde_volume_header_windows_7_t *) data )->unknown4 )[ 30 ] );

			libcnotify_printf(
			 "\n" );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_volume_header_windows_7_t *) data )->volume_serial_number,
			 value_32bit );
			libcnotify_printf(
			 "%s: volume serial number\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: volume label\t\t\t\t: %c%c%c%c%c%c%c%c%c%c%c\n",
			 function,
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 0 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 1 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 2 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 3 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 4 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 5 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 6 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 7 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 8 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 9 ],
			 ( (bde_volume_header_windows_7_t *) data )->volume_label[ 10 ] );

			libcnotify_printf(
			 "%s: file system signature\t\t\t: %c%c%c%c%c%c%c%c\n",
			 function,
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 0 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 1 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 2 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 3 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 4 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 5 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 6 ],
			 ( (bde_volume_header_windows_7_t *) data )->file_system_signature[ 7 ] );
		}
		if( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
		{
			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) data )->bootcode,
			 47,
			 0 );

			if( libbde_debug_print_guid_value(
			     function,
			     "identifier\t\t\t\t",
			     ( (bde_volume_header_windows_7_t *) data )->identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
		else if( volume_header->version == LIBBDE_VERSION_TO_GO )
		{
			libcnotify_printf(
			 "%s: bootcode\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_to_go_t *) data )->bootcode,
			 335,
			 0 );

			if( libbde_debug_print_guid_value(
			     function,
			     "identifier\t\t\t\t",
			     ( (bde_volume_header_to_go_t *) data )->identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
		if( ( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
		 || ( volume_header->version == LIBBDE_VERSION_TO_GO ) )
		{
			libcnotify_printf(
			 "%s: first metadata offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_first_metadata_offset );

			libcnotify_printf(
			 "%s: second metadata offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_second_metadata_offset );

			libcnotify_printf(
			 "%s: third metadata offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_third_metadata_offset );
		}
		if( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_windows_7_t *) data )->unknown5,
			 310,
			 0 );
		}
		else if( volume_header->version == LIBBDE_VERSION_TO_GO )
		{
			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_volume_header_to_go_t *) data )->unknown5,
			 46,
			 0 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_volume_header_windows_vista_t *) data )->sector_signature,
		 value_16bit );
		libcnotify_printf(
		 "%s: sector signature\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( volume_header->bytes_per_sector != 512 )
	 && ( volume_header->bytes_per_sector != 1024 )
	 && ( volume_header->bytes_per_sector != 2048 )
	 && ( volume_header->bytes_per_sector != 4096 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	if( total_number_of_sectors != 0 )
	{
		if( total_number_of_sectors > ( (uint64_t) INT64_MAX / volume_header->bytes_per_sector ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid volume size value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->volume_size = total_number_of_sectors * volume_header->bytes_per_sector;
	}
	if( volume_header->version == LIBBDE_VERSION_WINDOWS_VISTA )
	{
		if( volume_header->sectors_per_cluster_block == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid sectors per cluster block value out of bounds.",
			 function );

			return( -1 );
		}
		cluster_block_size = (uint32_t) volume_header->sectors_per_cluster_block * volume_header->bytes_per_sector;

		if( safe_first_metadata_offset > ( (uint64_t) INT64_MAX / cluster_block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid first metadata offset value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->first_metadata_offset = (off64_t) ( safe_first_metadata_offset * cluster_block_size );
		volume_header->metadata_size         = 16384;
	}
	else if( ( volume_header->version == LIBBDE_VERSION_WINDOWS_7 )
	      || ( volume_header->version == LIBBDE_VERSION_TO_GO ) )
	{
		if( safe_first_metadata_offset > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid first metadata offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( safe_second_metadata_offset > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid second metadata offset value out of bounds.",
			 function );

			return( -1 );
		}
		if( safe_third_metadata_offset > (uint64_t) INT64_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid third metadata offset value out of bounds.",
			 function );

			return( -1 );
		}
		volume_header->first_metadata_offset  = (off64_t) safe_first_metadata_offset;
		volume_header->second_metadata_offset = (off64_t) safe_second_metadata_offset;
		volume_header->third_metadata_offset  = (off64_t) safe_third_metadata_offset;
		volume_header->metadata_size          = 65536;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 volume_header->volume_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_header_read_file_io_handle(
     libbde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ 512 ];

	static char *function = "libbde_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
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
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &volume_header_data,
	              512,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libbde_volume_header_read_data(
	     volume_header,
	     (uint8_t *) &volume_header_data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

