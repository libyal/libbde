/*
 * Encrypt-on-Write (EOW) descriptor functions
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

#if !defined( _LIBBDE_EOW_DESCRIPTOR_H )
#define _LIBBDE_EOW_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libbde_libbfio.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_eow_descriptor libbde_eow_descriptor_t;

struct libbde_eow_descriptor
{
	/* The physical sector size
	 */
	uint32_t physical_sector_size;

	/* The relocation block size
	 */
	uint32_t relocation_block_size;

	/* The relocation log area size
	 */
	uint32_t relocation_log_area_size;

	/* The number of offsets
	 */
	uint32_t number_of_offsets;

	/* The block map area offsets
	 */
	off64_t offsets[ 64 ];
};

int libbde_eow_descriptor_initialize(
     libbde_eow_descriptor_t **eow_descriptor,
     libcerror_error_t **error );

int libbde_eow_descriptor_free(
     libbde_eow_descriptor_t **eow_descriptor,
     libcerror_error_t **error );

int libbde_eow_descriptor_read_data(
     libbde_eow_descriptor_t *eow_descriptor,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_eow_descriptor_read_file_io_handle(
     libbde_eow_descriptor_t *eow_descriptor,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_EOW_DESCRIPTOR_H ) */

