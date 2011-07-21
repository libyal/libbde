/*
 * Encryption functions
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

#if !defined( _LIBBDE_ENCRYPTION_H )
#define _LIBBDE_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_aes.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_encryption_context libbde_encryption_context_t;

struct libbde_encryption_context
{
	/* The encryption method
	 */
	uint8_t encryption_method;

	/* The volume (AES) decryption context
	 */
	libbde_aes_context_t *volume_decryption_context;

	/* The volume (AES) encryption context
	 */
	libbde_aes_context_t *volume_encryption_context;
};

int libbde_encryption_initialize(
     libbde_encryption_context_t **context,
     uint8_t encryption_method,
     liberror_error_t **error );

int libbde_encryption_free(
     libbde_encryption_context_t **context,
     liberror_error_t **error );

int libbde_encryption_set_key(
     libbde_encryption_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

