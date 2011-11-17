/*
 * Metadata entry functions
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

#if !defined( _LIBBDE_METADATA_ENTRY_H )
#define _LIBBDE_METADATA_ENTRY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libbde_metadata_entry libbde_metadata_entry_t;

struct libbde_metadata_entry
{
	/* The (entry) type
	 */
	uint16_t type;

	/* The value type
	 */
	uint16_t value_type;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	uint16_t value_data_size;
};

int libbde_metadata_entry_initialize(
     libbde_metadata_entry_t **metadata_entry,
     liberror_error_t **error );

int libbde_metadata_entry_free(
     libbde_metadata_entry_t **metadata_entry,
     liberror_error_t **error );

ssize_t libbde_metadata_entry_read(
         libbde_metadata_entry_t *metadata_entry,
         const uint8_t *fve_metadata,
         size_t fve_metadata_size,
         liberror_error_t **error );

int libbde_metadata_entry_read_string(
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

