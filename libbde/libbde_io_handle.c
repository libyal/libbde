/*
 * Input/Output (IO) handle functions
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

#include "libbde_definitions.h"
#include "libbde_encryption_context.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_ntfs_volume_header.h"
#include "libbde_sector_data.h"
#include "libbde_unused.h"

#include "bde_volume.h"

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
	io_handle->bytes_per_sector = 512;

	return( 1 );
}

/* Reads the unencrypted volume header
 * Returns 1 if successful or -1 on error
 */
int libbde_io_handle_read_unencrypted_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t volume_header_offset,
     libbde_encryption_context_t *encryption_context,
     libcerror_error_t **error )
{
	libbde_ntfs_volume_header_t *ntfs_volume_header = NULL;
	libbde_sector_data_t *sector_data               = NULL;
	static char *function                           = "libbde_io_handle_read_unencrypted_volume_header";

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
	if( libbde_sector_data_read_file_io_handle(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     volume_header_offset,
	     encryption_context,
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
	if( io_handle->volume_size == 0 )
	{
		if( libbde_ntfs_volume_header_initialize(
		     &ntfs_volume_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create NTFS volume header.",
			 function );

			goto on_error;
		}
		if( libbde_ntfs_volume_header_read_data(
		     ntfs_volume_header,
		     sector_data->data,
		     sector_data->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read NTFS volume header.",
			 function );

			goto on_error;
		}
		io_handle->volume_size = ntfs_volume_header->volume_size;

		if( libbde_ntfs_volume_header_free(
		     &ntfs_volume_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free NTFS volume header.",
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
	return( 1 );

on_error:
	if( ntfs_volume_header != NULL )
	{
		libbde_ntfs_volume_header_free(
		 &ntfs_volume_header,
		 NULL );
	}
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

