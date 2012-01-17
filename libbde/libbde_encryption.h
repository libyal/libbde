/*
 * Encryption functions
 *
 * Copyright (C) 2011-2012, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined( _LIBBDE_ENCRYPTION_H )
#define _LIBBDE_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libcaes.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBBDE_ENCYPTION_CRYPT_MODES
{
	LIBBDE_ENCYPTION_CRYPT_MODE_DECRYPT   = 0,
	LIBBDE_ENCYPTION_CRYPT_MODE_ENCRYPT   = 1
};

typedef struct libbde_encryption_context libbde_encryption_context_t;

struct libbde_encryption_context
{
	/* The encryption method
	 */
	uint32_t method;

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
};

int libbde_encryption_initialize(
     libbde_encryption_context_t **context,
     uint32_t method,
     liberror_error_t **error );

int libbde_encryption_free(
     libbde_encryption_context_t **context,
     liberror_error_t **error );

int libbde_encryption_set_keys(
     libbde_encryption_context_t *context,
     uint8_t full_volume_encryption_key[ 32 ],
     uint8_t tweak_key[ 32 ],
     liberror_error_t **error );

int libbde_encryption_crypt(
     libbde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     uint64_t block_key,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

