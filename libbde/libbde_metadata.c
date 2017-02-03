/*
 * Metadata functions
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

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_external_key.h"
#include "libbde_io_handle.h"
#include "libbde_key.h"
#include "libbde_libbfio.h"
#include "libbde_libcaes.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_libuna.h"
#include "libbde_metadata.h"
#include "libbde_metadata_entry.h"
#include "libbde_password.h"
#include "libbde_password_keep.h"
#include "libbde_volume_master_key.h"

#include "bde_metadata.h"

/* Creates metadata
 * Make sure the value metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_initialize(
     libbde_metadata_t **metadata,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_initialize";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( *metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata value already set.",
		 function );

		return( -1 );
	}
	*metadata = memory_allocate_structure(
	             libbde_metadata_t );

	if( *metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata,
	     0,
	     sizeof( libbde_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *metadata )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *metadata )->volume_master_keys_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume master keys array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata != NULL )
	{
		if( ( *metadata )->entries_array != NULL )
		{
			libcdata_array_free(
			 &( ( *metadata )->entries_array ),
			 (int(*)(intptr_t **, libcerror_error_t **)) &libbde_metadata_entry_free,
			 NULL );
		}
		memory_free(
		 *metadata );

		*metadata = NULL;
	}
	return( -1 );
}

/* Frees a metadata
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_free(
     libbde_metadata_t **metadata,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_free";
	int result            = 1;

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( *metadata != NULL )
	{
		if( ( *metadata )->description != NULL )
		{
			memory_free(
			 ( *metadata )->description );
		}
		if( ( *metadata )->full_volume_encryption_key != NULL )
		{
			if( libbde_aes_ccm_encrypted_key_free(
			     &( ( *metadata )->full_volume_encryption_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free full volume encryption key.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *metadata )->entries_array ),
		     (int(*)(intptr_t **, libcerror_error_t **)) &libbde_metadata_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *metadata )->volume_master_keys_array ),
		     (int(*)(intptr_t **, libcerror_error_t **)) &libbde_volume_master_key_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume master keys array.",
			 function );

			result = -1;
		}
		memory_free(
		 *metadata );

		*metadata = NULL;
	}
	return( result );
}

/* Reads a metadata block at the specified offset
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_read_block(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     const uint8_t *startup_key_identifier,
     size_t startup_key_identifier_size,
     libcerror_error_t **error )
{
	uint8_t *fve_metadata_block              = NULL;
	void *reallocation                       = NULL;
	static char *function                    = "libbde_metadata_read_block";
	size_t fve_metadata_block_offset         = 0;
	size_t read_size                         = 8192;
	ssize_t read_count                       = 0;
	uint64_t first_metadata_offset           = 0;
	uint64_t second_metadata_offset          = 0;
	uint64_t third_metadata_offset           = 0;
	uint32_t metadata_size                   = 0;
	uint32_t number_of_volume_header_sectors = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                     = 0;
	uint16_t value_16bit                     = 0;
#endif

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading metadata at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek metadata offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	fve_metadata_block = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * read_size );

	if( fve_metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create FVE metadata block.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              fve_metadata_block,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata block.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: FVE metadata block header:\n",
		 function );
		libcnotify_print_data(
		 fve_metadata_block,
		 sizeof( bde_metadata_block_header_v1_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     fve_metadata_block,
	     bde_signature,
	     8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid metadata block signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->version,
	 metadata->version );

	if( ( metadata->version != 1 )
	 && ( metadata->version != 2 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata block version.",
		 function );

		goto on_error;
	}
	if( metadata->version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->mft_mirror_cluster_block,
		 metadata->mft_mirror_cluster_block_number );
	}
	else if( metadata->version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v2_t *) fve_metadata_block )->encrypted_volume_size,
		 metadata->encrypted_volume_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v2_t *) fve_metadata_block )->volume_header_offset,
		 metadata->volume_header_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_block_header_v2_t *) fve_metadata_block )->number_of_volume_header_sectors,
		 number_of_volume_header_sectors );
	}
	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->first_metadata_offset,
	 first_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->second_metadata_offset,
	 second_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->third_metadata_offset,
	 third_metadata_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 fve_metadata_block[ 0 ],
		 fve_metadata_block[ 1 ],
		 fve_metadata_block[ 2 ],
		 fve_metadata_block[ 3 ],
		 fve_metadata_block[ 4 ],
		 fve_metadata_block[ 5 ],
		 fve_metadata_block[ 6 ],
		 fve_metadata_block[ 7 ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->size,
		 value_16bit );
		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 metadata->version );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( metadata->version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libcnotify_print_data(
			 ( (bde_metadata_block_header_v1_t *) fve_metadata_block )->unknown3,
			 16,
			 0 );
		}
		else if( metadata->version == 2 )
		{
			libcnotify_printf(
			 "%s: encrypted volume size\t\t\t: %" PRIu64 "\n",
			 function,
			 metadata->encrypted_volume_size );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_metadata_block_header_v2_t *) fve_metadata_block )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: number of volume header sectors\t\t: %" PRIu32 "\n",
			 function,
			 number_of_volume_header_sectors );
		}
		libcnotify_printf(
		 "%s: first metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 first_metadata_offset );

		libcnotify_printf(
		 "%s: second metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 second_metadata_offset );

		libcnotify_printf(
		 "%s: third metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 third_metadata_offset );

		if( metadata->version == 1 )
		{
			libcnotify_printf(
			 "%s: MFT mirror cluster block\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 metadata->mft_mirror_cluster_block_number );
		}
		else if( metadata->version == 2 )
		{
			libcnotify_printf(
			 "%s: volume header offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 metadata->volume_header_offset );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	{
		if( io_handle->second_metadata_offset == 0 )
		{
			io_handle->second_metadata_offset = second_metadata_offset;
		}
		if( io_handle->third_metadata_offset == 0 )
		{
			io_handle->third_metadata_offset = third_metadata_offset;
		}
	}
	if( (uint64_t) io_handle->first_metadata_offset != first_metadata_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for first metadata offset.",
		 function );

		goto on_error;
	}
	if( (uint64_t) io_handle->second_metadata_offset != second_metadata_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for second metadata offset.",
		 function );

		goto on_error;
	}
	if( (uint64_t) io_handle->third_metadata_offset != third_metadata_offset )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for third metadata offset.",
		 function );

		goto on_error;
	}
	fve_metadata_block_offset += sizeof( bde_metadata_block_header_v1_t );
	read_size                 -= sizeof( bde_metadata_block_header_v1_t );

	read_count = libbde_metadata_read_header(
	              metadata,
	              &( fve_metadata_block[ fve_metadata_block_offset ] ),
	              read_size,
	              &metadata_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata header.",
		 function );

		goto on_error;
	}
	fve_metadata_block_offset += read_count;
	read_size                 -= read_count;

	if( metadata_size < ( sizeof( bde_metadata_header_v1_t ) + read_count ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: metadata size value out of bounds.",
		 function );

		goto on_error;
	}
	if( metadata_size > read_size )
	{
		reallocation = memory_reallocate(
		                fve_metadata_block,
		                metadata_size );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize metadata.",
			 function );

			goto on_error;
		}
		fve_metadata_block = (uint8_t *) reallocation;
	}
	read_count = libbde_metadata_read_entries(
	              metadata,
	              &( fve_metadata_block[ fve_metadata_block_offset ] ),
	              (size_t) metadata_size - sizeof( bde_metadata_header_v1_t ),
	              startup_key_identifier,
	              startup_key_identifier_size,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata header.",
		 function );

		goto on_error;
	}
	memory_free(
	 fve_metadata_block );

	fve_metadata_block = NULL;

	return( 1 );

on_error:
	if( fve_metadata_block != NULL )
	{
		memory_free(
		 fve_metadata_block );
	}
	return( -1 );
}

/* Reads a metadata header
 * Returns the number of byte read if successful or -1 on error
 */
