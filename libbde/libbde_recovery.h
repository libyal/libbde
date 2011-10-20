/*
 * Recovery functions
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

#if !defined( _LIBBDE_RECOVERY_H )
#define _LIBBDE_RECOVERY_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_libhmac.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libbde_utf8_recovery_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     liberror_error_t **error );

int libbde_utf16_recovery_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

