/*
 * Password functions
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

#if !defined( _LIBBDE_PASSWORD_H )
#define _LIBBDE_PASSWORD_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libhmac.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_password_key_data libbde_password_key_data_t;

struct libbde_password_key_data
{
	/* The salt
	 */
	uint8_t salt[ 16 ];

	/* The SHA256 hash
	 * This value contains the SHA256 hash calculated in the previous key iteration
	 */
	uint8_t sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];
};

int libbde_utf8_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t password_hash[ 32 ],
     liberror_error_t **error );

int libbde_utf16_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t password_hash[ 32 ],
     liberror_error_t **error );

int libbde_password_calculate_key(
     const uint8_t password_hash[ 32 ],
     const uint8_t salt[ 16 ],
     uint8_t key[ 32 ],
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

