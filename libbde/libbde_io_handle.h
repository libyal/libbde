/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBBDE_IO_HANDLE_H )
#define _LIBBDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t *bde_signature;

typedef struct libbde_io_handle libbde_io_handle_t;

struct libbde_io_handle
{
	/* The format version
	 */
	int version;

	/* The first metadata offset
	 */
	off64_t first_metadata_offset;

	/* The second metadata offset
	 */
	off64_t second_metadata_offset;

	/* The third metadata offset
	 */
	off64_t third_metadata_offset;

	/* The encryption method
	 */
	int encryption_method;

	/* The (binary) recovery password
	 */
	uint16_t recovery_password[ 8 ];

	/* Value to indicate the recovery password is set
	 */
	uint8_t recovery_password_is_set;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libbde_io_handle_initialize(
     libbde_io_handle_t **io_handle,
     liberror_error_t **error );

int libbde_io_handle_free(
     libbde_io_handle_t **io_handle,
     liberror_error_t **error );

int libbde_io_handle_read_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error );

int libbde_io_handle_read_metadata(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

