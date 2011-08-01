/*
 * Metadata functions
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

#if !defined( _LIBBDE_METADATA_H )
#define _LIBBDE_METADATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_array_type.h"
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

	/* The volume identifier
	 * Contains a GUID
	 */
	uint8_t volume_identifier[ 16 ];

	/* The creation date and time
	 */
	uint64_t creation_time;

	/* The disk password protected volume master key
	 */
	libbde_volume_master_key_t *disk_password_volume_master_key;

	/* The external key protected volume master key
	 */
	libbde_volume_master_key_t *external_key_volume_master_key;

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

int libbde_metadata_read(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     liberror_error_t **error );

int libbde_metadata_get_volume_master_key(
     libbde_metadata_t *metadata,
     libbde_io_handle_t *io_handle,
     uint8_t key[ 32 ],
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

