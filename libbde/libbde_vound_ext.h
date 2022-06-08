/*
 * Volume functions
 *
 * Copyright (C) 2011-2019, Joachim Metz <joachim.metz@gmail.com>
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

#ifndef _LIBBDE_VOUND_EXT_H
#define _LIBBDE_VOUND_EXT_H

#include <common.h>
#include <types.h>

#include "libbde_extern.h"
#include "libcerror_types.h"
#include "libbde_key.h"

#if defined( __cplusplus )
extern "C" {
#endif



LIBBDE_EXTERN \
char libbde_vound_is_clear_key_present(libbde_volume_t* volume, libcerror_error_t** error);

extern \
int libbde_metadata_validate_recovery_master_key(libbde_volume_t* internal,libbde_key_t* volume_master_key,libcerror_error_t** error);


#if defined( __cplusplus )
}
#endif

#endif