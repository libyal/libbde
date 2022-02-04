/*
 * The NTFS volume header functions
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

#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_ntfs_volume_header.h"

#include "bde_volume.h"

/* Creates a NTFS volume header
 * Make sure the value ntfs_volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_ntfs_volume_header_initialize(
     libbde_ntfs_volume_header_t **ntfs_volume_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_ntfs_volume_header_initialize";

	if( ntfs_volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid NTFS volume header.",
		 function );

		return( -1 );
	}
	if( *ntfs_volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid NTFS volume header value already set.",
		 function );

		return( -1 );
	}
	*ntfs_volume_header = memory_allocate_structure(
	                  libbde_ntfs_volume_header_t );

	if( *ntfs_volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create NTFS volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *ntfs_volume_header,
	     0,
	     sizeof( libbde_ntfs_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear NTFS volume header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *ntfs_volume_header != NULL )
	{
		memory_free(
		 *ntfs_volume_header );

		*ntfs_volume_header = NULL;
	}
	return( -1 );
}

/* Frees a NTFS volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_ntfs_volume_header_free(
     libbde_ntfs_volume_header_t **ntfs_volume_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_ntfs_volume_header_free";

	if( ntfs_volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid NTFS volume header.",
		 function );

		return( -1 );
	}
	if( *ntfs_volume_header != NULL )
	{
		memory_free(
		 *ntfs_volume_header );

		*ntfs_volume_header = NULL;
	}
	return( 1 );
}

/* Reads the NTFS volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_ntfs_volume_header_read_data(
     libbde_ntfs_volume_header_t *ntfs_volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                    = "libbde_ntfs_volume_header_read_data";
	uint64_t mft_cluster_block_number        = 0;
	uint64_t mirror_mft_cluster_block_number = 0;
	uint32_t cluster_block_size              = 0;
	uint32_t index_entry_size                = 0;
	uint32_t mft_entry_size                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                     = 0;
	uint32_t value_32bit                     = 0;
	uint16_t value_16bit                     = 0;
#endif

	if( ntfs_volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid NTFS volume header.",
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
	if( ( data_size < sizeof( bde_ntfs_volume_header_t ) )
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
		 "%s: NTFS volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_ntfs_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (bde_ntfs_volume_header_t *) data )->file_system_signature,
	     "NTFS    ",
	     8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid volume system signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->bytes_per_sector,
	 ntfs_volume_header->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->total_number_of_sectors,
	 ntfs_volume_header->volume_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->mft_cluster_block_number,
	 mft_cluster_block_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->mirror_mft_cluster_block_number,
	 mirror_mft_cluster_block_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->mft_entry_size,
	 mft_entry_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_ntfs_volume_header_t *) data )->index_entry_size,
	 index_entry_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot entry point\t\t\t: 0x%02x 0x%02x 0x%02x\n",
		 function,
		 ( (bde_ntfs_volume_header_t *) data )->boot_entry_point[ 0 ],
		 ( (bde_ntfs_volume_header_t *) data )->boot_entry_point[ 1 ],
		 ( (bde_ntfs_volume_header_t *) data )->boot_entry_point[ 2 ] );

		libcnotify_printf(
		 "%s: file system signature\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 0 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 1 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 2 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 3 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 4 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 5 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 6 ],
		 ( (bde_ntfs_volume_header_t *) data )->file_system_signature[ 7 ] );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
		 function,
		 ntfs_volume_header->bytes_per_sector );

		libcnotify_printf(
		 "%s: sectors per cluster block\t\t: %" PRIu8 "\n",
		 function,
		 ( (bde_ntfs_volume_header_t *) data )->sectors_per_cluster_block );

		libcnotify_printf(
		 "%s: unknown1\n",
		 function );
		libcnotify_print_data(
		 ( (bde_ntfs_volume_header_t *) data )->unknown1,
		 7,
		 0 );

		libcnotify_printf(
		 "%s: media descriptor\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (bde_ntfs_volume_header_t *) data )->media_descriptor );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->sectors_per_track,
		 value_16bit );
		libcnotify_printf(
		 "%s: sectors per track\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->number_of_heads,
		 value_16bit );
		libcnotify_printf(
		 "%s: number of heads\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->number_of_hidden_sectors,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of hidden sectors\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->unknown4,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "%s: total number of sectors\t\t: %" PRIu64 "\n",
		 function,
		 ntfs_volume_header->volume_size );

		libcnotify_printf(
		 "%s: MFT cluster block number\t\t: %" PRIu64 "\n",
		 function,
		 mft_cluster_block_number );

		libcnotify_printf(
		 "%s: mirror MFT cluster block number\t: %" PRIu64 "\n",
		 function,
		 mirror_mft_cluster_block_number );

		libcnotify_printf(
		 "%s: MFT entry size\t\t\t: %" PRIu32 "\n",
		 function,
		 mft_entry_size );

		libcnotify_printf(
		 "%s: index entry size\t\t\t: %" PRIu32 "\n",
		 function,
		 index_entry_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->volume_serial_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: volume serial number\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: bootcode\n",
		 function );
		libcnotify_print_data(
		 ( (bde_ntfs_volume_header_t *) data )->bootcode,
		 426,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_ntfs_volume_header_t *) data )->sector_signature,
		 value_16bit );
		libcnotify_printf(
		 "%s: sector signature\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( ntfs_volume_header->bytes_per_sector != 256 )
	 && ( ntfs_volume_header->bytes_per_sector != 512 )
	 && ( ntfs_volume_header->bytes_per_sector != 1024 )
	 && ( ntfs_volume_header->bytes_per_sector != 2048 )
	 && ( ntfs_volume_header->bytes_per_sector != 4096 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bytes per sector: %" PRIu16 ".",
		 function,
		 ntfs_volume_header->bytes_per_sector );

		return( -1 );
	}
	cluster_block_size = ( (bde_ntfs_volume_header_t *) data )->sectors_per_cluster_block;

	if( cluster_block_size > 128 )
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		cluster_block_size = 256 - cluster_block_size;

		if( cluster_block_size > 12 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			return( -1 );
		}
		cluster_block_size = 1 << cluster_block_size;
	}
	cluster_block_size *= ntfs_volume_header->bytes_per_sector;

	if( ( cluster_block_size != 256 )
	 && ( cluster_block_size != 512 )
	 && ( cluster_block_size != 1024 )
	 && ( cluster_block_size != 2048 )
	 && ( cluster_block_size != 4096 )
	 && ( cluster_block_size != 8192 )
	 && ( cluster_block_size != 16384 )
	 && ( cluster_block_size != 32768 )
	 && ( cluster_block_size != 65536 )
	 && ( cluster_block_size != 131072 )
	 && ( cluster_block_size != 262144 )
	 && ( cluster_block_size != 524288 )
	 && ( cluster_block_size != 1048576 )
	 && ( cluster_block_size != 2097152 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported cluster block size: %" PRIu32 ".",
		 function,
		 cluster_block_size );

		return( -1 );
	}
	if( ( mft_entry_size == 0 )
	 || ( mft_entry_size > 255 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported MFT entry size: %" PRIu32 ".",
		 function,
		 mft_entry_size );

		return( -1 );
	}
	if( mft_entry_size < 128 )
	{
		if( mft_entry_size >= (size32_t) ( ( UINT32_MAX / cluster_block_size ) + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT entry size value out of bounds.",
			 function );

			return( -1 );
		}
		mft_entry_size *= cluster_block_size;
	}
	else
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		mft_entry_size = 256 - mft_entry_size;

		if( mft_entry_size >= 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid MFT entry size value out of bounds.",
			 function );

			return( -1 );
		}
		mft_entry_size = (uint32_t) 1UL << mft_entry_size;
	}
	if( ( (size_t) mft_entry_size < 42 )
	 || ( mft_entry_size >= (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry size: %" PRIu32 " value out of bounds.",
		 function,
		 mft_entry_size );

		return( -1 );
	}
	if( ( index_entry_size == 0 )
	 || ( index_entry_size > 255 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported index entry size: %" PRIu32 ".",
		 function,
		 index_entry_size );

		return( -1 );
	}
	if( index_entry_size < 128 )
	{
		if( index_entry_size >= (size32_t) ( ( UINT32_MAX / cluster_block_size ) + 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index entry size value out of bounds.",
			 function );

			return( -1 );
		}
		index_entry_size *= cluster_block_size;
	}
	else
	{
		/* The size is calculated as: 2 ^ ( 256 - value )
		 */
		index_entry_size = 256 - index_entry_size;

		if( index_entry_size >= 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid index entry size value out of bounds.",
			 function );

			return( -1 );
		}
		index_entry_size = (uint32_t) 1UL << index_entry_size;
	}
	if( (size_t) index_entry_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid MFT entry size: %" PRIu32 " value out of bounds.",
		 function,
		 index_entry_size );

		return( -1 );
	}
	if( ntfs_volume_header->volume_size > (size64_t) ( ( UINT64_MAX / ntfs_volume_header->bytes_per_sector ) + 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume size value out of bounds.",
		 function );

		return( -1 );
	}
	ntfs_volume_header->volume_size *= ntfs_volume_header->bytes_per_sector;
	ntfs_volume_header->volume_size += ntfs_volume_header->bytes_per_sector;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: calculated cluster block size\t: %" PRIu32 "\n",
		 function,
		 cluster_block_size );

		libcnotify_printf(
		 "%s: calculated MFT entry size\t\t: %" PRIu32 "\n",
		 function,
		 mft_entry_size );

		libcnotify_printf(
		 "%s: calculated index entry size\t: %" PRIu32 "\n",
		 function,
		 index_entry_size );

		libcnotify_printf(
		 "%s: calculated volume size\t\t: %" PRIu64 "\n",
		 function,
		 ntfs_volume_header->volume_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