ssize_t libbde_metadata_read_header(
         libbde_metadata_t *metadata,
         uint8_t *header_data,
         size_t header_data_size,
         uint32_t *metadata_size,
         libcerror_error_t **error )
{
	static char *function             = "libbde_metadata_read_header";
	uint32_t metadata_header_size     = 0;
	uint32_t metadata_size_copy       = 0;
	uint32_t version                  = 0;
	uint16_t encryption_method_copy   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t filetime_string[ 32 ];
	system_character_t guid_string[ 48 ];

	libfdatetime_filetime_t *filetime = NULL;
	libfguid_identifier_t *guid       = NULL;
	uint32_t value_32bit              = 0;
	int result                        = 0;
#endif

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid header data.",
		 function );

		return( -1 );
	}
	if( header_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid header data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( header_data_size < sizeof( bde_metadata_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: header data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( metadata_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: FVE metadata header:\n",
		 function );
		libcnotify_print_data(
		 header_data,
		 sizeof( bde_metadata_header_v1_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->metadata_size,
	 *metadata_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->version,
	 version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->metadata_header_size,
	 metadata_header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->metadata_size_copy,
	 metadata_size_copy );

	if( memory_copy(
	     metadata->volume_identifier,
	     ( (bde_metadata_header_v1_t *) header_data )->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->encryption_method,
	 metadata->encryption_method );

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->encryption_method_copy,
	 encryption_method_copy );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_header_v1_t *) header_data )->creation_time,
	 metadata->creation_time );

	if( version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata header version.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: metadata size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *metadata_size );

		libcnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 version );

		libcnotify_printf(
		 "%s: metadata header size\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_header_size );

		libcnotify_printf(
		 "%s: metadata size copy\t\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_size_copy );

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
		     metadata->volume_identifier,
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
		 "%s: volume identifier\t\t\t\t: %" PRIs_SYSTEM "\n",
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
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_header_v1_t *) header_data )->next_nonce_counter,
		 value_32bit );
		libcnotify_printf(
		 "%s: next nonce counter\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: encryption method\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 metadata->encryption_method,
		 libbde_debug_print_encryption_method(
		  metadata->encryption_method ) );

		libcnotify_printf(
		 "%s: encryption method copy\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 encryption_method_copy,
		 libbde_debug_print_encryption_method(
		  encryption_method_copy ) );

		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create filetime.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (bde_metadata_header_v1_t *) header_data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: creation time\t\t\t\t: %" PRIs_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free filetime.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( metadata_header_size != sizeof( bde_metadata_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata header size.",
		 function );

		goto on_error;
	}
	if( *metadata_size != metadata_size_copy )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata size and copy.",
		 function );

		goto on_error;
	}
	if( *metadata_size < sizeof( bde_metadata_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: metadata size value out of bounds.",
		 function );

		goto on_error;
	}
	return( sizeof( bde_metadata_header_v1_t ) );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads a metadata entries
 * Returns the number of byte read if successful or -1 on error
 */
