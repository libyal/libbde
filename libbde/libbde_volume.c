/*
 * Volume functions
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libfdata.h"
#include "libbde_recovery.h"
#include "libbde_sector_data.h"
#include "libbde_volume.h"

/* Initialize a volume
 * Make sure the value volume is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_initialize(
     libbde_volume_t **volume,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_initialize";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume == NULL )
	{
		internal_volume = memory_allocate_structure(
		                   libbde_internal_volume_t );

		if( internal_volume == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     internal_volume,
		     0,
		     sizeof( libbde_internal_volume_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear volume.",
			 function );

			goto on_error;
		}
		if( libbde_io_handle_initialize(
		     &( internal_volume->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create IO handle.",
			 function );

			goto on_error;
		}
		*volume = (libbde_volume_t *) internal_volume;
	}
	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		memory_free(
		 internal_volume );
	}
	return( -1 );
}

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_free(
     libbde_volume_t **volume,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_free";
	int result                                = 1;

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		internal_volume = (libbde_internal_volume_t *) *volume;

		if( internal_volume->file_io_handle != NULL )
		{
			if( libbde_volume_close(
			     *volume,
			     error ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				result = -1;
			}
		}
		*volume = NULL;

		if( libbde_io_handle_free(
		     &( internal_volume->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume );
	}
	return( result );
}

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_signal_abort(
     libbde_volume_t *volume,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_signal_abort";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_volume->io_handle->abort = 1;

	return( 1 );
}

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_open(
     libbde_volume_t *volume,
     const char *filename,
     int access_flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_open";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     libcstring_narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libbde_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_open_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle          = NULL;
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_open_wide";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     libcstring_wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libbde_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_volume->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
        return( -1 );
}

#endif

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_open_file_io_handle";
	int bfio_access_flags                     = 0;

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->file_io_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBBDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBBDE_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	internal_volume->file_io_handle = file_io_handle;

	if( libbfio_handle_open(
	     internal_volume->file_io_handle,
	     bfio_access_flags,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_IO,
                 LIBERROR_IO_ERROR_OPEN_FAILED,
                 "%s: unable to open file IO handle.",
                 function );

                return( -1 );
	}
	if( libbde_volume_open_read(
	     internal_volume,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from volume handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
int libbde_volume_close(
     libbde_volume_t *volume,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_close";
	int result                                = 0;

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle_created_in_library != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( libbde_debug_print_read_offsets(
			     internal_volume->file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );
			}
		}
#endif
		if( libbfio_handle_close(
		     internal_volume->file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		if( libbfio_handle_free(
		     &( internal_volume->file_io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
	}
	internal_volume->file_io_handle                    = NULL;
	internal_volume->file_io_handle_created_in_library = 0;

	if( libfdata_vector_free(
	     &( internal_volume->sectors_vector ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sectors vector.",
		 function );

		result = -1;
	}
	if( libfdata_cache_free(
	     &( internal_volume->sectors_cache ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sectors cache.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_open_read(
     libbde_internal_volume_t *internal_volume,
     liberror_error_t **error )
{
	static char *function = "libbde_volume_open_read";

	if( internal_volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_vector != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal file - sectors vector already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_cache != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal file - sectors cache already set.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_seek_offset(
	     internal_volume->file_io_handle,
	     0,
	     SEEK_END,
	     error ) < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek end of volume.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_offset(
	     internal_volume->file_io_handle,
	     (off64_t *) &( internal_volume->size ),
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading BitLocker volume header:\n" );
	}
#endif
	if( libbde_io_handle_read_volume_header(
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading BitLocker first metadata:\n" );
	}
#endif
	if( libbde_io_handle_read_metadata(
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->io_handle->first_metadata_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading BitLocker second metadata:\n" );
	}
#endif
	if( libbde_io_handle_read_metadata(
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->io_handle->second_metadata_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading BitLocker third metadata:\n" );
	}
#endif
	if( libbde_io_handle_read_metadata(
	     internal_volume->io_handle,
	     internal_volume->file_io_handle,
	     internal_volume->io_handle->third_metadata_offset,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata.",
		 function );

		return( -1 );
	}
	/* TODO clone function ? */
	if( libfdata_vector_initialize(
	     &( internal_volume->sectors_vector ),
	     (size64_t) internal_volume->size,
	     (intptr_t *) internal_volume->io_handle,
	     NULL,
	     NULL,
	     &libbde_io_handle_read_sector,
	     LIBFDATA_FLAG_IO_HANDLE_NON_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sectors vector.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_append_segment(
	     internal_volume->sectors_vector,
	     0,
	     internal_volume->size,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to sectors vector.",
		 function );

		return( -1 );
	}
	if( libfdata_cache_initialize(
	     &( internal_volume->sectors_cache ),
	     LIBBDE_MAXIMUM_CACHE_ENTRIES_SECTORS,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sectors cache.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbde_volume_read_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         liberror_error_t **error )
{
	libbde_sector_data_t *sector_data         = NULL;
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_read_buffer";
	size_t buffer_offset                      = 0;
	size_t read_size                          = 0;
	size_t sector_data_offset                 = 0;
	ssize_t total_read_count                  = 0;

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_vector != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing sectors vector.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_cache != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing sectors cache.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle->current_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid internal volume - invalid IO handle - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_volume->io_handle->current_offset >= internal_volume->size )
	{
		return( 0 );
	}
	if( (size64_t) ( internal_volume->io_handle->current_offset + buffer_size ) >= internal_volume->size )
	{
		buffer_size = (size_t) ( internal_volume->size - internal_volume->io_handle->current_offset );
	}
	sector_data_offset = (size_t) ( internal_volume->io_handle->current_offset % internal_volume->io_handle->bytes_per_sector );

	while( buffer_size > 0 )
	{
		if( libfdata_vector_get_element_value_at_offset(
		     internal_volume->sectors_vector,
		     internal_volume->file_io_handle,
		     internal_volume->sectors_cache,
		     internal_volume->io_handle->current_offset,
		     (intptr_t **) &sector_data,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data at offset: %" PRIi64 ".",
			 function,
			 internal_volume->io_handle->current_offset );

			return( -1 );
		}
		if( sector_data == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector data at offset: %" PRIi64 ".",
			 function,
			 internal_volume->io_handle->current_offset );

			return( -1 );
		}
		read_size = sector_data->data_size - sector_data_offset;

		if( read_size > buffer_size )
		{
			read_size = buffer_size;
		}
		if( read_size == 0 )
		{
			break;
		}
		if( memory_copy(
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     &( ( sector_data->data )[ sector_data_offset ] ),
		     read_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy sector data to buffer.",
			 function );

			return( -1 );
		}
		buffer_offset     += read_size;
		buffer_size       -= read_size;
		total_read_count  += (ssize_t) read_size;
		sector_data_offset = 0;

		internal_volume->io_handle->current_offset += (off64_t) read_size;

		if( (size64_t) internal_volume->io_handle->current_offset >= internal_volume->size )
		{
			break;
		}
		if( internal_volume->io_handle->abort != 0 )
		{
			break;
		}
	}
	return( total_read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libbde_volume_read_random(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libbde_volume_read_random";
	ssize_t read_count    = 0;

	if( libbde_volume_seek_offset(
	     volume,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	read_count = libbde_volume_read_buffer(
	              volume,
	              buffer,
	              buffer_size,
	              error );

	if( read_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		return( -1 );
	}
	return( read_count );
}

#ifdef TODO

/* Writes (media) data at the current offset
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libbde_volume_write_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         liberror_error_t **error )
{
	return( -1 );
}

/* Writes (media) data at a specific offset,
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libbde_volume_write_random(
         libbde_volume_t *volume,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         liberror_error_t **error )
{
	static char *function = "libbde_volume_write_random";
	ssize_t write_count   = 0;

	if( libbde_volume_seek_offset(
	     volume,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libbde_volume_write_buffer(
	               volume,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

#endif

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libbde_volume_seek_offset(
         libbde_volume_t *volume,
         off64_t offset,
         int whence,
         liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_seek_offset";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_volume->io_handle->current_offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_volume->size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: seeking media data offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_volume->io_handle->current_offset = offset;

	return( offset );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_get_size(
     libbde_volume_t *volume,
     size64_t *size,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_get_size";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	*size = internal_volume->size;

	return( 1 );
}

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_volume_set_utf8_recovery_password(
     libbde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_set_utf8_recovery_password";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( libbde_recovery_password_copy_utf8_to_binary(
	     utf8_string,
	     utf8_string_size,
	     internal_volume->io_handle->recovery_password,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set recovery password.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: binary recovery password:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) internal_volume->io_handle->recovery_password,
		 16 );
	}
#endif
	internal_volume->io_handle->recovery_password_is_set = 1;

	return( 1 );
}

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_volume_set_utf16_recovery_password(
     libbde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	libbde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libbde_volume_set_utf16_recovery_password";

	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libbde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( libbde_recovery_password_copy_utf16_to_binary(
	     utf16_string,
	     utf16_string_size,
	     internal_volume->io_handle->recovery_password,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set recovery password.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: binary recovery password:\n",
		 function );
		libnotify_print_data(
		 (uint8_t *) internal_volume->io_handle->recovery_password,
		 16 );
	}
#endif
	internal_volume->io_handle->recovery_password_is_set = 1;

	return( 1 );
}

