/*
 * Volume functions
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

#if !defined( _LIBBDE_INTERNAL_VOLUME_H )
#define _LIBBDE_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libbde_encryption_context.h"
#include "libbde_extern.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcthreads.h"
#include "libbde_metadata.h"
#include "libbde_password_keep.h"
#include "libbde_sector_data_vector.h"
#include "libbde_types.h"
#include "libbde_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_internal_volume libbde_internal_volume_t;

struct libbde_internal_volume
{
	/* The current (storage media) offset
	 */
	off64_t current_offset;

	/* The encryption method
	 */
	uint16_t encryption_method;

	/* The volume header
	 */
	libbde_volume_header_t *volume_header;

	/* The metadata at the first metadata offset
	 */
	libbde_metadata_t *primary_metadata;

	/* The metadata at the second metadata offset
	 */
	libbde_metadata_t *secondary_metadata;

	/* The metadata at the third metadata offset
	 */
	libbde_metadata_t *tertiary_metadata;

	/* The metadata volumed in a startup key file
	 */
	libbde_metadata_t *external_key_metadata;

	/* The encryption context
	 */
	libbde_encryption_context_t *encryption_context;

	/* The sector data vector
	 */
	libbde_sector_data_vector_t *sector_data_vector;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libbde_io_handle_t *io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* External provided full volume encryption key
	 */
	uint8_t full_volume_encryption_key[ 64 ];

	/* Size of the external provided full volume encryption key
	 */
	size_t full_volume_encryption_key_size;

	/* External provided tweak key
	 */
	uint8_t tweak_key[ 32 ];

	/* Size of the tweak key
	 */
	size_t tweak_key_size;

	/* Value to indicate the keys are set
	 */
	uint8_t keys_are_set;

	/* The password keep
	 */
	libbde_password_keep_t *password_keep;

	/* Value to indicate if the volume is locked
	 */
	uint8_t is_locked;

#if defined( HAVE_LIBBDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
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

int libbde_internal_volume_open_read(
     libbde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libbde_internal_volume_open_read_keys(
     libbde_internal_volume_t *internal_volume,
     libcerror_error_t **error );

int libbde_internal_volume_open_read_keys_from_metadata(
     libbde_internal_volume_t *internal_volume,
     libbde_metadata_t *metadata,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_is_locked(
     libbde_volume_t *volume,
     libcerror_error_t **error );

int libbde_internal_volume_unlock(
     libbde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_unlock(
     libbde_volume_t *volume,
     libcerror_error_t **error );

ssize_t libbde_internal_volume_read_buffer_from_file_io_handle(
         libbde_internal_volume_t *internal_volume,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBDE_EXTERN \
ssize_t libbde_volume_read_buffer(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBBDE_EXTERN \
ssize_t libbde_volume_read_buffer_at_offset(
         libbde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

off64_t libbde_internal_volume_seek_offset(
         libbde_internal_volume_t *internal_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBDE_EXTERN \
off64_t libbde_volume_seek_offset(
         libbde_volume_t *volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_offset(
     libbde_volume_t *volume,
     off64_t *offset,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_size(
     libbde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_encryption_method(
     libbde_volume_t *volume,
     uint16_t *encryption_method,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_volume_identifier(
     libbde_volume_t *volume,
     uint8_t *guid_data,
     size_t guid_data_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_creation_time(
     libbde_volume_t *volume,
     uint64_t *filetime,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_utf8_description_size(
     libbde_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_utf8_description(
     libbde_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_utf16_description_size(
     libbde_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_utf16_description(
     libbde_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_number_of_key_protectors(
     libbde_volume_t *volume,
     int *number_of_key_protectors,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_key_protector_by_index(
     libbde_volume_t *volume,
     int key_protector_index,
     libbde_key_protector_t **key_protector,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_key_protector(
     libbde_volume_t *volume,
     int key_protector_index,
     libbde_key_protector_t **key_protector,
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

#endif /* !defined( _LIBBDE_INTERNAL_VOLUME_H ) */

