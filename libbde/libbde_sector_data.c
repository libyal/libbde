/*
 * Sector data functions
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
#include "libbde_sector_data.h"

/* Creates sector data
 * Make sure the value sector_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_initialize(
     libbde_sector_data_t **sector_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_sector_data_initialize";

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector data value already set.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*sector_data = memory_allocate_structure(
	                libbde_sector_data_t );

	if( *sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_data,
	     0,
	     sizeof( libbde_sector_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector data.",
		 function );

		memory_free(
		 *sector_data );

		*sector_data = NULL;

		return( -1 );
	}
	( *sector_data )->data = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * data_size );

	if( ( *sector_data )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	( *sector_data )->data_size = data_size;

	return( 1 );

on_error:
	if( *sector_data != NULL )
	{
		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( -1 );
}

/* Frees sector data
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_free(
     libbde_sector_data_t **sector_data,
     libcerror_error_t **error )
{
	static char *function = "libbde_sector_data_free";
	int result            = 1;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		if( memory_set(
		     ( *sector_data )->data,
		     0,
		     ( *sector_data )->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data.",
			 function );

			result = -1;
		}
		memory_free(
		 ( *sector_data )->data );

		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( result );
}

/* Reads sector data
 * Returns 1 if successful or -1 on error
 */
int libbde_sector_data_read_file_io_handle(
     libbde_sector_data_t *sector_data,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t sector_data_offset,
     libbde_encryption_context_t *encryption_context,
     uint8_t zero_metadata,
     libcerror_error_t **error )
{
	uint8_t *encrypted_data = NULL;
	uint8_t *read_buffer    = 0;
	static char *function   = "libbde_sector_data_read_file_io_handle";
	ssize_t read_count      = 0;
	uint64_t block_key      = 0;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( sector_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing data.",
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
	if( io_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing bytes per sector.",
		 function );

		return( -1 );
	}
	if( encryption_context == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encryption context.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading sector data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 sector_data_offset,
		 sector_data_offset );
	}
#endif
	if( zero_metadata != 0 )
	{
		/* The BitLocker metadata areas are represented as zero byte blocks
		 */
		if( ( ( sector_data_offset >= io_handle->first_metadata_offset )
		  &&  ( sector_data_offset < ( io_handle->first_metadata_offset + (off64_t) io_handle->metadata_size ) ) )
		 || ( ( sector_data_offset >= io_handle->second_metadata_offset )
		  &&  ( sector_data_offset < ( io_handle->second_metadata_offset + (off64_t) io_handle->metadata_size ) ) )
		 || ( ( sector_data_offset >= io_handle->third_metadata_offset )
		  &&  ( sector_data_offset < ( io_handle->third_metadata_offset + (off64_t) io_handle->metadata_size ) ) ) )
		{
			if( memory_set(
			     sector_data->data,
			     0,
			     sector_data->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear data.",
				 function );

				goto on_error;
			}
			return( 1 );
		}
	}
	if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_7 )
	 || ( io_handle->version == LIBBDE_VERSION_TO_GO ) )
	{
		if( zero_metadata != 0 )
		{
			if( ( sector_data_offset >= io_handle->volume_header_offset )
			 && ( sector_data_offset < ( io_handle->volume_header_offset + (off64_t) io_handle->volume_header_size ) ) )
			{
				if( memory_set(
				     sector_data->data,
				     0,
				     sector_data->data_size ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to clear data.",
					 function );

					goto on_error;
				}
				return( 1 );
			}
		}
		/* Normally the first 8192 bytes are stored in another location on the volume
		 */
		if( (size64_t) sector_data_offset < io_handle->volume_header_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
				 function,
				 io_handle->volume_header_offset,
				 io_handle->volume_header_offset );
			}
#endif
			if( ( io_handle->volume_header_offset < 0 )
			 || ( sector_data_offset > (off64_t) ( INT64_MAX - io_handle->volume_header_offset ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid sector data offset value out of bounds.",
				 function );

				goto on_error;
			}
			sector_data_offset += io_handle->volume_header_offset;
		}
	}
	/* In Windows Vista the first 16 sectors are unencrypted
	 */
	if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	 && ( (size64_t) sector_data_offset < 8192 ) )
	{
		read_buffer = sector_data->data;
	}
	else if( encryption_context->method == LIBBDE_ENCRYPTION_METHOD_NONE )
	{
		read_buffer = sector_data->data;
	}
	/* Check if the offset is outside the encrypted part of the volume
	 */
	else if( ( io_handle->encrypted_volume_size != 0 )
	      && ( sector_data_offset >= (off64_t) io_handle->encrypted_volume_size ) )
	{
		read_buffer = sector_data->data;
	}
	else
	{
		encrypted_data = (uint8_t *) memory_allocate(
		                              sizeof( uint8_t ) * sector_data->data_size );

		if( encrypted_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create encrypted data.",
			 function );

			goto on_error;
		}
		read_buffer = encrypted_data;
	}
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              read_buffer,
	              sector_data->data_size,
	              sector_data_offset,
	              error );

	if( read_count != (ssize_t) sector_data->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sector data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 sector_data_offset,
		 sector_data_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( read_buffer == encrypted_data )
		{
			libcnotify_printf(
			 "%s: encrypted sector data:\n",
			 function );
			libcnotify_print_data(
			 encrypted_data,
			 sector_data->data_size,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	/* In Windows Vista the first sector is altered
	 */
	if( ( io_handle->version == LIBBDE_VERSION_WINDOWS_VISTA )
	 && ( (size64_t) sector_data_offset < 512 ) )
	{
		/* Change the volume header signature "-FVE-FS-"
		 * into "NTFS    "
		 */
		if( memory_copy(
		     &( sector_data->data[ 3 ] ),
		     "NTFS    ",
		     8 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy NTFS signature.",
			 function );

			goto on_error;
		}
		/* Change the FVE metadata block 1 cluster block number
		 * into the MFT mirror cluster block number
		 */
		byte_stream_copy_from_uint64_little_endian(
		 &( sector_data->data[ 56 ] ),
		 io_handle->mft_mirror_cluster_block_number );
	}
	if( read_buffer == encrypted_data )
	{
		block_key = (uint64_t) sector_data_offset;

		if( ( encryption_context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_XTS )
		 || ( encryption_context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_XTS ) )
		{
			block_key /= io_handle->bytes_per_sector;
		}
		if( libbde_encryption_context_crypt(
		     encryption_context,
		     LIBBDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
		     encrypted_data,
		     sector_data->data_size,
		     sector_data->data,
		     sector_data->data_size,
		     block_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decrypt sector data.",
			 function );

			goto on_error;
		}
		memory_free(
		 encrypted_data );

		encrypted_data = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sector data:\n",
		 function );
		libcnotify_print_data(
		 sector_data->data,
		 sector_data->data_size,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( encrypted_data != NULL )
	{
		memory_free(
		 encrypted_data );
	}
	return( -1 );
}

