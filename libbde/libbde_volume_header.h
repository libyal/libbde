/*
 * The BDE volume header functions
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

#if !defined( _LIBBDE_VOLUME_HEADER_H )
#define _LIBBDE_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libbde_libbfio.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_volume_header libbde_volume_header_t;

struct libbde_volume_header
{
	/* The format version
	 */
	int version;

	/* The bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The sectors per cluster block
	 */
	uint8_t sectors_per_cluster_block;

	/* The first metadata offset
	 */
	off64_t first_metadata_offset;

	/* The second metadata offset
	 */
	off64_t second_metadata_offset;

	/* The third metadata offset
	 */
	off64_t third_metadata_offset;

	/* The metadata (block) size
	 */
	size64_t metadata_size;

	/* The volume size
	 */
	size64_t volume_size;
};

int libbde_volume_header_initialize(
     libbde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libbde_volume_header_free(
     libbde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libbde_volume_header_read_data(
     libbde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_volume_header_read_file_io_handle(
     libbde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_VOLUME_HEADER_H ) */

