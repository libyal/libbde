/*
 * Elephant diffuser encryption functions
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

#if !defined( _LIBBDE_DIFFUSER_H )
#define _LIBBDE_DIFFUSER_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

int libbde_diffuser_decrypt(
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error );

int libbde_diffuser_a_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     liberror_error_t **error );

int libbde_diffuser_b_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     liberror_error_t **error );

int libbde_diffuser_encrypt(
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error );

int libbde_diffuser_a_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     liberror_error_t **error );

int libbde_diffuser_b_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     liberror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif

