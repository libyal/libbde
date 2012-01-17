/*
 * Volume Master Key (VMK) metadata entry functions
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

#if !defined( _LIBBDE_VOLUME_MASTER_KEY_H )
#define _LIBBDE_VOLUME_MASTER_KEY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_array_type.h"
#include "libbde_key.h"
#include "libbde_metadata_entry.h"
#include "libbde_stretch_key.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_volume_master_key libbde_volume_master_key_t;

struct libbde_volume_master_key
{
	/* The identifier
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

	/* The protection type
	 */
	uint16_t protection_type;

	/* The key
	 */
	libbde_key_t *key;

	/* The string metadata entry
	 */
	libbde_metadata_entry_t *string_entry;

	/* The stretch key
	 */
	libbde_stretch_key_t *stretch_key;

	/* The AES-CCM encrypted key
	 */
	libbde_aes_ccm_encrypted_key_t *aes_ccm_encrypted_key;

	/* The metadata entries array
	 */
	libbde_array_t *entries_array;
};

int libbde_volume_master_key_initialize(
     libbde_volume_master_key_t **volume_master_key,
     liberror_error_t **error );

int libbde_volume_master_key_free(
     libbde_volume_master_key_t **volume_master_key,
     liberror_error_t **error );

int libbde_volume_master_key_read(
     libbde_volume_master_key_t *volume_master_key,
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

