/*
 * Stretch Key metadata entry functions
 *
 * Copyright (C) 2011, Google Inc.
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

#if !defined( _LIBBDE_STRETCH_KEY_H )
#define _LIBBDE_STRETCH_KEY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

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
     liberror_error_t **error );

int libbde_stretch_key_free(
     libbde_stretch_key_t **stretch_key,
     liberror_error_t **error );

int libbde_stretch_key_read(
     libbde_stretch_key_t *stretch_key,
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

