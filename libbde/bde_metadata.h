/*
 * The BitLocker (BVE) metadata definition
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

#if !defined( _BDE_METADATA_H )
#define _BDE_METADATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct bde_metadata_block_header_v1 bde_metadata_block_header_v1_t;

struct bde_metadata_block_header_v1
{
	/* The signature
	 * Consists of 8 bytes
	 * Contains: -FVE-FS-
	 */
	uint8_t signature[ 8 ];

	/* The size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The version
	 * Consists of 2 bytes
	 */
	uint8_t version[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* Unknown
	 * Consists of 16 bytes
	 */
	uint8_t unknown3[ 16 ];

	/* The first FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t first_metadata_offset[ 8 ];

	/* The second FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t second_metadata_offset[ 8 ];

	/* The third FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t third_metadata_offset[ 8 ];

	/* The MFT mirror cluster block number
	 * Consists of 8 bytes
	 */
	uint8_t mft_mirror_cluster_block[ 8 ];
};

typedef struct bde_metadata_block_header_v2 bde_metadata_block_header_v2_t;

struct bde_metadata_block_header_v2
{
	/* The signature
	 * Consists of 8 bytes
	 * Contains: -FVE-FS-
	 */
	uint8_t signature[ 8 ];

	/* The size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The version
	 * Consists of 2 bytes
	 */
	uint8_t version[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* The encrypted volume size
	 * Consists of 8 bytes
	 */
	uint8_t encrypted_volume_size[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* The number of volume header sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_volume_header_sectors[ 4 ];

	/* The first FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t first_metadata_offset[ 8 ];

	/* The second FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t second_metadata_offset[ 8 ];

	/* The third FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t third_metadata_offset[ 8 ];

	/* The volume header offset offset
	 * Consists of 8 bytes
	 */
	uint8_t volume_header_offset[ 8 ];
};

typedef struct bde_metadata_header_v1 bde_metadata_header_v1_t;

struct bde_metadata_header_v1
{
	/* The metadata size
	 * Consists of 4 bytes
	 */
	uint8_t metadata_size[ 4 ];

	/* The version
	 * Consists of 4 bytes
	 */
	uint8_t version[ 4 ];

	/* The metadata header size
	 * Consists of 4 bytes
	 */
	uint8_t metadata_header_size[ 4 ];

	/* The metadata size copy
	 * Consists of 4 bytes
	 */
	uint8_t metadata_size_copy[ 4 ];

	/* The volume identifier
	 * Consists of 16 bytes
	 */
	uint8_t volume_identifier[ 16 ];

	/* The next nonce counter
	 * Consists of 4 bytes
	 */
	uint8_t next_nonce_counter[ 4 ];

	/* The encryption method
	 * Consists of 2 bytes
	 */
	uint8_t encryption_method[ 2 ];

	/* The encryption method copy
	 * Consists of 2 bytes
	 */
	uint8_t encryption_method_copy[ 2 ];

	/* The creation date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t creation_time[ 8 ];
};

typedef struct bde_metadata_entry_v1 bde_metadata_entry_v1_t;

struct bde_metadata_entry_v1
{
	/* The size
	 * Consists of 2 bytes
	 */
	uint8_t size[ 2 ];

	/* The type
	 * Consists of 2 bytes
	 */
	uint8_t type[ 2 ];

	/* The value type
	 * Consists of 2 bytes
	 */
	uint8_t value_type[ 2 ];

	/* The version
	 * Consists of 2 bytes
	 */
	uint8_t version[ 2 ];

};

typedef struct bde_metadata_entry_key_header bde_metadata_entry_key_header_t;

struct bde_metadata_entry_key_header
{
	/* The encryption method
	 * Consists of 4 bytes
	 */
	uint8_t encryption_method[ 4 ];
};

typedef struct bde_metadata_entry_stretch_key_header bde_metadata_entry_stretch_key_header_t;

struct bde_metadata_entry_stretch_key_header
{
	/* The encryption method
	 * Consists of 4 bytes
	 */
	uint8_t encryption_method[ 4 ];

	/* The salt
	 * Consists of 16 bytes
	 */
	uint8_t salt[ 16 ];
};

typedef struct bde_metadata_entry_aes_ccm_encrypted_key_header bde_metadata_entry_aes_ccm_encrypted_key_header_t;

struct bde_metadata_entry_aes_ccm_encrypted_key_header
{
	/* The nonce date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t nonce_time[ 8 ];

	/* The nonce counter
	 * Consists of 4 bytes
	 */
	uint8_t nonce_counter[ 4 ];
};

typedef struct bde_metadata_entry_volume_master_key_header bde_metadata_entry_volume_master_key_header_t;

struct bde_metadata_entry_volume_master_key_header
{
	/* The identfier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

	/* The (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t modification_time[ 8 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The protection type
	 * Consists of 2 bytes
	 */
	uint8_t protection_type[ 2 ];
};

typedef struct bde_metadata_entry_external_key_header bde_metadata_entry_external_key_header_t;

struct bde_metadata_entry_external_key_header
{
	/* The identfier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

	/* The (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a filetime
	 */
	uint8_t modification_time[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BDE_METADATA_H ) */
