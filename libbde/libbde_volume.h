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

#if !defined( _LIBBDE_INTERNAL_VOLUME_H )
#define _LIBBDE_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_extern.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_internal_volume libbde_internal_volume_t;

struct libbde_internal_volume
{
	/* The (volume) size
	 */
	size64_t size;

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
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_free(
     libbde_volume_t **volume,
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_signal_abort(
     libbde_volume_t *volume,
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_open(
     libbde_volume_t *volume,
     const char *filename,
     int access_flags,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBBDE_EXTERN \
int libbde_volume_open_wide(
     libbde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     liberror_error_t **error );
#endif

LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_close(
     libbde_volume_t *volume,
     liberror_error_t **error );

int libbde_volume_open_read(
     libbde_internal_volume_t *internal_volume,
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_get_size(
     libbde_volume_t *volume,
     size64_t *size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

