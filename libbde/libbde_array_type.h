/* 
 * Array type functions
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

#if !defined( _LIBBDE_ARRAY_TYPE_H )
#define _LIBBDE_ARRAY_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The array comparison definitions
 */
enum LIBBDE_ARRAY_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBBDE_ARRAY_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBBDE_ARRAY_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBBDE_ARRAY_COMPARE_GREATER
};

/* The array insert flag definitions
 */
enum LIBBDE_ARRAY_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBBDE_ARRAY_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBBDE_ARRAY_INSERT_FLAG_UNIQUE_ENTRIES		= 0x01,
};

typedef struct libbde_array libbde_array_t;

struct libbde_array
{
	/* The number of allocated entries
	 */
	int number_of_allocated_entries;

	/* The number of entries
	 */
	int number_of_entries;

	/* The entries
	 */
	intptr_t **entries;
};

int libbde_array_initialize(
     libbde_array_t **array,
     int number_of_entries,
     liberror_error_t **error );

int libbde_array_free(
     libbde_array_t **array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libbde_array_empty(
     libbde_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libbde_array_clear(
     libbde_array_t *array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libbde_array_clone(
     libbde_array_t **destination_array,
     libbde_array_t *source_array,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     int (*entry_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libbde_array_resize(
     libbde_array_t *array,
     int number_of_entries,
     int (*entry_free_function)(
            intptr_t **entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libbde_array_get_number_of_entries(
     libbde_array_t *array,
     int *number_of_entries,
     liberror_error_t **error );

int libbde_array_get_entry_by_index(
     libbde_array_t *array,
     int entry_index,
     intptr_t **entry,
     liberror_error_t **error );

int libbde_array_set_entry_by_index(
     libbde_array_t *array,
     int entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libbde_array_append_entry(
     libbde_array_t *array,
     int *entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libbde_array_insert_entry(
     libbde_array_t *array,
     int *entry_index,
     intptr_t *entry,
     int (*entry_compare_function)(
            intptr_t *first_entry,
            intptr_t *second_entry,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

