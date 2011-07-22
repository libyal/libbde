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

#if !defined( _LIBBDE_SECTOR_DATA_H )
#define _LIBBDE_SECTOR_DATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_encryption.h"
#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_sector_data libbde_sector_data_t;

struct libbde_sector_data
{
	/* The encrypted data
	 */
	uint8_t *encrypted_data;

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
     liberror_error_t **error );

int libbde_sector_data_free(
     libbde_sector_data_t *sector_data,
     liberror_error_t **error );

int libbde_sector_data_read(
     libbde_sector_data_t *sector_data,
     libbfio_handle_t *file_io_handle,
     off64_t sector_offset,
     libbde_encryption_context_t *encryption_context,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

