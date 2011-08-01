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

typedef struct bde_volume_header_v1 bde_volume_header_v1_t;

struct bde_volume_header_v1
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

	/* Unknown
	 * Consists of 43 bytes
	 */
	uint8_t unknown1[ 43 ];

	/* The first FVE metadata offset
	 * Consists of 8 bytes
	 */
	uint8_t first_metadata_offset[ 8 ];
};

typedef struct bde_volume_header_v2 bde_volume_header_v2_t;

struct bde_volume_header_v2
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

	/* Unknown
	 * Consists of 147 bytes
	 */
	uint8_t unknown1[ 147 ];

	/* The bitlocker identifier
	 * Consists of 16 bytes
	 * Contains a GUID
	 */
	uint8_t bitlocker_identifier[ 16 ];

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
