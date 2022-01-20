/*
 * Metadata block header functions
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

#if !defined( _LIBBDE_METADATA_BLOCK_HEADER_H )
#define _LIBBDE_METADATA_BLOCK_HEADER_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"
#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_metadata_block_header libbde_metadata_block_header_t;

struct libbde_metadata_block_header
{
	/* The format version
	 */
	uint16_t version;

	/* The encrypted volume size
	 */
	size64_t encrypted_volume_size;

	union
	{
		/* The MFT mirror cluster block number
		 * Used by Windows Vista (version 1)
		 */
		uint64_t mft_mirror_cluster_block_number;

		/* The volume header offset
		 * Used by Windows 7 (version 2)
		 */
		off64_t volume_header_offset;
	};

	/* The number of volume header sectors
	 * Used by Windows 7 (version 2)
	 */
	uint32_t number_of_volume_header_sectors;

	/* The first metadata offset
	 */
	uint64_t first_metadata_offset;

	/* The second metadata offset
	 */
	uint64_t second_metadata_offset;

	/* The third metadata offset
	 */
	uint64_t third_metadata_offset;
};

int libbde_metadata_block_header_initialize(
     libbde_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error );

int libbde_metadata_block_header_free(
     libbde_metadata_block_header_t **metadata_block_header,
     libcerror_error_t **error );

int libbde_metadata_block_header_read_data(
     libbde_metadata_block_header_t *metadata_block_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_metadata_block_header_read_file_io_handle(
     libbde_metadata_block_header_t *metadata_block_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_METADATA_BLOCK_HEADER_H ) */

