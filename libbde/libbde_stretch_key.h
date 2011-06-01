/*
 * Stretch Key metadata entry functions
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_STRETCH_KEY_H )
#define _LIBBDE_STRETCH_KEY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_io_handle.h"
#include "libbde_metadata_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_stretch_key libbde_stretch_key_t;

struct libbde_stretch_key
{
	/* The salt
	 */
	uint8_t salt[ 16 ];
};

int libbde_stretch_key_initialize(
     libbde_stretch_key_t **stretch_key,
     liberror_error_t **error );

int libbde_stretch_key_free(
     libbde_stretch_key_t **stretch_key,
     liberror_error_t **error );

int libbde_stretch_key_read(
     libbde_stretch_key_t *stretch_key,
     libbde_io_handle_t *io_handle,
     libbde_metadata_entry_t *metadata_entry,
     uint8_t use_recovery_password,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

