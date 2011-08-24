/*
 * The BitLocker disk encryption (BDE) volume definition
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
	 * Consists of 7 bytes
	 */
	uint8_t unknown1[ 7 ];

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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The total number of sectors
	 * Consists of 8 bytes
	 */
	uint8_t total_number_of_sectors[ 8 ];

	/* The master file table (MFT) cluster block number
	 * Consists of 8 bytes
	 */
	uint8_t mft_cluster_block_number[ 8 ];

	/* The first FVE metadata cluster block number
	 * Consists of 8 bytes
	 */
	uint8_t first_metadata_cluster_block_number[ 8 ];
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
	 * Consists of 146 bytes
	 */
	uint8_t unknown1[ 146 ];

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
};

typedef struct bde_volume_header_togo bde_volume_header_togo_t;

struct bde_volume_header_togo
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

	/* Unknown
	 * Consists of 411 bytes
	 */
	uint8_t unknown1[ 411 ];

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
};

#if defined( __cplusplus )
}
#endif

#endif
