/*
 * OSS-Fuzz target for libbde volume type
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libbde.h"

#if !defined( LIBBDE_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libbde_error_t **error );

#endif /* !defined( LIBBDE_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t buffer[ 512 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 64 ];
	uint8_t guid[ 16 ];

	libbfio_handle_t *file_io_handle = NULL;
	libbde_volume_t *volume          = NULL;
	off64_t volume_offset            = 0;
	size64_t volume_size             = 0;
	uint64_t value_64bit             = 0;
	uint16_t value_16bit             = 0;
	size_t string_size               = 0;
	int is_locked                    = 0;
	int number_of_key_protectors     = 0;
	int read_iterator                = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbde_volume_initialize(
	     &volume,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbde_volume_set_utf8_password(
	     volume,
	     (uint8_t *) "bde-TEST",
	     8,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libbde_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     LIBBDE_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_encryption_method(
	     volume,
	     &value_16bit,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_volume_identifier(
	     volume,
	     guid,
	     16,
	     NULL ) == -1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_creation_time(
	     volume,
	     &value_64bit,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_utf8_description_size(
	     volume,
	     &string_size,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_utf8_description(
	     volume,
	     utf8_string,
	     64,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_utf8_description_size(
	     volume,
	     &string_size,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_utf16_description(
	     volume,
	     utf16_string,
	     64,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	if( libbde_volume_get_number_of_key_protectors(
	     volume,
	     &number_of_key_protectors,
	     NULL ) != 1 )
	{
		goto on_error_libbde;
	}
	is_locked = libbde_volume_is_locked(
	             volume,
		     NULL );

	if( is_locked == -1 )
	{
		goto on_error_libbde;
	}
	else if( is_locked == 0 )
	{
		if( libbde_volume_get_size(
		     volume,
		     &volume_size,
		     NULL ) != 1 )
		{
			goto on_error_libbde;
		}
		for( read_iterator = 0;
		     read_iterator < 128;
		     read_iterator++ )
		{
			if( volume_offset >= volume_size )
			{
				break;
			}
			if( libbde_volume_read_buffer_at_offset(
			     volume,
			     buffer,
			     497,
			     volume_offset,
			     NULL ) == -1 )
			{
				goto on_error_libbde;
			}
			volume_offset += 497;
		}
	}
	libbde_volume_close(
	 volume,
	 NULL );

on_error_libbde:
	libbde_volume_free(
	 &volume,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */

