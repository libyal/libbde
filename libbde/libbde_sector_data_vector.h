/*
 * Sector data vector functions
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

#if !defined( _LIBBDE_SECTOR_DATA_VECTOR_H )
#define _LIBBDE_SECTOR_DATA_VECTOR_H

#include <common.h>
#include <types.h>

#include "libbde_encryption_context.h"
#include "libbde_io_handle.h"
#include "libbde_libcerror.h"
#include "libbde_libbfio.h"
#include "libbde_libfcache.h"
#include "libbde_sector_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_sector_data_vector libbde_sector_data_vector_t;

struct libbde_sector_data_vector
{
	/* The encryption context
	 */
	libbde_encryption_context_t *encryption_context;

	/* The cache timestamp
	 */
	int64_t cache_timestamp;

	/* The sector data cache
	 */
	libfcache_cache_t *cache;

	/* The number of bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The total data size
	 */
	size64_t data_size;
};

int libbde_sector_data_vector_initialize(
     libbde_sector_data_vector_t **sector_data_vector,
     libbde_encryption_context_t *encryption_context,
     uint16_t bytes_per_sector,
     size64_t data_size,
     libcerror_error_t **error );

int libbde_sector_data_vector_free(
     libbde_sector_data_vector_t **sector_data_vector,
     libcerror_error_t **error );

int libbde_sector_data_vector_get_sector_data_at_offset(
     libbde_sector_data_vector_t *sector_data_vector,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t offset,
     libbde_sector_data_t **sector_data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_SECTOR_DATA_VECTOR_H ) */

