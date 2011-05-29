/*
 * Encryption functions
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

#if !defined( _LIBBDE_ENCRYPTION_H )
#define _LIBBDE_ENCRYPTION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

int libbde_encryption_aes_ccm_decrypt(
     const uint8_t *initialization_vector,
     size_t initialization_vector_size,
     const uint8_t *encrypted_data,
     size_t encrypted_data_size,
     const uint8_t *unencrypted_data,
     size_t unencrypted_data_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

