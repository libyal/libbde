/*
 * Error functions
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

#if !defined( _LIBBDE_INTERNAL_ERROR_H )
#define _LIBBDE_INTERNAL_ERROR_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#if !defined( HAVE_LOCAL_LIBBDE )
#include <libbde/error.h>
#endif

#include "libbde_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBBDE )

LIBBDE_EXTERN \
void libbde_error_free(
      libbde_error_t **error );

LIBBDE_EXTERN \
int libbde_error_fprint(
     libbde_error_t *error,
     FILE *stream );

LIBBDE_EXTERN \
int libbde_error_sprint(
     libbde_error_t *error,
     char *string,
     size_t size );

LIBBDE_EXTERN \
int libbde_error_backtrace_fprint(
     libbde_error_t *error,
     FILE *stream );

LIBBDE_EXTERN \
int libbde_error_backtrace_sprint(
     libbde_error_t *error,
     char *string,
     size_t size );

#endif

#if defined( __cplusplus )
}
#endif

#endif

