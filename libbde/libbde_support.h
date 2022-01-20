/*
 * Support functions
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

#if !defined( _LIBBDE_SUPPORT_H )
#define _LIBBDE_SUPPORT_H

#include <common.h>
#include <types.h>

#include "libbde_extern.h"
#include "libbde_libcerror.h"
#include "libbde_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBBDE )

LIBBDE_EXTERN \
const char *libbde_get_version(
             void );

LIBBDE_EXTERN \
int libbde_get_access_flags_read(
     void );

LIBBDE_EXTERN \
int libbde_get_codepage(
     int *codepage,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_set_codepage(
     int codepage,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBBDE ) */

LIBBDE_EXTERN \
int libbde_check_volume_signature(
     const char *volumename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBBDE_EXTERN \
int libbde_check_volume_signature_wide(
     const wchar_t *volumename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBBDE_EXTERN \
int libbde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_SUPPORT_H ) */