ssize_t libbde_metadata_read_entries(
         libbde_metadata_t *metadata,
         uint8_t *entries_data,
         size_t entries_data_size,
         const uint8_t *startup_key_identifier,
         size_t startup_key_identifier_size,
         libcerror_error_t **error )
{
	libbde_aes_ccm_encrypted_key_t *aes_ccm_encrypted_key = NULL;
	libbde_external_key_t *external_key                   = NULL;
	libbde_metadata_entry_t *metadata_entry               = NULL;
	libbde_volume_master_key_t *volume_master_key         = NULL;
	static char *function                                 = "libbde_metadata_read_entries";
	size_t entries_data_offset                            = 0;
	ssize_t read_count                                    = 0;
	uint64_t volume_header_offset                         = 0;
	uint64_t volume_header_size                           = 0;
	int entry_index                                       = 0;

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entries data.",
		 function );

		return( -1 );
	}
	if( entries_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid entries data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( startup_key_identifier != NULL )
	{
		if( startup_key_identifier_size < 16 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid startup key identifier value too small.",
			 function );

			return( -1 );
		}
	}
	while( entries_data_size >= sizeof( bde_metadata_entry_v1_t ) )
	{
		if( memory_compare(
		     &( entries_data[ entries_data_offset ] ),
		     libbde_metadata_entry_empty,
		     sizeof( bde_metadata_entry_v1_t ) ) == 0 )
		{
			break;
		}
		if( libbde_metadata_entry_initialize(
		     &metadata_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create metadata entry.",
			 function );

			goto on_error;
		}
		read_count = libbde_metadata_entry_read(
			      metadata_entry,
			      &( entries_data[ entries_data_offset ] ),
			      entries_data_size,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read metadata entry.",
			 function );

			goto on_error;
		}
		entries_data_offset += read_count;
		entries_data_size   -= read_count;

		switch( metadata_entry->type )
		{
			case LIBBDE_ENTRY_TYPE_VOLUME_MASTER_KEY:
				if( libbde_volume_master_key_initialize(
				     &volume_master_key,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume master key.",
					 function );

					goto on_error;
				}
				if( libbde_volume_master_key_read(
				     volume_master_key,
				     metadata_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume master key.",
					 function );

					goto on_error;
				}
				if( volume_master_key->protection_type == LIBBDE_KEY_PROTECTION_TYPE_CLEAR_KEY )
				{
					if( metadata->clear_key_volume_master_key == NULL )
					{
						metadata->clear_key_volume_master_key = volume_master_key;
					}
				}
				else if( volume_master_key->protection_type == LIBBDE_KEY_PROTECTION_TYPE_STARTUP_KEY )
				{
					if( ( metadata->startup_key_volume_master_key == NULL )
					 && ( startup_key_identifier != NULL ) )
					{
						/* There can be multiple startup keys
						 * check if the identifiers matches
						 */
						if( memory_compare(
						     volume_master_key->identifier,
						     startup_key_identifier,
						     16 ) == 0 )
						{
							metadata->startup_key_volume_master_key = volume_master_key;
						}
					}
				}
				else if( volume_master_key->protection_type == LIBBDE_KEY_PROTECTION_TYPE_RECOVERY_PASSWORD )
				{
					if( metadata->recovery_password_volume_master_key == NULL )
					{
						metadata->recovery_password_volume_master_key = volume_master_key;
					}
				}
				else if( volume_master_key->protection_type == LIBBDE_KEY_PROTECTION_TYPE_PASSWORD )
				{
					if( metadata->password_volume_master_key == NULL )
					{
						metadata->password_volume_master_key = volume_master_key;
					}
				}
				if( libcdata_array_append_entry(
				     metadata->volume_master_keys_array,
				     &entry_index,
				     (intptr_t *) volume_master_key,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to append volume mastesr key to array.",
					 function );

					goto on_error;
				}
				volume_master_key = NULL;

				break;

			case LIBBDE_ENTRY_TYPE_FULL_VOLUME_ENCRYPTION_KEY:
/* TODO change to name */
			case 0x000b:
				if( libbde_aes_ccm_encrypted_key_initialize(
				     &aes_ccm_encrypted_key,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create AES-CCM encrypted key.",
					 function );

					goto on_error;
				}
				if( libbde_aes_ccm_encrypted_key_read(
				     aes_ccm_encrypted_key,
				     metadata_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read AES-CCM encrypted key from property metadata entry.",
					 function );

					goto on_error;
				}
				if( ( metadata_entry->type == LIBBDE_ENTRY_TYPE_FULL_VOLUME_ENCRYPTION_KEY )
				 && ( metadata->full_volume_encryption_key == NULL ) )
				{
					metadata->full_volume_encryption_key = aes_ccm_encrypted_key;

					aes_ccm_encrypted_key = NULL;
				}
				if( metadata_entry->type == 0x000b )
				{
/* TODO store key somewhere */
				}
				if( aes_ccm_encrypted_key != NULL )
				{
					if( libbde_aes_ccm_encrypted_key_free(
					     &aes_ccm_encrypted_key,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free AES-CCM encrypted key.",
						 function );

						goto on_error;
					}
				}
				break;

			case LIBBDE_ENTRY_TYPE_STARTUP_KEY:
				if( libbde_external_key_initialize(
				     &external_key,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create external key.",
					 function );

					goto on_error;
				}
				if( libbde_external_key_read(
				     external_key,
				     metadata_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read external key from property metadata entry.",
					 function );

					goto on_error;
				}
				if( metadata->startup_key_external_key == NULL )
				{
					metadata->startup_key_external_key = external_key;

					external_key = NULL;
				}
				if( external_key != NULL )
				{
					if( libbde_external_key_free(
					     &external_key,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free external key.",
						 function );

						goto on_error;
					}
				}
				break;

			case LIBBDE_ENTRY_TYPE_DESCRIPTION:
#if defined( HAVE_DEBUG_OUTPUT )
				if( libbde_metadata_entry_read_string(
				     metadata_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read string metadata entry.",
					 function );

					goto on_error;
				}
#endif
				if( metadata->description != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid metadata - description value already set.",
					 function );

					goto on_error;
				}
				if( metadata_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing metadata entry.",
					 function );

					goto on_error;
				}
				if( ( metadata_entry->value_data != NULL )
				 && ( metadata_entry->value_data_size > 0 ) )
				{
					metadata->description = (uint8_t *) memory_allocate(
					                                     metadata_entry->value_data_size );

					if( metadata->description == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
						 "%s: unable to create description.",
						 function );

						goto on_error;
					}
					if( memory_copy(
					     metadata->description,
					     metadata_entry->value_data,
					     metadata_entry->value_data_size ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy description metadata entry value data.",
						 function );

						goto on_error;
					}
					metadata->description_size = metadata_entry->value_data_size;
				}
				break;

			case LIBBDE_ENTRY_TYPE_VOLUME_HEADER_BLOCK:
				if( metadata_entry->value_type == LIBBDE_VALUE_TYPE_OFFSET_AND_SIZE )
				{
/* TODO move to separate function and check for the size */
					if( metadata_entry->value_data_size < 16 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: value data size value out of bounds.",
						 function );

						goto on_error;
					}
					byte_stream_copy_to_uint64_little_endian(
					 &( metadata_entry->value_data[ 0 ] ),
					 volume_header_offset );

					byte_stream_copy_to_uint64_little_endian(
					 &( metadata_entry->value_data[ 8 ] ),
					 volume_header_size );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: offset\t\t\t\t\t: 0x%" PRIx64 "\n",
						 function,
						 volume_header_offset );

						libcnotify_printf(
						 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
						 function,
						 volume_header_size );

						if( metadata_entry->value_data_size > 16 )
						{
							libcnotify_printf(
							 "%s: unknown1:\n",
							 function );
							libcnotify_print_data(
							 &( metadata_entry->value_data[ 16 ] ),
							 metadata_entry->value_data_size - 16,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "\n" );
						}
					}
