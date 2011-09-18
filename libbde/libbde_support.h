/*
 * Support functions
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

#if !defined( _LIBBDE_SUPPORT_H )
#define _LIBBDE_SUPPORT_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libbde_extern.h"
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
     liberror_error_t **error );

LIBBDE_EXTERN \
int libbde_set_codepage(
     int codepage,
     liberror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBBDE ) */

LIBBDE_EXTERN \
int libbde_check_volume_signature(
     const char *volumename,
     liberror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )
LIBBDE_EXTERN \
int libbde_check_volume_signature_wide(
     const wchar_t *volumename,
     liberror_error_t **error );
#endif

LIBBDE_EXTERN \
int libbde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

