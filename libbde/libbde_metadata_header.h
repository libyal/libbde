/*
 * Metadata header functions
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

#if !defined( _LIBBDE_METADATA_HEADER_H )
#define _LIBBDE_METADATA_HEADER_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"
#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_metadata_header libbde_metadata_header_t;

struct libbde_metadata_header
{
	/* The metadata size
	 */
	uint32_t metadata_size;

	/* The volume identifier
	 * Contains a GUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The encryption method
	 */
	uint16_t encryption_method;

	/* The creation date and time
	 */
	uint64_t creation_time;
};

int libbde_metadata_header_initialize(
     libbde_metadata_header_t **metadata_header,
     libcerror_error_t **error );

int libbde_metadata_header_free(
     libbde_metadata_header_t **metadata_header,
     libcerror_error_t **error );

int libbde_metadata_header_read_data(
     libbde_metadata_header_t *metadata_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_metadata_header_read_file_io_handle(
     libbde_metadata_header_t *metadata_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_METADATA_HEADER_H ) */

