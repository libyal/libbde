/*
 * Password keep functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libbde_libcerror.h"
#include "libbde_password_keep.h"

/* Creates a password keep
 * Make sure the value password_keep is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_password_keep_initialize(
     libbde_password_keep_t **password_keep,
     libcerror_error_t **error )
{
	static char *function = "libbde_password_keep_initialize";

	if( password_keep == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pasword keep.",
		 function );

		return( -1 );
	}
	if( *password_keep != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid pasword keep value already set.",
		 function );

		return( -1 );
	}
	*password_keep = memory_allocate_structure(
	                  libbde_password_keep_t );

	if( *password_keep == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create pasword keep.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *password_keep,
	     0,
	     sizeof( libbde_password_keep_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear pasword keep.",
		 function );

		memory_free(
		 *password_keep );

		*password_keep = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *password_keep != NULL )
	{
		memory_free(
		 *password_keep );

		*password_keep = NULL;
	}
	return( -1 );
}

/* Frees a password keep
 * Returns 1 if successful or -1 on error
 */
int libbde_password_keep_free(
     libbde_password_keep_t **password_keep,
     libcerror_error_t **error )
{
	static char *function = "libbde_password_keep_free";
	int result            = 1;

	if( password_keep == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pasword keep.",
		 function );

		return( -1 );
	}
	if( *password_keep != NULL )
	{
		if( memory_set(
		     *password_keep,
		     0,
		     sizeof( libbde_password_keep_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear password keep.",
			 function );

			result = -1;
		}
		memory_free(
		 *password_keep );

		*password_keep = NULL;
	}
	return( result );
}

