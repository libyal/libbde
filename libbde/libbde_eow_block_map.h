/*
 * Encrypt-on-Write (EOW) block map functions
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

#if !defined( _LIBBDE_EOW_BLOCK_MAP_H )
#define _LIBBDE_EOW_BLOCK_MAP_H

#include <common.h>
#include <types.h>

#include "libbde_libbfio.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_eow_block_map libbde_eow_block_map_t;

struct libbde_eow_block_map
{
	/* The block map size
	 */
	uint32_t block_map_size;

	/* The volume region offset
	 */
	uint64_t volume_region_offset;

	/* The volume region size
	 */
	uint64_t volume_region_size;

	/* The relocation log area offset
	 */
	uint64_t relocation_log_area_offset;

	/* The first block record offset
	 */
	uint32_t first_block_record_offset;

	/* The second block record offset
	 */
	uint32_t second_block_record_offset;

	/* The block record size
	 */
	uint32_t block_record_size;
};

int libbde_eow_block_map_initialize(
     libbde_eow_block_map_t **eow_block_map,
     libcerror_error_t **error );

int libbde_eow_block_map_free(
     libbde_eow_block_map_t **eow_block_map,
     libcerror_error_t **error );

int libbde_eow_block_map_read_data(
     libbde_eow_block_map_t *eow_block_map,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_eow_block_map_read_file_io_handle(
     libbde_eow_block_map_t *eow_block_map,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_map_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_EOW_BLOCK_MAP_H ) */

