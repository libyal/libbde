/*
 * Volume Master Key (VMK) metadata entry functions
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

#if !defined( _LIBBDE_VOLUME_MASTER_KEY_H )
#define _LIBBDE_VOLUME_MASTER_KEY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_aes_ccm_encrypted_key.h"
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

	/* The type
	 */
	uint32_t type;

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

