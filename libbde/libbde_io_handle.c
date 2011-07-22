/*
 * Input/Output (IO) handle functions
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

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libfdata.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_metadata_entry.h"
#include "libbde_sector_data.h"
#include "libbde_unused.h"
#include "libbde_volume_master_key.h"

#include "bde_metadata.h"
#include "bde_volume.h"

const uint8_t bde_boot_entry_point_vista[ 3 ] = { 0xeb, 0x52, 0x90 };
const uint8_t bde_boot_entry_point_win7[ 3 ]  = { 0xeb, 0x58, 0x90 };

const uint8_t *bde_signature = (uint8_t *) "-FVE-FS-";

/* Initialize an IO handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_initialize(
     libbde_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libbde_io_handle_initialize";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle == NULL )
	{
		*io_handle = memory_allocate_structure(
		              libbde_io_handle_t );

		if( *io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create IO handle.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *io_handle,
		     0,
		     sizeof( libbde_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			memory_free(
			 *io_handle );

			*io_handle = NULL;

			return( -1 );
		}
/* TODO init encryption context */
		( *io_handle )->sector_size = 512;
	}
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
     liberror_error_t **error )
{
	static char *function = "libbde_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error )
{
	uint8_t *volume_header_data = NULL;
	static char *function       = "libbde_io_handle_read_volume_header";
	size_t read_size            = 512;
	ssize_t read_count          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t guid_string[ LIBFGUID_IDENTIFIER_STRING_SIZE ];

	libfguid_identifier_t *guid = NULL;
	int result                  = 0;
#endif

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek volume header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	volume_header_data = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * read_size );

	if( volume_header_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              volume_header_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libnotify_print_data(
		 volume_header_data,
		 read_size );
	}
#endif
	if( memory_compare(
	     volume_header_data,
	     bde_boot_entry_point_vista,
	     3 ) == 0 )
	{
		io_handle->version = 1;
	}
	else if( memory_compare(
	          volume_header_data,
	          bde_boot_entry_point_win7,
	          3 ) == 0 )
	{
		io_handle->version = 2;
	}
	else
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume boot entry point.",
		 function );

		goto on_error;
	}
	if( memory_compare(
	     &( volume_header_data[ 3 ] ),
	     bde_signature,
	     8 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid volume signature.",
		 function );

		goto on_error;
	}
	if( io_handle->version == 1 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_v1_t *) volume_header_data )->first_metadata_offset,
		 io_handle->first_metadata_offset );
	}
	else if( io_handle->version == 2 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_v2_t *) volume_header_data )->first_metadata_offset,
		 io_handle->first_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_v2_t *) volume_header_data )->second_metadata_offset,
		 io_handle->second_metadata_offset );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_volume_header_v2_t *) volume_header_data )->third_metadata_offset,
		 io_handle->third_metadata_offset );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: boot entry point:\n",
		 function );
		libnotify_print_data(
		 volume_header_data,
		 3 );

		libnotify_printf(
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

		if( io_handle->version == 1 )
		{
			libnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libnotify_print_data(
			 ( (bde_volume_header_v1_t *) volume_header_data )->unknown2,
			 45 );
		}
		else if( io_handle->version == 2 )
		{
			libnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libnotify_print_data(
			 ( (bde_volume_header_v2_t *) volume_header_data )->unknown2,
			 149 );

			if( libfguid_identifier_initialize(
			     &guid,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create GUID.",
				 function );

				goto on_error;
			}
			if( libfguid_identifier_copy_from_byte_stream(
			     guid,
			     ( (bde_volume_header_v2_t *) volume_header_data )->bitlocker_identifier,
			     16,
			     LIBFGUID_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy byte stream to GUID.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libfguid_identifier_copy_to_utf16_string(
				  guid,
				  (uint16_t *) guid_string,
				  LIBFGUID_IDENTIFIER_STRING_SIZE,
				  error );
#else
			result = libfguid_identifier_copy_to_utf8_string(
				  guid,
				  (uint8_t *) guid_string,
				  LIBFGUID_IDENTIFIER_STRING_SIZE,
				  error );
#endif
			if( result != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy GUID to string.",
				 function );

				goto on_error;
			}
			libnotify_printf(
			 "%s: BitLocker identifier\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 function,
			 guid_string );

			if( libfguid_identifier_free(
			     &guid,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free GUID.",
				 function );

				goto on_error;
			}
		}
		libnotify_printf(
		 "%s: first metadata offset\t\t: 0x%08" PRIx64 "\n",
		 function,
		 io_handle->first_metadata_offset );

		if( io_handle->version == 2 )
		{
			libnotify_printf(
			 "%s: second metadata offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->second_metadata_offset );

			libnotify_printf(
			 "%s: third metadata offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 io_handle->third_metadata_offset );
		}
		libnotify_printf(
		 "\n" );
	}
#endif
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

/* Reads the metadata
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_metadata(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error )
{
	libbde_metadata_entry_t *metadata_entry       = NULL;
	libbde_volume_master_key_t *volume_master_key = NULL;
	uint8_t *fve_metadata                         = NULL;
	uint8_t *fve_metadata_block                   = NULL;
	static char *function                         = "libbde_io_handle_read_metadata";
	size_t read_size                              = 4096;
	ssize_t read_count                            = 0;
	uint32_t metadata_header_size                 = 0;
	uint32_t metadata_size                        = 0;
	uint32_t metadata_size_copy                   = 0;
	uint32_t version                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t filetime_string[ 24 ];
	libcstring_system_character_t guid_string[ LIBFGUID_IDENTIFIER_STRING_SIZE ];

	libfdatetime_filetime_t *filetime             = NULL;
	libfguid_identifier_t *guid                   = NULL;
	uint64_t value_64bit                          = 0;
	uint32_t value_32bit                          = 0;
	uint16_t value_16bit                          = 0;
	int result                                    = 0;
#endif

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek metadata offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	fve_metadata_block = (uint8_t *) memory_allocate(
	                                  sizeof( uint8_t ) * read_size );

	if( fve_metadata_block == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create FVE metadata block.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              fve_metadata_block,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata block.",
		 function );

		goto on_error;
	}
	fve_metadata = fve_metadata_block;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: FVE metadata block header:\n",
		 function );
		libnotify_print_data(
		 fve_metadata,
		 sizeof( bde_metadata_block_header_v1_t ) );
	}
#endif
	if( memory_compare(
	     fve_metadata,
	     bde_signature,
	     8 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid metadata block signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_block_header_v1_t *) fve_metadata )->version,
	 version );

	if( ( version != 1 )
	 && ( version != 2 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata block version.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 fve_metadata[ 0 ],
		 fve_metadata[ 1 ],
		 fve_metadata[ 2 ],
		 fve_metadata[ 3 ],
		 fve_metadata[ 4 ],
		 fve_metadata[ 5 ],
		 fve_metadata[ 6 ],
		 fve_metadata[ 7 ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->size,
		 value_16bit );
		libnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 version );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->unknown1,
		 value_16bit );
		libnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->unknown2,
		 value_16bit );
		libnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( version == 1 )
		{
			libnotify_printf(
			 "%s: unknown3:\n",
			 function );
			libnotify_print_data(
			 ( (bde_metadata_block_header_v1_t *) fve_metadata )->unknown3,
			 16 );
		}
		else if( version == 2 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_metadata_block_header_v2_t *) fve_metadata )->volume_size,
			 value_64bit );
			libnotify_printf(
			 "%s: volume size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_metadata_block_header_v2_t *) fve_metadata )->unknown3,
			 value_32bit );
			libnotify_printf(
			 "%s: unknown3\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (bde_metadata_block_header_v2_t *) fve_metadata )->volume_size,
			 value_32bit );
			libnotify_printf(
			 "%s: number of volume header sectors\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->first_metadata_offset,
		 value_64bit );
		libnotify_printf(
		 "%s: first metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->second_metadata_offset,
		 value_64bit );
		libnotify_printf(
		 "%s: second metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (bde_metadata_block_header_v1_t *) fve_metadata )->third_metadata_offset,
		 value_64bit );
		libnotify_printf(
		 "%s: third metadata offset\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		if( version == 1 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_metadata_block_header_v1_t *) fve_metadata )->mft_mirror_cluster_block,
			 value_64bit );
			libnotify_printf(
			 "%s: MFT mirror cluster block\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		else if( version == 2 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (bde_metadata_block_header_v2_t *) fve_metadata )->volume_header_offset,
			 value_64bit );
			libnotify_printf(
			 "%s: volume header offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		libnotify_printf(
		 "\n" );
	}
#endif
/* TODO what about size value ? */
	fve_metadata += sizeof( bde_metadata_block_header_v1_t );
	read_size    -= sizeof( bde_metadata_block_header_v1_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: FVE metadata header:\n",
		 function );
		libnotify_print_data(
		 fve_metadata,
		 sizeof( bde_metadata_header_v1_t ) );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) fve_metadata )->metadata_size,
	 metadata_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) fve_metadata )->version,
	 version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) fve_metadata )->metadata_header_size,
	 metadata_header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) fve_metadata )->metadata_size_copy,
	 metadata_size_copy );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) fve_metadata )->encryption_method,
	 io_handle->encryption_method );

	if( version != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata header version.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: metadata size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_size );

		libnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 version );

		libnotify_printf(
		 "%s: metadata header size\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_header_size );

		libnotify_printf(
		 "%s: metadata size copy\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_size_copy );

		if( libfguid_identifier_initialize(
		     &guid,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create GUID.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     ( (bde_metadata_header_v1_t *) fve_metadata )->volume_identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  LIBFGUID_IDENTIFIER_STRING_SIZE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  LIBFGUID_IDENTIFIER_STRING_SIZE,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: volume identifier\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 function,
		 guid_string );

		if( libfguid_identifier_free(
		     &guid,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free GUID.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_header_v1_t *) fve_metadata )->next_nonce_counter,
		 value_32bit );
		libnotify_printf(
		 "%s: next nonce counter\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libnotify_printf(
		 "%s: encryption method\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 io_handle->encryption_method,
		 libbde_debug_print_encryption_method(
		  io_handle->encryption_method ) );

		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create filetime.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (bde_metadata_header_v1_t *) fve_metadata )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  24,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  24,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: creation time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free filetime.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	if( metadata_header_size != sizeof( bde_metadata_header_v1_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata header size.",
		 function );

		goto on_error;
	}
	if( metadata_size != metadata_size_copy )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_INPUT,
		 LIBERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata size and copy.",
		 function );

		goto on_error;
	}
	if( ( metadata_size < sizeof( bde_metadata_header_v1_t ) )
	 || ( metadata_size > read_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: metadata size value out of bounds.",
		 function );

		goto on_error;
	}
	fve_metadata  += sizeof( bde_metadata_header_v1_t );
	metadata_size -= sizeof( bde_metadata_header_v1_t );

	while( metadata_size > sizeof( bde_metadata_header_v1_t ) )
	{
		if( libbde_metadata_entry_initialize(
		     &metadata_entry,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create metadata entry.",
			 function );

			goto on_error;
		}
		read_count = libbde_metadata_entry_read(
			      metadata_entry,
			      fve_metadata,
			      metadata_size,
			      error );

		if( read_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read metadata entry.",
			 function );

			goto on_error;
		}
/* TODO refactor */

		/* TODO */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			switch( metadata_entry->type )
			{
				case 0x0002:
					if( libbde_volume_master_key_initialize(
					     &volume_master_key,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create volume master key.",
						 function );

						goto on_error;
					}
					if( libbde_volume_master_key_read(
					     volume_master_key,
					     io_handle,
					     metadata_entry,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read volume master key metadata entry.",
						 function );

						libbde_volume_master_key_free(
						 &volume_master_key,
						 NULL );

						goto on_error;
					}
					if( libbde_volume_master_key_free(
					     &volume_master_key,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free volume master key.",
						 function );

						goto on_error;
					}
					break;

				case 0x0007:
					if( libbde_metadata_entry_read_string(
					     metadata_entry,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_IO,
						 LIBERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read string metadata entry.",
						 function );

						goto on_error;
					}
					break;

				case 0x000f:
					if( ( metadata_entry->value_type == 0x000f )
					 && ( metadata_entry->value_data_size == 16 ) )
					{
						if( libfguid_identifier_initialize(
						     &guid,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
							 "%s: unable to create GUID.",
							 function );

							goto on_error;
						}
						if( libfguid_identifier_copy_from_byte_stream(
						     guid,
						     metadata_entry->value_data,
						     16,
						     LIBFGUID_ENDIAN_LITTLE,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
							 "%s: unable to copy byte stream to GUID.",
							 function );

							goto on_error;
						}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
						result = libfguid_identifier_copy_to_utf16_string(
							  guid,
							  (uint16_t *) guid_string,
							  LIBFGUID_IDENTIFIER_STRING_SIZE,
							  error );
#else
						result = libfguid_identifier_copy_to_utf8_string(
							  guid,
							  (uint8_t *) guid_string,
							  LIBFGUID_IDENTIFIER_STRING_SIZE,
							  error );
#endif
						if( result != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
							 "%s: unable to copy GUID to string.",
							 function );

							goto on_error;
						}
						libnotify_printf(
						 "%s: volume identifier\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
						 function,
						 guid_string );

						if( libfguid_identifier_free(
						     &guid,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free GUID.",
							 function );

							goto on_error;
						}
						libnotify_printf(
						 "\n" );
					}
					break;

				default:
					break;
			}
		}
