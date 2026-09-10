/*
 * Sector range functions
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

#if !defined( _LIBBDE_SECTOR_RANGE_H )
#define _LIBBDE_SECTOR_RANGE_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_sector_range libbde_sector_range_t;

struct libbde_sector_range
{
	/* The start offset
	 */
	off64_t start_offset;

	/* The end offset
	 */
	off64_t end_offset;

	/* The value
	 */
	uint8_t value;
};

int libbde_sector_range_initialize(
     libbde_sector_range_t **sector_range,
     libcerror_error_t **error );

int libbde_sector_range_free(
     libbde_sector_range_t **sector_range,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_SECTOR_RANGE_H ) */

