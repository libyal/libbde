/*
 * Encrypt-on-Write (EOW) block record functions
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

#if !defined( _LIBBDE_EOW_BLOCK_RECORD_H )
#define _LIBBDE_EOW_BLOCK_RECORD_H

#include <common.h>
#include <types.h>

#include "libbde_libbfio.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_eow_block_record libbde_eow_block_record_t;

struct libbde_eow_block_record
{
	/* Number of bytes per bit in the bitmap
	 */
	uint32_t bytes_per_bit;

	/* The sequence number
	 */
	uint32_t sequence_number;

	/* The ranges array
	 */
	libcdata_array_t *ranges_array;
};

int libbde_eow_block_record_initialize(
     libbde_eow_block_record_t **eow_block_record,
     uint32_t bytes_per_bit,
     libcerror_error_t **error );

int libbde_eow_block_record_free(
     libbde_eow_block_record_t **eow_block_record,
     libcerror_error_t **error );

int libbde_eow_block_record_read_bitmap(
     libbde_eow_block_record_t *eow_block_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_eow_block_record_read_data(
     libbde_eow_block_record_t *eow_block_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_eow_block_record_read_file_io_handle(
     libbde_eow_block_record_t *eow_block_record,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t block_record_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_EOW_BLOCK_RECORD_H ) */