#endif
		fve_metadata  += read_count;
		metadata_size -= read_count;

		if( libbde_metadata_entry_free(
		     &metadata_entry,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free metadata entry.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 fve_metadata_block );

	fve_metadata_block = NULL;

	return( 1 );

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
	if( metadata_entry != NULL )
	{
		libbde_metadata_entry_free(
		 &metadata_entry,
		 NULL );
	}
	if( fve_metadata_block != NULL )
	{
		memory_free(
		 fve_metadata_block );
	}
	return( -1 );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_sector(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     off64_t element_data_offset,
     size64_t element_data_size LIBBDE_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBBDE_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	libbde_sector_data_t *sector_data = NULL;
	static char *function             = "libbde_io_handle_read_sector";

	LIBBDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBBDE_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
/* TODO handle virtual sectors, what about different sector sizes? */
	if( libbde_sector_data_initialize(
	     &sector_data,
	     ( (libbde_io_handle_t *) io_handle )->sector_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( libbde_sector_data_read(
	     sector_data,
	     file_io_handle,
	     element_data_offset,
	     ( (libbde_io_handle_t *) io_handle )->encryption_context,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     cache,
	     element_index,
	     (intptr_t *) sector_data,
	     (int (*)(intptr_t *, liberror_error_t **)) &libbde_sector_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data as element value.",
		 function );

		return( -1 );
	}
	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 sector_data,
		 NULL );
	}
	return( -1 );
}