#endif
					if( (off64_t) volume_header_offset != metadata->volume_header_offset )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_INPUT,
						 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
						 "%s: value mismatch for metadata volume header offset.",
						 function );

						goto on_error;
					}
					metadata->volume_header_size = (size64_t) volume_header_size;
				}
				break;

			default:
				break;
		}
		if( libcdata_array_append_entry(
		     metadata->entries_array,
		     &entry_index,
		     (intptr_t *) metadata_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to append metadata entry to array.",
			 function );

			goto on_error;
		}
		metadata_entry = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( entries_data_size > 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( entries_data[ entries_data_offset ] ),
			 entries_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	return( (ssize_t) entries_data_offset );

on_error:
	if( aes_ccm_encrypted_key != NULL )
	{
		libbde_aes_ccm_encrypted_key_free(
		 &aes_ccm_encrypted_key,
		 NULL );
	}
	if( external_key != NULL )
	{
		libbde_external_key_free(
		 &external_key,
		 NULL );
	}
	if( volume_master_key != NULL )
	{
		libbde_volume_master_key_free(
		 &volume_master_key,
		 NULL );
	}
	if( metadata->description != NULL )
	{
		memory_free(
		 metadata->description );

		metadata->description      = NULL;
		metadata->description_size = 0;
	}
	if( metadata_entry != NULL )
	{
		libbde_metadata_entry_free(
		 &metadata_entry,
		 NULL );
	}
	libcdata_array_empty(
	 metadata->entries_array,
	 (int(*)(intptr_t **, libcerror_error_t **)) &libbde_metadata_entry_free,
	 NULL );

	return( -1 );
}

