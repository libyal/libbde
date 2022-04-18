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

#if !defined( _LIBBDE_SECTOR_DATA_H )
#define _LIBBDE_SECTOR_DATA_H

#include <common.h>
#include <types.h>

#include "libbde_encryption_context.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_sector_data libbde_sector_data_t;

struct libbde_sector_data
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libbde_sector_data_initialize(
     libbde_sector_data_t **sector_data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_sector_data_free(
     libbde_sector_data_t **sector_data,
     libcerror_error_t **error );

int libbde_sector_data_read_file_io_handle(
     libbde_sector_data_t *sector_data,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t sector_offset,
     libbde_encryption_context_t *encryption_context,
     uint8_t zero_metadata,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_SECTOR_DATA_H ) */

