/*
 * File IO handle functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBFIO_FILE_IO_HANDLE_H )
#define _LIBBFIO_FILE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libbfio_libcerror.h"
#include "libbfio_libcfile.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbfio_file_io_handle libbfio_file_io_handle_t;

struct libbfio_file_io_handle
{
	/* The name
	 */
	system_character_t *name;

	/* The size of the name
	 */
	size_t name_size;

	/* The file (handle)
	 */
	libcfile_file_t *file;

	/* The access flags
	 */
	int access_flags;
};
__declspec(dllexport) \
int libbfio_file_io_handle_initialize(
     libbfio_file_io_handle_t **file_io_handle,
     libcerror_error_t **error );

__declspec(dllexport) \
int libbfio_file_io_handle_free(
     libbfio_file_io_handle_t **file_io_handle,
     libcerror_error_t **error );

__declspec(dllexport) \
int libbfio_file_io_handle_clone(
     libbfio_file_io_handle_t **destination_file_io_handle,
     libbfio_file_io_handle_t *source_file_io_handle,
     libcerror_error_t **error );

int libbfio_file_io_handle_get_name_size(
     libbfio_file_io_handle_t *file_io_handle,
     size_t *name_size,
     libcerror_error_t **error );

int libbfio_file_io_handle_get_name(
     libbfio_file_io_handle_t *file_io_handle,
     char *name,
     size_t name_size,
     libcerror_error_t **error );

int libbfio_file_io_handle_set_name(
     libbfio_file_io_handle_t *file_io_handle,
     const char *name,
     size_t name_length,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int libbfio_file_io_handle_get_name_size_wide(
     libbfio_file_io_handle_t *file_io_handle,
     size_t *name_size,
     libcerror_error_t **error );

int libbfio_file_io_handle_get_name_wide(
     libbfio_file_io_handle_t *file_io_handle,
     wchar_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libbfio_file_io_handle_set_name_wide(
     libbfio_file_io_handle_t *file_io_handle,
     const wchar_t *name,
     size_t name_length,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libbfio_file_io_handle_open(
     libbfio_file_io_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

int libbfio_file_io_handle_close(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error );

ssize_t libbfio_file_io_handle_read_buffer(
         libbfio_file_io_handle_t *file_io_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

ssize_t libbfio_file_io_handle_write_buffer(
         libbfio_file_io_handle_t *file_io_handle,
         const uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t libbfio_file_io_handle_seek_offset(
         libbfio_file_io_handle_t *file_io_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int libbfio_file_io_handle_exists(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error );

int libbfio_file_io_handle_is_open(
     libbfio_file_io_handle_t *file_io_handle,
     libcerror_error_t **error );

int libbfio_file_io_handle_get_size(
     libbfio_file_io_handle_t *file_io_handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBFIO_FILE_IO_HANDLE_H ) */