/* Reads the volume master key from the metadata
 * Returns 1 if successful, 0 if no key could be obtained or -1 on error
 */
int libbde_metadata_read_volume_master_key(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     libbde_password_keep_t *password_keep,
     const uint8_t *external_key,
     size_t external_key_size,
     uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error )
{
	uint8_t aes_ccm_key[ 32 ];

	libcaes_context_t *aes_context = NULL;
	uint8_t *unencrypted_data      = NULL;
	static char *function          = "libbde_metadata_read_volume_master_key";
	size_t unencrypted_data_size   = 0;
	uint32_t data_size             = 0;
	uint32_t version               = 0;
	int result                     = 0;

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
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
	if( password_keep == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password keep.",
		 function );

		return( -1 );
	}
	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( volume_master_key_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid volume master key value too small.",
		 function );

		return( -1 );
	}
	if( metadata->clear_key_volume_master_key != NULL )
	{
		if( metadata->clear_key_volume_master_key->key == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid metadata - invalid clear key volume master key - missing key.",
			 function );

			return( -1 );
		}
		if( metadata->clear_key_volume_master_key->aes_ccm_encrypted_key == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid metadata - invalid clear key volume master key - missing AES-CCM encrypted key.",
			 function );

			return( -1 );
		}
		if( metadata->clear_key_volume_master_key->key->data_size != 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: clear key volume master key - key data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     aes_ccm_key,
		     metadata->clear_key_volume_master_key->key->data,
		     32 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy AES-CCM key.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: AES-CCM key:\n",
			 function );
			libcnotify_print_data(
			 aes_ccm_key,
			 32,
			 0 );
		}
#endif
		if( metadata->clear_key_volume_master_key->aes_ccm_encrypted_key->data_size < 28 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: clear key volume master key - AES-CCM encrypted key data size value out of bounds.",
			 function );

			goto on_error;
		}
		unencrypted_data_size = metadata->clear_key_volume_master_key->aes_ccm_encrypted_key->data_size;

		unencrypted_data = (uint8_t *) memory_allocate(
						unencrypted_data_size );

		if( unencrypted_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create unencrypted data.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     unencrypted_data,
		     0,
		     unencrypted_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear unencrypted data.",
			 function );

			goto on_error;
		}
		if( libcaes_context_initialize(
		     &aes_context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable initialize AES context.",
			 function );

			goto on_error;
		}
		if( libcaes_context_set_key(
		     aes_context,
		     LIBCAES_CRYPT_MODE_ENCRYPT,
		     aes_ccm_key,
		     256,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set encryption key in AES context.",
			 function );

			goto on_error;
		}
		if( libcaes_crypt_ccm(
		     aes_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     metadata->clear_key_volume_master_key->aes_ccm_encrypted_key->nonce,
		     12,
		     metadata->clear_key_volume_master_key->aes_ccm_encrypted_key->data,
		     metadata->clear_key_volume_master_key->aes_ccm_encrypted_key->data_size,
		     unencrypted_data,
		     unencrypted_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
			 "%s: unable to decrypt data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unencrypted data:\n",
			 function );
			libcnotify_print_data(
			 unencrypted_data,
			 unencrypted_data_size,
			 0 );
		}
#endif
/* TODO improve this check */
		byte_stream_copy_to_uint16_little_endian(
		 &( unencrypted_data[ 16 ] ),
		 data_size );

		byte_stream_copy_to_uint16_little_endian(
		 &( unencrypted_data[ 20 ] ),
		 version );

		if( version == 1 )
		{
			if( data_size == 0x2c )
			{
				if( memory_copy(
				     volume_master_key,
				     &( unencrypted_data[ 28 ] ),
				     32 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy unencrypted volume master key.",
					 function );

					goto on_error;
				}
				result = 1;
			}
		}
		if( libcaes_context_free(
		     &aes_context,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     unencrypted_data,
		     0,
		     unencrypted_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear unencrypted data.",
			 function );

			goto on_error;
		}
		memory_free(
		 unencrypted_data );

		unencrypted_data = NULL;
	}
	if( result == 0 )
	{
		if( ( external_key != NULL )
		 && ( external_key_size == 32 ) )
		{
			if( metadata->startup_key_volume_master_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - missing startup key volume master key.",
				 function );

				return( -1 );
			}
			if( metadata->startup_key_volume_master_key->aes_ccm_encrypted_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - invalid startup key volume master key - missing AES-CCM encrypted key.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     aes_ccm_key,
			     0,
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear AES-CCM key.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     aes_ccm_key,
			     external_key,
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy AES-CCM key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: AES-CCM key:\n",
				 function );
				libcnotify_print_data(
				 aes_ccm_key,
				 32,
				 0 );
			}
