/*
 * Metadata functions
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

#if !defined( _LIBBDE_METADATA_H )
#define _LIBBDE_METADATA_H

#include <common.h>
#include <types.h>

#include "libbde_external_key.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"
#include "libbde_password_keep.h"
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

	/* The encrypted volume size
	 */
	size64_t encrypted_volume_size;

	/* The MFT mirror cluster block number
	 * Used by Windows Vista (version 1)
	 */
	uint64_t mft_mirror_cluster_block_number;

	/* The volume header offset
	 * Used by Windows 7 (version 2)
	 */
	off64_t volume_header_offset;

	/* The volume header size
	 * Used by Windows 7 (version 2)
	 */
	size64_t volume_header_size;

	/* The volume identifier
	 * Contains a GUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The encryption method
	 */
	uint16_t encryption_method;

	/* The creation date and time
	 */
	uint64_t creation_time;

	/* The description
	 */
	uint8_t *description;

	/* The description size
	 */
	size_t description_size;

	/* The startup key external key
	 */
	libbde_external_key_t *startup_key_external_key;

	/* The full volume encryption key
	 */
	libbde_aes_ccm_encrypted_key_t *full_volume_encryption_key;

	/* The metadata entries array
	 */
	libcdata_array_t *entries_array;

	/* The volume master keys array
	 */
	libcdata_array_t *volume_master_keys_array;

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
};

int libbde_metadata_initialize(
     libbde_metadata_t **metadata,
     libcerror_error_t **error );

int libbde_metadata_free(
     libbde_metadata_t **metadata,
     libcerror_error_t **error );

int libbde_metadata_read_block(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     const uint8_t *startup_key_identifier,
     size_t startup_key_identifier_size,
     libcerror_error_t **error );

int libbde_metadata_read_entries_file_io_handle(
     libbde_metadata_t *metadata,
     libbfio_handle_t *file_io_handle,
     size_t entries_data_size,
     const uint8_t *startup_key_identifier,
     size_t startup_key_identifier_size,
     libcerror_error_t **error );

int libbde_metadata_read_entries_data(
     libbde_metadata_t *metadata,
     uint8_t *entries_data,
     size_t entries_data_size,
     const uint8_t *startup_key_identifier,
     size_t startup_key_identifier_size,
     libcerror_error_t **error );

int libbde_metadata_read_volume_master_key(
     libbde_metadata_t *metadata,
     libbde_password_keep_t *password_keep,
     const uint8_t *external_key,
     size_t external_key_size,
     uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error );

int libbde_metadata_read_full_volume_encryption_key(
     libbde_metadata_t *metadata,
     uint16_t encryption_method,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     uint8_t *full_volume_encryption_key,
     size_t full_volume_encryption_key_size,
     uint8_t *tweak_key,
     size_t tweak_key_size,
     libcerror_error_t **error );

int libbde_metadata_get_volume_identifier(
     libbde_metadata_t *metadata,
     uint8_t *guid_data,
     size_t guid_data_size,
     libcerror_error_t **error );

int libbde_metadata_get_creation_time(
     libbde_metadata_t *metadata,
     uint64_t *filetime,
     libcerror_error_t **error );

int libbde_metadata_get_utf8_description_size(
     libbde_metadata_t *metadata,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libbde_metadata_get_utf8_description(
     libbde_metadata_t *metadata,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libbde_metadata_get_utf16_description_size(
     libbde_metadata_t *metadata,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libbde_metadata_get_utf16_description(
     libbde_metadata_t *metadata,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libbde_metadata_get_number_of_volume_master_keys(
     libbde_metadata_t *metadata,
     int *number_of_keys,
     libcerror_error_t **error );

int libbde_metadata_get_volume_master_key_by_index(
     libbde_metadata_t *metadata,
     int key_index,
     libbde_volume_master_key_t **key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_METADATA_H ) */

