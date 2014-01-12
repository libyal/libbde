/*
 * Error functions
 *
 * Copyright (c) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCTHREADS_INTERNAL_ERROR_H )
#define _LIBCTHREADS_INTERNAL_ERROR_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#if !defined( HAVE_LOCAL_LIBCTHREADS )
#include <libcthreads/error.h>
#endif

#include "libcthreads_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCTHREADS )

LIBCTHREADS_EXTERN \
void libcthreads_error_free(
      libcthreads_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_error_fprint(
     libcthreads_error_t *error,
     FILE *stream );

LIBCTHREADS_EXTERN \
int libcthreads_error_sprint(
     libcthreads_error_t *error,
     char *string,
     size_t size );

LIBCTHREADS_EXTERN \
int libcthreads_error_backtrace_fprint(
     libcthreads_error_t *error,
     FILE *stream );

LIBCTHREADS_EXTERN \
int libcthreads_error_backtrace_sprint(
     libcthreads_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) */

#if defined( __cplusplus )
}
#endif

#endif

