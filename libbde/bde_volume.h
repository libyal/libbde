/*
 * The BitLocker disk encryption (BDE) volume definition
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

#if !defined( _BDE_VOLUME_H )
#define _BDE_VOLUME_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct bde_volume_header_windows_vista bde_volume_header_windows_vista_t;

struct bde_volume_header_windows_vista
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The signature
	 * Consists of 8 bytes
	 * Contains: -FVE-FS-
	 */
	uint8_t signature[ 8 ];

	/* The number of bytes per sector
	 * Consists of 2 bytes
	 */
	uint8_t bytes_per_sector[ 2 ];

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* Unknown
	 * Consists of 5 bytes
	 */
	uint8_t unknown1[ 5 ];

	/* The total number of sectors 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t total_number_of_sectors_16bit[ 2 ];

	/* The media descriptor
	 * Consists of 1 byte
	 */
	uint8_t media_descriptor;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* The number of sectors per track
	 * Consists of 2 bytes
	 */
	uint8_t sectors_per_track[ 2 ];

	/* The number of heads
	 * Consists of 2 bytes
	 */
	uint8_t number_of_heads[ 2 ];

	/* The number of hidden sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_hidden_sectors[ 4 ];

	/* The total number of sectors 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_sectors_32bit[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The total number of sectors
	 * Consists of 8 bytes
	 */
	uint8_t total_number_of_sectors_64bit[ 8 ];

	/* The master file table (MFT) cluster block number
	 * Consists of 8 bytes
	 */
	uint8_t mft_cluster_block_number[ 8 ];

	/* The first FVE metadata cluster block number
	 * Consists of 8 bytes
	 */
	uint8_t first_metadata_cluster_block_number[ 8 ];

	/* The MFT entry size
	 * Consists of 4 bytes
	 * Contains the number of cluster blocks or a byte size
	 */
	uint8_t mft_entry_size[ 4 ];

	/* The index entry size
	 * Consists of 4 bytes
	 * Contains the number of cluster blocks or a byte size
	 */
	uint8_t index_entry_size[ 4 ];

	/* The volume serial number
	 * Consists of 8 bytes
	 */
	uint8_t volume_serial_number[ 8 ];

	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The bootcode
	 * Consists of 426 bytes
	 */
	uint8_t bootcode[ 426 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

typedef struct bde_volume_header_windows_7 bde_volume_header_windows_7_t;

struct bde_volume_header_windows_7
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The signature
	 * Consists of 8 bytes
	 * Contains: -FVE-FS-
	 */
	uint8_t signature[ 8 ];

	/* The number of bytes per sector
	 * Consists of 2 bytes
	 */
	uint8_t bytes_per_sector[ 2 ];

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* Unknown
	 * Consists of 5 bytes
	 */
	uint8_t unknown1[ 5 ];

	/* The total number of sectors 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t total_number_of_sectors_16bit[ 2 ];

	/* The media descriptor
	 * Consists of 1 byte
	 */
	uint8_t media_descriptor;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* The number of sectors per track
	 * Consists of 2 bytes
	 */
	uint8_t sectors_per_track[ 2 ];

	/* The number of heads
	 * Consists of 2 bytes
	 */
	uint8_t number_of_heads[ 2 ];

	/* The number of hidden sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_hidden_sectors[ 4 ];

	/* The total number of sectors 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_sectors_32bit[ 4 ];

	/* Unknown
	 * Consists of 31 bytes
	 */
	uint8_t unknown4[ 31 ];

	/* The volume serial number
	 * Consists of 4 bytes
	 */
	uint8_t volume_serial_number[ 4 ];

	/* The volume label
	 * Consists of 11 bytes
	 */
	uint8_t volume_label[ 11 ];

	/* The file system signature
	 * Consists of 8 bytes
	 * Contains: "FAT32\x20\x20\x20"
	 */
	uint8_t file_system_signature[ 8 ];

	/* The bootcode
	 * Consists of 70 bytes
	 */
	uint8_t bootcode[ 70 ];

	/* The identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

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

	/* Unknown
	 * Consists of 310 bytes
	 */
	uint8_t unknown5[ 310 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

typedef struct bde_volume_header_to_go bde_volume_header_to_go_t;

struct bde_volume_header_to_go
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The signature
	 * Consists of 9 bytes
	 * Contains: MSWIN4.1
	 */
	uint8_t signature[ 8 ];

	/* The number of bytes per sector
	 * Consists of 2 bytes
	 */
	uint8_t bytes_per_sector[ 2 ];

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* Unknown
	 * Consists of 5 bytes
	 */
	uint8_t unknown1[ 5 ];

	/* The total number of sectors 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t total_number_of_sectors_16bit[ 2 ];

	/* The media descriptor
	 * Consists of 1 byte
	 */
	uint8_t media_descriptor;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* The number of sectors per track
	 * Consists of 2 bytes
	 */
	uint8_t sectors_per_track[ 2 ];

	/* The number of heads
	 * Consists of 2 bytes
	 */
	uint8_t number_of_heads[ 2 ];

	/* The number of hidden sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_hidden_sectors[ 4 ];

	/* The total number of sectors 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_sectors_32bit[ 4 ];

	/* Unknown
	 * Consists of 31 bytes
	 */
	uint8_t unknown4[ 31 ];

	/* The volume serial number
	 * Consists of 4 bytes
	 */
	uint8_t volume_serial_number[ 4 ];

	/* The volume label
	 * Consists of 11 bytes
	 */
	uint8_t volume_label[ 11 ];

	/* The file system signature
	 * Consists of 8 bytes
	 * Contains: "FAT32\x20\x20\x20"
	 */
	uint8_t file_system_signature[ 8 ];

	/* The bootcode
	 * Consists of 334 bytes
	 */
	uint8_t bootcode[ 334 ];

	/* The identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t identifier[ 16 ];

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

	/* Unknown
	 * Consists of 46 bytes
	 */
	uint8_t unknown5[ 46 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif
