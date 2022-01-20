/*
 * External Key metadata entry functions
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

#if !defined( _LIBBDE_EXTERNAL_KEY_H )
#define _LIBBDE_EXTERNAL_KEY_H

#include <common.h>
#include <types.h>

#include "libbde_key.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"
#include "libbde_metadata_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_external_key libbde_external_key_t;

struct libbde_external_key
{
	/* The identifier
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

	/* The string metadata entry
	 */
	libbde_metadata_entry_t *string_entry;

	/* The key
	 */
	libbde_key_t *key;

	/* The metadata entries array
	 */
	libcdata_array_t *entries_array;
};

int libbde_external_key_initialize(
     libbde_external_key_t **external_key,
     libcerror_error_t **error );

int libbde_external_key_free(
     libbde_external_key_t **external_key,
     libcerror_error_t **error );

int libbde_external_key_read(
     libbde_external_key_t *external_key,
     libbde_metadata_entry_t *metadata_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_EXTERNAL_KEY_H ) */

