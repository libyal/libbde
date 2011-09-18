/*
 * Metadata functions
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

#if !defined( _LIBBDE_METADATA_H )
#define _LIBBDE_METADATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_array_type.h"
#include "libbde_external_key.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_volume_master_key.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_metadata libbde_metadata_t;

struct libbde_metadata
{
	/* The format version
	 */
	uint16_t version;

	/* The encryption method
	 */
	uint32_t encryption_method;

	/* The encrypted volume size
	 */
	size64_t encrypted_volume_size;

	union
	{
		/* The MFT mirror cluster block number
		 * Used by Windows Vista (version 1)
		 */
		uint64_t mft_mirror_cluster_block_number;

		/* The volume header offset
		 * Used by Windows 7 (version 2)
		 */
		off64_t volume_header_offset;
	};

	/* The volume header size
	 * Used by Windows 7 (version 2)
	 */
	size64_t volume_header_size;

	/* The volume identifier
	 * Contains a GUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The creation date and time
	 */
	uint64_t creation_time;

	/* The startup key external key
	 */
	libbde_external_key_t *startup_key_external_key;

	/* The clear key protected volume master key
	 */
	libbde_volume_master_key_t *clear_key_volume_master_key;

	/* The startup key protected volume master key
	 */
	libbde_volume_master_key_t *startup_key_volume_master_key;

	/* The recovery password protected volume master key
	 */
	libbde_volume_master_key_t *recovery_password_volume_master_key;

	/* The password protected volume master key
	 */
	libbde_volume_master_key_t *password_volume_master_key;

	/* The full volume encryption key
	 */
	libbde_aes_ccm_encrypted_key_t *full_volume_encryption_key;

	/* The metadata entries array
	 */
	libbde_array_t *entries_array;
};

int libbde_metadata_initialize(
     libbde_metadata_t **metadata,
     liberror_error_t **error );

int libbde_metadata_free(
     libbde_metadata_t **metadata,
     liberror_error_t **error );

int libbde_metadata_read_block(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     const uint8_t *startup_key_identifier,
     size_t startup_key_identifier_size,
     liberror_error_t **error );

ssize_t libbde_metadata_read_header(
         libbde_metadata_t *metadata,
         uint8_t *header_data,
         size_t header_data_size,
         uint32_t *metadata_size,
         liberror_error_t **error );

ssize_t libbde_metadata_read_entries(
         libbde_metadata_t *metadata,
         uint8_t *entries_data,
         size_t entries_data_size,
         const uint8_t *startup_key_identifier,
         size_t startup_key_identifier_size,
         liberror_error_t **error );

int libbde_metadata_get_volume_master_key(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     const uint8_t *external_key,
     size_t external_key_size,
     uint8_t *volume_master_key,
     size_t volume_master_key_size,
     liberror_error_t **error );

int libbde_metadata_get_full_volume_encryption_key(
     libbde_metadata_t *metadata,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     uint8_t *full_volume_encryption_key,
     size_t full_volume_encryption_key_size,
     uint8_t *tweak_key,
     size_t tweak_key_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

