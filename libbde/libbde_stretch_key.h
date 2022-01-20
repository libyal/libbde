/*
 * Stretch Key metadata entry functions
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

#if !defined( _LIBBDE_STRETCH_KEY_H )
#define _LIBBDE_STRETCH_KEY_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"
#include "libbde_metadata_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_stretch_key libbde_stretch_key_t;

struct libbde_stretch_key
{
	/* The encryption method
	 */
	uint32_t encryption_method;

	/* The salt
	 */
	uint8_t salt[ 16 ];

	/* The (encrypted) data
	 */
	uint8_t *data;

	/* The (encrypted) data size
	 */
	size_t data_size;
};

int libbde_stretch_key_initialize(
     libbde_stretch_key_t **stretch_key,
     libcerror_error_t **error );

int libbde_stretch_key_free(
     libbde_stretch_key_t **stretch_key,
     libcerror_error_t **error );

int libbde_stretch_key_read(
     libbde_stretch_key_t *stretch_key,
     libbde_metadata_entry_t *metadata_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_STRETCH_KEY_H ) */