#endif
			if( metadata->startup_key_volume_master_key->aes_ccm_encrypted_key->data_size < 28 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: startup key volume master key - AES-CCM encrypted key data size value out of bounds.",
				 function );

				goto on_error;
			}
			unencrypted_data_size = metadata->startup_key_volume_master_key->aes_ccm_encrypted_key->data_size;

			unencrypted_data = (uint8_t *) memory_allocate(
							unencrypted_data_size );

			if( unencrypted_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create unencrypted data.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			if( libcaes_context_initialize(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable initialize AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_context_set_key(
			     aes_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     aes_ccm_key,
			     256,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set encryption key in AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_crypt_ccm(
			     aes_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     metadata->startup_key_volume_master_key->aes_ccm_encrypted_key->nonce,
			     12,
			     metadata->startup_key_volume_master_key->aes_ccm_encrypted_key->data,
			     metadata->startup_key_volume_master_key->aes_ccm_encrypted_key->data_size,
			     unencrypted_data,
			     unencrypted_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to decrypt data.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unencrypted data:\n",
				 function );
				libcnotify_print_data(
				 unencrypted_data,
				 unencrypted_data_size,
				 0 );
			}
#endif
/* TODO improve this check */
			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 16 ] ),
			 data_size );

			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 20 ] ),
			 version );

			if( version == 1 )
			{
				if( data_size == 0x2c )
				{
					if( memory_copy(
					     volume_master_key,
					     &( unencrypted_data[ 28 ] ),
					     32 ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy unencrypted volume master key.",
						 function );

						goto on_error;
					}
					result = 1;
				}
			}
			if( libcaes_context_free(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free context.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			memory_free(
			 unencrypted_data );

			unencrypted_data = NULL;
		}
	}
	if( result == 0 )
	{
		if( password_keep->password_is_set != 0 )
		{
			if( metadata->password_volume_master_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - missing password volume master key.",
				 function );

				return( -1 );
			}
			if( metadata->password_volume_master_key->stretch_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - invalid password volume master key - missing stretch key.",
				 function );

				return( -1 );
			}
			if( metadata->password_volume_master_key->aes_ccm_encrypted_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - invalid password volume master key - missing AES-CCM encrypted key.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     aes_ccm_key,
			     0,
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear AES-CCM key.",
				 function );

				goto on_error;
			}
			if( libbde_password_calculate_key(
			     password_keep->password_hash,
			     32,
			     metadata->password_volume_master_key->stretch_key->salt,
			     16,
			     aes_ccm_key,
			     32,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine AES-CCM key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: AES-CCM key:\n",
				 function );
				libcnotify_print_data(
				 aes_ccm_key,
				 32,
				 0 );
			}
#endif
			if( metadata->password_volume_master_key->aes_ccm_encrypted_key->data_size < 28 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: password volume master key - AES-CCM encrypted key data size value out of bounds.",
				 function );

				goto on_error;
			}
			unencrypted_data_size = metadata->password_volume_master_key->aes_ccm_encrypted_key->data_size;

			unencrypted_data = (uint8_t *) memory_allocate(
							unencrypted_data_size );

			if( unencrypted_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create unencrypted data.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			if( libcaes_context_initialize(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable initialize AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_context_set_key(
			     aes_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     aes_ccm_key,
			     256,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set encryption key in AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_crypt_ccm(
			     aes_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     metadata->password_volume_master_key->aes_ccm_encrypted_key->nonce,
			     12,
			     metadata->password_volume_master_key->aes_ccm_encrypted_key->data,
			     metadata->password_volume_master_key->aes_ccm_encrypted_key->data_size,
			     unencrypted_data,
			     unencrypted_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to decrypt data.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unencrypted data:\n",
				 function );
				libcnotify_print_data(
				 unencrypted_data,
				 unencrypted_data_size,
				 0 );
			}
#endif
/* TODO improve this check */
			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 16 ] ),
			 data_size );

			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 20 ] ),
			 version );

			if( version == 1 )
			{
				if( data_size == 0x2c )
				{
					if( memory_copy(
					     volume_master_key,
					     &( unencrypted_data[ 28 ] ),
					     32 ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy unencrypted volume master key.",
						 function );

						goto on_error;
					}
					result = 1;
				}
			}
			if( libcaes_context_free(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free context.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			memory_free(
			 unencrypted_data );

			unencrypted_data = NULL;
		}
	}
	if( result == 0 )
	{
		if( password_keep->recovery_password_is_set != 0 )
		{
			if( metadata->recovery_password_volume_master_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - missing recovery password volume master key.",
				 function );

				return( -1 );
			}
			if( metadata->recovery_password_volume_master_key->stretch_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - invalid recovery password volume master key - missing stretch key.",
				 function );

				return( -1 );
			}
			if( metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid metadata - invalid recovery password volume master key - missing AES-CCM encrypted key.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     aes_ccm_key,
			     0,
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear AES-CCM key.",
				 function );

				goto on_error;
			}
			if( libbde_password_calculate_key(
			     password_keep->recovery_password_hash,
			     32,
			     metadata->recovery_password_volume_master_key->stretch_key->salt,
			     16,
			     aes_ccm_key,
			     32,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine AES-CCM key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: AES-CCM key:\n",
				 function );
				libcnotify_print_data(
				 aes_ccm_key,
				 32,
				 0 );
			}
#endif
			if( metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key->data_size < 28 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: recovery password volume master key - AES-CCM encrypted key data size value out of bounds.",
				 function );

				goto on_error;
			}
			unencrypted_data_size = metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key->data_size;

			unencrypted_data = (uint8_t *) memory_allocate(
							unencrypted_data_size );

			if( unencrypted_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create unencrypted data.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			if( libcaes_context_initialize(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable initialize AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_context_set_key(
			     aes_context,
			     LIBCAES_CRYPT_MODE_ENCRYPT,
			     aes_ccm_key,
			     256,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set encryption key in AES context.",
				 function );

				goto on_error;
			}
			if( libcaes_crypt_ccm(
			     aes_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key->nonce,
			     12,
			     metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key->data,
			     metadata->recovery_password_volume_master_key->aes_ccm_encrypted_key->data_size,
			     unencrypted_data,
			     unencrypted_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
				 "%s: unable to decrypt data.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unencrypted data:\n",
				 function );
				libcnotify_print_data(
				 unencrypted_data,
				 unencrypted_data_size,
				 0 );
			}
#endif
/* TODO improve this check */
			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 16 ] ),
			 data_size );

			byte_stream_copy_to_uint16_little_endian(
			 &( unencrypted_data[ 20 ] ),
			 version );

			if( version == 1 )
			{
				if( data_size == 0x2c )
				{
					if( memory_copy(
					     volume_master_key,
					     &( unencrypted_data[ 28 ] ),
					     32 ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
						 "%s: unable to copy unencrypted volume master key.",
						 function );

						goto on_error;
					}
					result = 1;
				}
			}
			if( libcaes_context_free(
			     &aes_context,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable free context.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     unencrypted_data,
			     0,
			     unencrypted_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear unencrypted data.",
				 function );

				goto on_error;
			}
			memory_free(
			 unencrypted_data );

			unencrypted_data = NULL;
		}
	}
	if( result == 0 )
	{
		/* Keys were set manually
		 */
		if( io_handle->keys_are_set != 0 )
		{
			result = 1;
		}
	}
	return( result );

on_error:
	if( unencrypted_data != NULL )
	{
		memory_set(
		 unencrypted_data,
		 0,
		 unencrypted_data_size );
		memory_free(
		 unencrypted_data );
	}
	if( aes_context != NULL )
	{
		libcaes_context_free(
		 &aes_context,
		 NULL );
	}
	return( -1 );
}

/* Reads the full volume encryption key from the metadata
 * Returns 1 if successful, 0 if no key could be obtained or -1 on error
 */
