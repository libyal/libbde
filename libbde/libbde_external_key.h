/*
 * External Key metadata entry functions
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

#if !defined( _LIBBDE_EXTERNAL_KEY_H )
#define _LIBBDE_EXTERNAL_KEY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_array_type.h"
#include "libbde_key.h"
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
	libbde_array_t *entries_array;
};

int libbde_external_key_initialize(
     libbde_external_key_t **external_key,
     liberror_error_t **error );

int libbde_external_key_free(
     libbde_external_key_t **external_key,
     liberror_error_t **error );

int libbde_external_key_read(
     libbde_external_key_t *external_key,
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

