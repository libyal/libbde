/*
 * Sector data functions
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

#include "libbde_encryption.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_sector_data.h"

/* Initialize sector data
 * Make sure the value sector_data is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_initialize(
     libbde_sector_data_t **sector_data,
     size_t data_size,
     liberror_error_t **error )
{
	static char *function = "libbde_sector_data_initialize";

	if( sector_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( *sector_data == NULL )
	{
		*sector_data = memory_allocate_structure(
		                libbde_sector_data_t );

		if( *sector_data == NULL )
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
		     *sector_data,
		     0,
		     sizeof( libbde_sector_data_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear IO handle.",
			 function );

			memory_free(
			 *sector_data );

			*sector_data = NULL;

			return( -1 );
		}
		if( data_size > 0 )
		{
			( *sector_data )->encrypted_data = (uint8_t *) memory_allocate(
			                                                sizeof( uint8_t ) * data_size );

			if( ( *sector_data )->encrypted_data == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create encrypted data.",
				 function );

				goto on_error;
			}
			( *sector_data )->data = (uint8_t *) memory_allocate(
			                                      sizeof( uint8_t ) * data_size );

			if( ( *sector_data )->data == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create data.",
				 function );

				goto on_error;
			}
			( *sector_data )->data_size = data_size;
		}
	}
	return( 1 );

on_error:
	if( *sector_data != NULL )
	{
		if( ( *sector_data )->encrypted_data != NULL )
		{
			memory_free(
			 ( *sector_data )->encrypted_data );
		}
		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( -1 );
}

/* Frees the sector data
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_free(
     libbde_sector_data_t *sector_data,
     liberror_error_t **error )
{
	static char *function = "libbde_sector_data_free";
	int result            = 1;

	if( sector_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( sector_data->data != NULL )
	{
		if( memory_set(
		     sector_data->data,
		     0,
		     sector_data->data_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			result = -1;
		}
		memory_free(
		 sector_data->data );

		memory_free(
		 sector_data->encrypted_data );
	}
	memory_free(
	 sector_data );

	return( result );
}

/* Reads sector data
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_read(
     libbde_sector_data_t *sector_data,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t sector_data_offset,
     libbde_encryption_context_t *encryption_context,
     liberror_error_t **error )
{
	static char *function = "libbde_sector_data_read";
	ssize_t read_count    = 0;

	if( sector_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( sector_data->encrypted_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing encrypted data.",
		 function );

		return( -1 );
	}
	if( sector_data->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing data.",
		 function );

		return( -1 );
	}
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
		 "%s: reading sector data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 sector_data_offset,
		 sector_data_offset  );
	}
#endif
	if( sector_data_offset == 0 )
	{
		if( io_handle->version == 2 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: reading volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 io_handle->volume_header_offset,
				 io_handle->volume_header_offset  );
			}
#endif
			sector_data_offset = io_handle->volume_header_offset;
		}
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     sector_data_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek sector data offset: %" PRIi64 ".",
		 function,
		 sector_data_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              file_io_handle,
	              sector_data->encrypted_data,
	              sector_data->data_size,
	              error );

	if( read_count != (ssize_t) sector_data->data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: encrypted sector data:\n",
		 function );
		libnotify_print_data(
		 sector_data->encrypted_data,
		 sector_data->data_size );
	}
#endif
	if( ( sector_data_offset == io_handle->volume_header_offset )
	 && ( io_handle->version == 1 ) )
	{
		if( memory_copy(
		     sector_data->data,
		     sector_data->encrypted_data,
		     sector_data->data_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy encrypted data.",
			 function );

			return( -1 );
		}
		/* Change the volume header signature "-FVE-FS-"
		 * into "NTFS    "
		 */
		if( memory_copy(
		     &( sector_data->data[ 3 ] ),
		     "NTFS    ",
		     8 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy encrypted data.",
			 function );

			return( -1 );
		}
		/* Change the FVE metadatsa block 1 cluster block number
		 * into the MFT mirror cluster block number
		 */
		byte_stream_copy_from_uint64_little_endian(
		 &( sector_data->data[ 56 ] ),
		 io_handle->mft_mirror_cluster_block_number );
	}
	else
	{
		if( libbde_encryption_crypt(
		     encryption_context,
		     LIBBDE_ENCYPTION_CRYPT_MODE_DECRYPT,
		     sector_data->encrypted_data,
		     sector_data->data_size,
		     sector_data->data,
		     sector_data->data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decrypt sector data.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: sector data:\n",
		 function );
		libnotify_print_data(
		 sector_data->data,
		 sector_data->data_size );
	}
#endif
	return( 1 );
}

