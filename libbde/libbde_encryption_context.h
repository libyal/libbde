/*
 * Encryption context functions
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

#if !defined( _LIBBDE_ENCRYPTION_CONTEXT_H )
#define _LIBBDE_ENCRYPTION_CONTEXT_H

#include <common.h>
#include <types.h>

#include "libbde_libcaes.h"
#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_encryption_context libbde_encryption_context_t;

struct libbde_encryption_context
{
	/* The encryption method
	 */
	uint16_t method;

	/* The FVEK (AES) decryption context
	 */
	libcaes_context_t *fvek_decryption_context;

	/* The FVEK (AES) encryption context
	 */
	libcaes_context_t *fvek_encryption_context;

	/* The TWEAK key (AES) decryption context
	 */
	libcaes_context_t *tweak_decryption_context;

	/* The TWEAK key (AES) encryption context
	 */
	libcaes_context_t *tweak_encryption_context;

	/* The FVEK (AES) decryption tweaked context
	 */
	libcaes_tweaked_context_t *fvek_decryption_tweaked_context;

	/* The FVEK (AES) encryption tweaked context
	 */
	libcaes_tweaked_context_t *fvek_encryption_tweaked_context;
};

int libbde_encryption_context_initialize(
     libbde_encryption_context_t **context,
     uint16_t method,
     libcerror_error_t **error );

int libbde_encryption_context_free(
     libbde_encryption_context_t **context,
     libcerror_error_t **error );

int libbde_encryption_context_set_keys(
     libbde_encryption_context_t *context,
     const uint8_t *key,
     size_t key_size,
     const uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error );

int libbde_encryption_context_crypt(
     libbde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t block_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_ENCRYPTION_CONTEXT_H ) */

