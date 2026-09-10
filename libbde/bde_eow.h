/*
 * The BitLocker (BVE) Encrypt-on-Write (EOW) definitions
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _BDE_EOW_H )
#define _BDE_EOW_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct bde_eow_descriptor bde_eow_descriptor_t;

struct bde_eow_descriptor
{
	/* The signature
	 * Consists of 8 bytes
	 * Contains: FVE-EOW\x00
	 */
	uint8_t signature[ 8 ];

	/* The header size
	 * Consists of 2 bytes
	 */
	uint8_t header_size[ 2 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* The logical sector size
	 * Consists of 4 bytes
	 */
	uint8_t logical_sector_size[ 4 ];

	/* The physical sector size
	 * Consists of 4 bytes
	 */
	uint8_t physical_sector_size[ 4 ];

	/* The relocation block size
	 * Consists of 4 bytes
	 */
	uint8_t relocation_block_size[ 4 ];

	/* The relocation log area size
	 * Consists of 4 bytes
	 */
	uint8_t relocation_log_area_size[ 4 ];

	/* The relocation log entry size
	 * Consists of 4 bytes
	 */
	uint8_t relocation_log_entry_size[ 4 ];

	/* Number of block map area offsets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_offsets[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The first EOW descriptor offset
	 * Consists of 8 bytes
	 */
	uint8_t first_eow_descriptor_offset[ 8 ];

	/* The second EOW descriptor offset
	 * Consists of 8 bytes
	 */
	uint8_t second_eow_descriptor_offset[ 8 ];

	/* Block map area offsets
	 * Consists of 456 bytes
	 */
	uint8_t block_map_area_offsets[ 456 ];
};

typedef struct bde_eow_block_map bde_eow_block_map_t;

struct bde_eow_block_map
{
	/* The signature
	 * Consists of 10 bytes
	 * Contains: FVE-EOWBM\x00
	 */
	uint8_t signature[ 10 ];

	/* The header size
	 * Consists of 2 bytes
	 */
	uint8_t header_size[ 2 ];

	/* The block map size
	 * Consists of 4 bytes
	 */
	uint8_t block_map_size[ 4 ];

	/* The block map index
	 * Consists of 4 bytes
	 */
	uint8_t block_map_index[ 4 ];

	/* The volume region offset
	 * Consists of 8 bytes
	 */
	uint8_t volume_region_offset[ 8 ];

	/* The volume region size
	 * Consists of 8 bytes
	 */
	uint8_t volume_region_size[ 8 ];

	/* The relocation log area offset
	 * Consists of 8 bytes
	 */
	uint8_t relocation_log_area_offset[ 8 ];

	/* The first block record offset
	 * Consists of 4 bytes
	 */
	uint8_t first_block_record_offset[ 4 ];

	/* The second block record offset
	 * Consists of 4 bytes
	 */
	uint8_t second_block_record_offset[ 4 ];

	/* The block record size
	 * Consists of 4 bytes
	 */
	uint8_t block_record_size[ 4 ];

	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct bde_eow_block_record bde_eow_block_record_t;

struct bde_eow_block_record
{
	/* The signature
	 * Consists of 10 bytes
	 * Contains: FVE-EOWBR\x00
	 */
	uint8_t signature[ 10 ];

	/* The header size
	 * Consists of 2 bytes
	 */
	uint8_t header_size[ 2 ];

	/* The physical sector size
	 * Consists of 4 bytes
	 */
	uint8_t physical_sector_size[ 4 ];

	/* The number of bits
	 * Consists of 4 bytes
	 */
	uint8_t number_of_bits[ 4 ];

	/* The sequence number
	 * Consists of 4 bytes
	 */
	uint8_t sequence_number[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Flags
	 * Consists of 4 bytes
	 */
	uint8_t flags[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BDE_EOW_H ) */
