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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libbde_encryption.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libfdata.h"
#include "libbde_libfguid.h"
#include "libbde_sector_data.h"
#include "libbde_unused.h"

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

			goto on_error;
		}
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
		if( ( *io_handle )->encryption_context != NULL )
		{
			if( libbde_encryption_free(
			     &( ( *io_handle )->encryption_context ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encryption context.",
				 function );

				result = -1;
			}
		}
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
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_volume_header_v1_t *) volume_header_data )->bytes_per_sector,
	 io_handle->bytes_per_sector );

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

		libnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->bytes_per_sector );

		if( io_handle->version == 1 )
		{
			libnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libnotify_print_data(
			 ( (bde_volume_header_v1_t *) volume_header_data )->unknown1,
			 43 );
		}
		else if( io_handle->version == 2 )
		{
			libnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libnotify_print_data(
			 ( (bde_volume_header_v2_t *) volume_header_data )->unknown1,
			 147 );

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
	     (size_t) ( (libbde_io_handle_t *) io_handle )->bytes_per_sector,
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
	     (libbde_io_handle_t *) io_handle,
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

