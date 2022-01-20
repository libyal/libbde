/*
 * Metadata entry functions
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

#if !defined( _LIBBDE_METADATA_ENTRY_H )
#define _LIBBDE_METADATA_ENTRY_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t libbde_metadata_entry_empty[ 8 ];

typedef struct libbde_metadata_entry libbde_metadata_entry_t;

struct libbde_metadata_entry
{
	/* The (entry) type
	 */
	uint16_t type;

	/* The value type
	 */
	uint16_t value_type;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	uint16_t value_data_size;
};

int libbde_metadata_entry_initialize(
     libbde_metadata_entry_t **metadata_entry,
     libcerror_error_t **error );

int libbde_metadata_entry_free(
     libbde_metadata_entry_t **metadata_entry,
     libcerror_error_t **error );

ssize_t libbde_metadata_entry_read(
         libbde_metadata_entry_t *metadata_entry,
         const uint8_t *fve_metadata,
         size_t fve_metadata_size,
         libcerror_error_t **error );

int libbde_metadata_entry_read_string(
     libbde_metadata_entry_t *metadata_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_METADATA_ENTRY_H ) */

