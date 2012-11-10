/*
 * Volume functions
 *
 * Copyright (C) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBDE_INTERNAL_VOLUME_H )
#define _LIBBDE_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libbde_extern.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libfcache.h"
#include "libbde_libfdata.h"
#include "libbde_metadata.h"
#include "libbde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_internal_volume libbde_internal_volume_t;

struct libbde_internal_volume
{
	/* The encryption method
	 */
	uint32_t encryption_method;

	/* The metadata at the first metadata offset
	 */
	libbde_metadata_t *primary_metadata;

	/* The metadata at the second metadata offset
	 */
	libbde_metadata_t *secondary_metadata;

	/* The metadata at the third metadata offset
	 */
	libbde_metadata_t *tertiary_metadata;

	/* The metadata stored in a startup key file
	 */
	libbde_metadata_t *external_key_metadata;

	/* The sectors vector
	 */
	libfdata_vector_t *sectors_vector;

	/* The sectors cache
	 */
	libfcache_cache_t *sectors_cache;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libbde_io_handle_t *io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;
};

LIBBDE_EXTERN \
int libbde_volume_initialize(
     libbde_volume_t **volume,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_free(
     libbde_volume_t **volume,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_signal_abort(
     libbde_volume_t *volume,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_open(
     libbde_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBBDE_EXTERN \
int libbde_volume_open_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );
#endif

LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_close(
     libbde_volume_t *volume,
     libcerror_error_t **error );

int libbde_volume_open_read(
     libbde_internal_volume_t *internal_volume,
     libcerror_error_t **error );

int libbde_volume_open_read_keys_from_metadata(
     libbde_internal_volume_t *internal_volume,
     libbde_metadata_t *metadata,
     libcerror_error_t **error );

LIBBDE_EXTERN \
ssize_t libbde_volume_read_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBDE_EXTERN \
ssize_t libbde_volume_read_random(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

#ifdef TODO
LIBBDE_EXTERN \
ssize_t libbde_volume_write_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBDE_EXTERN \
ssize_t libbde_volume_write_random(
         libbde_volume_t *volume,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );
#endif

LIBBDE_EXTERN \
off64_t libbde_volume_seek_offset(
         libbde_volume_t *volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_size(
     libbde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_encryption_method(
     libbde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_set_keys(
     libbde_volume_t *volume,
     const uint8_t *full_volume_encryption_key,
     size_t full_volume_encryption_key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_set_utf8_password(
     libbde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_set_utf16_password(
     libbde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_set_utf8_recovery_password(
     libbde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_set_utf16_recovery_password(
     libbde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_read_startup_key(
     libbde_volume_t *volume,
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBBDE_EXTERN \
int libbde_volume_read_startup_key_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     libcerror_error_t **error );
#endif

LIBBDE_EXTERN \
int libbde_volume_read_startup_key_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

