/*
 * Thread attributes functions
 *
 * Copyright (C) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_libcerror.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates thread attributes
 * Make sure the value thread_attributes is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_attributes_initialize(
     libcthreads_thread_attributes_t **thread_attributes,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_attributes_t *internal_thread_attributes = NULL;
	static char *function                                                = "libcthreads_thread_attributes_initialize";

	if( thread_attributes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread attributes.",
		 function );

		return( -1 );
	}
	if( *thread_attributes != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread attributes value already set.",
		 function );

		return( -1 );
	}
	internal_thread_attributes = memory_allocate_structure(
	                              libcthreads_internal_thread_attributes_t );

	if( internal_thread_attributes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create thread attributes.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_thread_attributes,
	     0,
	     sizeof( libcthreads_internal_thread_attributes_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear thread attributes.",
		 function );

		goto on_error;
	}
/* TODO */
	*thread_attributes = (libcthreads_thread_attributes_t *) internal_thread_attributes;

	return( 1 );

on_error:
	if( internal_thread_attributes != NULL )
	{
		memory_free(
		 internal_thread_attributes );
	}
	return( -1 );
}

/* Frees thread attributes
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_attributes_free(
     libcthreads_thread_attributes_t **thread_attributes,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_attributes_t *internal_thread_attributes = NULL;
	static char *function                                                = "libcthreads_thread_attributes_free";
	int result                                                           = 1;

	if( thread_attributes == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread attributes.",
		 function );

		return( -1 );
	}
	if( *thread_attributes != NULL )
	{
		internal_thread_attributes = (libcthreads_internal_thread_attributes_t *) *thread_attributes;
		*thread_attributes         = NULL;

/* TODO */
		memory_free(
		 internal_thread_attributes );
	}
	return( result );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

