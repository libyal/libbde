/*
 * Elephant diffuser encryption functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBDE_DIFFUSER_H )
#define _LIBBDE_DIFFUSER_H

#include <common.h>
#include <types.h>

#include "libbde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libbde_diffuser_decrypt(
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_diffuser_a_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error );

int libbde_diffuser_b_decrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error );

int libbde_diffuser_encrypt(
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libbde_diffuser_a_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error );

int libbde_diffuser_b_encrypt(
     uint32_t *values_32bit,
     size_t number_of_values,
     libcerror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif

