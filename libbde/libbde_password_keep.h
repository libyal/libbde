/*
 * Password keep functions
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

#if !defined( _LIBBDE_PASSWORD_KEEP_H )
#define _LIBBDE_PASSWORD_KEEP_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_password_keep libbde_password_keep_t;

struct libbde_password_keep
{
	/* The SHA-256 hash of the password
	 */
	uint8_t password_hash[ 32 ];

	/* Value to indicate the password is set
	 */
	uint8_t password_is_set;

	/* The SHA-256 hash of the (binary) recovery password
	 */
	uint8_t recovery_password_hash[ 32 ];

	/* Value to indicate the recovery password is set
	 */
	uint8_t recovery_password_is_set;
};

int libbde_password_keep_initialize(
     libbde_password_keep_t **password_keep,
     libcerror_error_t **error );

int libbde_password_keep_free(
     libbde_password_keep_t **password_keep,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_PASSWORD_KEEP_H ) */

