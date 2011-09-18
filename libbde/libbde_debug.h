/*
 * Debug functions
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

#if !defined( _LIBBDE_DEBUG_H )
#define _LIBBDE_DEBUG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

const char *libbde_debug_print_encryption_method(
             uint32_t encryption_method );

const char *libbde_debug_print_entry_type(
             uint16_t entry_type );

const char *libbde_debug_print_value_type(
             uint16_t value_type );

const char *libbde_debug_print_key_protection_type(
             uint32_t key_protection_type );

int libbde_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

