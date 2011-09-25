/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBBDE_IO_HANDLE_H )
#define _LIBBDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_array_type.h"
#include "libbde_encryption.h"
#include "libbde_libbfio.h"
#include "libbde_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char* bde_signature;

typedef struct libbde_io_handle libbde_io_handle_t;

struct libbde_io_handle
{
	/* The current (storage media) offset
	 */
	off64_t current_offset;

	/* The format version
	 */
	int version;

	/* The bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The sectors per cluster block
	 */
	uint8_t sectors_per_cluster_block;

	/* The first metadata offset
	 */
	off64_t first_metadata_offset;

	/* The second metadata offset
	 */
	off64_t second_metadata_offset;

	/* The third metadata offset
	 */
	off64_t third_metadata_offset;

	/* The metadata (block) size
	 */
	size64_t metadata_size;

	/* The volume size
	 */
	size64_t volume_size;

	/* The encrtypted volume size
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

	/* The encryption context
	 */
	libbde_encryption_context_t *encryption_context;

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

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libbde_io_handle_initialize(
     libbde_io_handle_t **io_handle,
     liberror_error_t **error );

int libbde_io_handle_free(
     libbde_io_handle_t **io_handle,
     liberror_error_t **error );

int libbde_io_handle_read_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error );

int libbde_io_handle_read_sector(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint8_t read_flags,
     liberror_error_t **error );

int libbde_io_handle_read_unencrypted_volume_header(
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

