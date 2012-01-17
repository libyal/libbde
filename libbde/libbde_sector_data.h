/*
 * Sector data functions
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

#if !defined( _LIBBDE_SECTOR_DATA_H )
#define _LIBBDE_SECTOR_DATA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_encryption.h"
#include "libbde_io_handle.h"
#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_sector_data libbde_sector_data_t;

struct libbde_sector_data
{
	/* The encrypted data
	 */
	uint8_t *encrypted_data;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libbde_sector_data_initialize(
     libbde_sector_data_t **sector_data,
     size_t data_size,
     liberror_error_t **error );

int libbde_sector_data_free(
     libbde_sector_data_t **sector_data,
     liberror_error_t **error );

int libbde_sector_data_read(
     libbde_sector_data_t *sector_data,
     libbde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t sector_offset,
     libbde_encryption_context_t *encryption_context,
     uint8_t zero_metadata,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

