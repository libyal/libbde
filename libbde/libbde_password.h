/*
 * Password functions
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

#if !defined( _LIBBDE_PASSWORD_H )
#define _LIBBDE_PASSWORD_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"
#include "libbde_libhmac.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_password_key_data libbde_password_key_data_t;

struct libbde_password_key_data
{
	/* The last calculated SHA256 hash
	 * This value contains the SHA256 hash calculated in the previous key iteration
	 */
	uint8_t last_sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	/* The initial calculated SHA256 hash
	 * This value contains the SHA256 hash calculated from the recovery password
	 */
	uint8_t initial_sha256_hash[ LIBHMAC_SHA256_HASH_SIZE ];

	/* The salt
	 */
	uint8_t salt[ 16 ];

	/* The key iteration count
	 */
	uint64_t iteration_count;
};

int libbde_utf8_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t *password_hash,
     size_t password_hash_size,
     libcerror_error_t **error );

int libbde_utf16_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t *password_hash,
     size_t password_hash_size,
     libcerror_error_t **error );

int libbde_password_calculate_key(
     const uint8_t *password_hash,
     size_t password_hash_size,
     const uint8_t *salt,
     size_t salt_size,
     uint8_t *key,
     size_t key_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_PASSWORD_H ) */

