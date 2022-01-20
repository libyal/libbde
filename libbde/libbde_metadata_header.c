/*
 * Metadata header functions
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
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_metadata_header.h"

#include "bde_metadata.h"

/* Creates a metadata header
 * Make sure the value metadata header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_header_initialize(
     libbde_metadata_header_t **metadata_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_header_initialize";

	if( metadata_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata header.",
		 function );

		return( -1 );
	}
	if( *metadata_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata header value already set.",
		 function );

		return( -1 );
	}
	*metadata_header = memory_allocate_structure(
	                          libbde_metadata_header_t );

	if( *metadata_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_header,
	     0,
	     sizeof( libbde_metadata_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_header != NULL )
	{
		memory_free(
		 *metadata_header );

		*metadata_header = NULL;
	}
	return( -1 );
}

/* Frees a metadata header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_header_free(
     libbde_metadata_header_t **metadata_header,
     libcerror_error_t **error )
{
	static char *function = "libbde_metadata_header_free";

	if( metadata_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata header.",
		 function );

		return( -1 );
	}
	if( *metadata_header != NULL )
	{
		memory_free(
		 *metadata_header );

		*metadata_header = NULL;
	}
	return( 1 );
}

/* Reads a metadata header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_header_read_data(
     libbde_metadata_header_t *metadata_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function           = "libbde_metadata_header_read_data";
	uint32_t format_version         = 0;
	uint32_t header_size            = 0;
	uint32_t metadata_size_copy     = 0;
	uint16_t encryption_method_copy = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit            = 0;
#endif

	if( metadata_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata header.",
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
	if( ( data_size < sizeof( bde_metadata_header_v1_t ) )
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
		 "%s: FVE metadata header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( bde_metadata_header_v1_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->metadata_size,
	 metadata_header->metadata_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->version,
	 format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->metadata_header_size,
	 header_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->metadata_size_copy,
	 metadata_size_copy );

	if( memory_copy(
	     metadata_header->volume_identifier,
	     ( (bde_metadata_header_v1_t *) data )->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->encryption_method,
	 metadata_header->encryption_method );

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->encryption_method_copy,
	 encryption_method_copy );

	byte_stream_copy_to_uint64_little_endian(
	 ( (bde_metadata_header_v1_t *) data )->creation_time,
	 metadata_header->creation_time );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: metadata size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_header->metadata_size );

		libcnotify_printf(
		 "%s: format version\t\t\t: %" PRIu32 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: header size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 header_size );

		libcnotify_printf(
		 "%s: metadata size copy\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_size_copy );

		if( libbde_debug_print_guid_value(
		     function,
		     "volume identifier\t\t\t",
		     metadata_header->volume_identifier,
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
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_header_v1_t *) data )->next_nonce_counter,
		 value_32bit );
		libcnotify_printf(
		 "%s: next nonce counter\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: encryption method\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 metadata_header->encryption_method,
		 libbde_debug_print_encryption_method(
		  metadata_header->encryption_method ) );

		libcnotify_printf(
		 "%s: encryption method copy\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 encryption_method_copy,
		 libbde_debug_print_encryption_method(
		  encryption_method_copy ) );

		if( libbde_debug_print_filetime_value(
		     function,
		     "creation time\t\t\t\t",
		     ( (bde_metadata_header_v1_t *) data )->creation_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version.",
		 function );

		return( -1 );
	}
	if( header_size != sizeof( bde_metadata_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for header size.",
		 function );

		return( -1 );
	}
	if( metadata_header->metadata_size != metadata_size_copy )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata size and copy.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a metadata header
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_header_read_file_io_handle(
     libbde_metadata_header_t *metadata_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t metadata_header_data[ sizeof( bde_metadata_header_v1_t ) ];

	static char *function = "libbde_metadata_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( metadata_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata header.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              metadata_header_data,
	              sizeof( bde_metadata_header_v1_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( bde_metadata_header_v1_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libbde_metadata_header_read_data(
	     metadata_header,
	     metadata_header_data,
	     sizeof( bde_metadata_header_v1_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read FVE metadata header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

