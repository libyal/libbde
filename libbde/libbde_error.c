/*
 * Error functions
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

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#include "libbde_error.h"

#if !defined( HAVE_LOCAL_LIBBDE )

/* Free an error and its elements
 */
void libbde_error_free(
      libbde_error_t **error )
{
	liberror_error_free(
	 (liberror_error_t **) error );
}

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
int libbde_error_fprint(
     libbde_error_t *error,
     FILE *stream )
{
	return( liberror_error_fprint(
	         (liberror_error_t *) error,
	         stream ) );
}

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
int libbde_error_sprint(
     libbde_error_t *error,
     char *string,
     size_t size )
{
	return( liberror_error_sprint(
	         (liberror_error_t *) error,
	         string,
	         size ) );
}

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
int libbde_error_backtrace_fprint(
     libbde_error_t *error,
      FILE *stream )
{
	return( liberror_error_backtrace_fprint(
	         (liberror_error_t *) error,
	         stream ) );
}

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
int libbde_error_backtrace_sprint(
     libbde_error_t *error,
     char *string,
     size_t size )
{
	return( liberror_error_backtrace_sprint(
	         (liberror_error_t *) error,
	         string,
	         size ) );
}

#endif