int libbde_metadata_read_full_volume_encryption_key(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     uint8_t *full_volume_encryption_key,
     size_t full_volume_encryption_key_size,
     uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error )
{
	uint8_t *unencrypted_data      = NULL;
	libcaes_context_t *aes_context = NULL;
	static char *function          = "libbde_metadata_read_full_volume_encryption_key";
	size_t unencrypted_data_size   = 0;
	uint32_t data_size             = 0;
	uint32_t version               = 0;
	int result                     = 0;

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( metadata->full_volume_encryption_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid metadata - missing full volume encryption key.",
		 function );

		return( -1 );
	}
	if( metadata->full_volume_encryption_key->data_size < 28 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: full volume encryption key data size value out of bounds.",
		 function );

		goto on_error;
	}
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
	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( volume_master_key_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid volume master key value too small.",
		 function );

		return( -1 );
	}
	if( full_volume_encryption_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid full volume encryption key.",
		 function );

		return( -1 );
	}
	if( full_volume_encryption_key_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid full volume encryption key value too small.",
		 function );

		return( -1 );
	}
	if( tweak_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid TWEAK key.",
		 function );

		return( -1 );
	}
	if( tweak_key_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid TWEAK key value too small.",
		 function );

		return( -1 );
	}
	unencrypted_data_size = metadata->full_volume_encryption_key->data_size;

	unencrypted_data = (uint8_t *) memory_allocate(
	                                unencrypted_data_size );

	if( unencrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create unencrypted data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     unencrypted_data,
	     0,
	     unencrypted_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear unencrypted data.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_set_key(
	     aes_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     volume_master_key,
	     256,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set encryption key in AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_crypt_ccm(
	     aes_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     metadata->full_volume_encryption_key->nonce,
	     12,
	     metadata->full_volume_encryption_key->data,
	     metadata->full_volume_encryption_key->data_size,
	     unencrypted_data,
	     unencrypted_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
		 "%s: unable to decrypt data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unencrypted data:\n",
		 function );
		libcnotify_print_data(
		 unencrypted_data,
		 unencrypted_data_size,
		 0 );
	}
#endif
/* TODO improve this check */
	byte_stream_copy_to_uint16_little_endian(
	 &( unencrypted_data[ 16 ] ),
	 data_size );

	byte_stream_copy_to_uint16_little_endian(
	 &( unencrypted_data[ 20 ] ),
	 version );

	if( version == 1 )
	{
		if( data_size == 0x1c )
		{
			if( memory_copy(
			     full_volume_encryption_key,
			     &( unencrypted_data[ 28 ] ),
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unencrypted full volume encryption key.",
				 function );

				goto on_error;
			}
			result = 1;
		}
		else if( data_size == 0x2c )
		{
			if( memory_copy(
			     full_volume_encryption_key,
			     &( unencrypted_data[ 28 ] ),
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unencrypted full volume encryption key.",
				 function );

				goto on_error;
			}
			result = 1;
		}
		else if( data_size == 0x4c )
		{
			if( memory_copy(
			     full_volume_encryption_key,
			     &( unencrypted_data[ 28 ] ),
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unencrypted full volume encryption key.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     tweak_key,
			     &( unencrypted_data[ 60 ] ),
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unencrypted TWEAK key.",
				 function );

				goto on_error;
			}
			result = 1;
		}
	}
	if( libcaes_context_free(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable free context.",
		 function );

		goto on_error;
	}
	if( unencrypted_data != NULL )
	{
		if( memory_set(
		     unencrypted_data,
		     0,
		     unencrypted_data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear unencrypted data.",
			 function );

			goto on_error;
		}
		memory_free(
		 unencrypted_data );
	}
	if( result == 0 )
	{
		/* Keys were set manually
		 */
		if( io_handle->keys_are_set != 0 )
		{
			if( memory_copy(
			     full_volume_encryption_key,
			     io_handle->full_volume_encryption_key,
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy full volume encryption key.",
				 function );

				goto on_error;
			}
			if( io_handle->tweak_key_size > 0 )
			{
				if( memory_copy(
				     tweak_key,
				     io_handle->tweak_key,
				     32 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy tweak key.",
					 function );

					goto on_error;
				}
			}
			result = 1;
		}
	}
	return( result );

on_error:
	if( unencrypted_data != NULL )
	{
		memory_set(
		 unencrypted_data,
		 0,
		 unencrypted_data_size );
		memory_free(
		 unencrypted_data );
	}
	if( aes_context != NULL )
	{
		libcaes_context_free(
		 &aes_context,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the volume identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_get_volume_identifier(
     libbde_metadata_t *metadata,
     uint8_t *volume_identifier,
     size_t size,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_volume_identifier";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( volume_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: volume identifier too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_identifier,
	     metadata->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set volume identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation date and time
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_get_creation_time(
     libbde_metadata_t *metadata,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_creation_time";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filetime.",
		 function );

		return( -1 );
	}
	*filetime = metadata->creation_time;

	return( 1 );
}

/* Retrieves the UTF-8 string size of the metadata description
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libbde_metadata_get_utf8_description_size(
     libbde_metadata_t *metadata,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_utf8_description_size";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( ( metadata->description == NULL )
	 || ( metadata->description_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     metadata->description,
	     metadata->description_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the metadata description
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libbde_metadata_get_utf8_description(
     libbde_metadata_t *metadata,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_utf8_description";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( ( metadata->description == NULL )
	 || ( metadata->description_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     metadata->description,
	     metadata->description_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the metadata description
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libbde_metadata_get_utf16_description_size(
     libbde_metadata_t *metadata,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_utf16_description_size";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( ( metadata->description == NULL )
	 || ( metadata->description_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     metadata->description,
	     metadata->description_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the metadata description
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libbde_metadata_get_utf16_description(
     libbde_metadata_t *metadata,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_utf16_description";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( ( metadata->description == NULL )
	 || ( metadata->description_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     metadata->description,
	     metadata->description_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of volume master keys
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_get_number_of_volume_master_keys(
     libbde_metadata_t *metadata,
     int *number_of_keys,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_number_of_volume_master_keys";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     metadata->volume_master_keys_array,
	     number_of_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of volume master keys.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific volume master key
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_get_volume_master_key_by_index(
     libbde_metadata_t *metadata,
     int key_index,
     libbde_volume_master_key_t **key,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_get_volume_master_key_by_index";

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     metadata->volume_master_keys_array,
	     key_index,
	     (intptr_t **) key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume master key: %d.",
		 function,
		 key_index );

		return( -1 );
	}
	return( 1 );
}

