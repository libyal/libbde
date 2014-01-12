/*
 * Thread functions
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

#if defined( WINAPI ) && ( WINVER >= 0x0602 ) && !defined( USE_CRT_FUNCTIONS )
#include <Processthreadsapi.h>
#include <Synchapi.h>
#endif

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_libcerror.h"
#include "libcthreads_thread.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

#if defined( WINAPI )
/* Start function helper function for WINAPI
 * Returns 0 if successful or 1 on error
 */
DWORD WINAPI libcthreads_thread_callback_function_helper(
              void *arguments )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	DWORD result                                   = 1;
	int callback_function_result                   = 0;

	if( arguments != NULL )
	{
		internal_thread = (libcthreads_internal_thread_t *) arguments;

		if( ( internal_thread != NULL )
		 && ( internal_thread->callback_function != NULL ) )
		{
			callback_function_result = internal_thread->callback_function(
			                            internal_thread->callback_function_arguments );
		}
		result = (DWORD) ( callback_function_result != 1 );
	}
	ExitThread(
	 result );
}

#elif defined( HAVE_PTHREAD_H )
/* Start function helper function for pthread
 * Returns a pointer to a newly allocated int containing 1 if successful or -1 on error
 * NULL is return if the helper function was unable to run the callback
 */
void *libcthreads_thread_callback_function_helper(
       void *arguments )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	int *result                                    = NULL;

	if( arguments != NULL )
	{
		internal_thread = (libcthreads_internal_thread_t *) arguments;

		if( ( internal_thread != NULL )
  		 && ( internal_thread->callback_function != NULL ) )
		{
			result = (int *) memory_allocate(
			                  sizeof( int ) );

			if( result != NULL )
			{
				*result = internal_thread->callback_function(
				           internal_thread->callback_function_arguments );
			}
		}
	}
	pthread_exit(
	 (void *) result );
}

#endif

/* Creates a thread
 * Make sure the value thread is referencing, is set to NULL
 *
 * The callback_function should return 1 if successful and -1 on error
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_create(
     libcthreads_thread_t **thread,
     const libcthreads_thread_attributes_t *thread_attributes,
     int (*callback_function)(
            void *arguments ),
     void *callback_function_arguments,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	static char *function                          = "libcthreads_thread_create";

#if defined( WINAPI )
	SECURITY_ATTRIBUTES *security_attributes       = NULL;
	HANDLE thread_handle                           = NULL;
	DWORD error_code                               = 0;

#elif defined( HAVE_PTHREAD_H )
	pthread_attr_t *attributes                     = NULL;
	int pthread_result                             = 0;
#endif

	if( thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread.",
		 function );

		return( -1 );
	}
	if( *thread != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid thread value already set.",
		 function );

		return( -1 );
	}
	if( callback_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid callback function.",
		 function );

		return( -1 );
	}
	internal_thread = memory_allocate_structure(
	                   libcthreads_internal_thread_t );

	if( internal_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create thread.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_thread,
	     0,
	     sizeof( libcthreads_internal_thread_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear thread.",
		 function );

		goto on_error;
	}
	internal_thread->callback_function           = callback_function;
	internal_thread->callback_function_arguments = callback_function_arguments;

#if defined( WINAPI )
	if( thread_attributes != NULL )
	{
		security_attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->security_attributes );
	}
	thread_handle = CreateThread(
	                 security_attributes,
	                 0, /* stack size */
	                 &libcthreads_thread_callback_function_helper,
	                 (void *) internal_thread,
	                 0, /* creation flags */
	                 &( internal_thread->thread_identifier ) );

	if( thread_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread handle.",
		 function );

		goto on_error;
	}
	internal_thread->thread_handle = thread_handle;

#elif defined( HAVE_PTHREAD_H )
	if( thread_attributes != NULL )
	{
		attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->attributes );
	}
	pthread_result = pthread_create(
		          &( internal_thread->thread ),
	                  attributes,
	                  &libcthreads_thread_callback_function_helper,
	                  (void *) internal_thread );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create thread.",
		 function );

		goto on_error;
	}
#endif
	*thread = (libcthreads_thread_t *) internal_thread;

	return( 1 );

on_error:
	if( internal_thread != NULL )
	{
		memory_free(
		 internal_thread );
	}
	return( -1 );
}

/* Joins the current with a specified thread
 * The the thread is freed after join
 * Returns 1 if successful or -1 on error
 */
int libcthreads_thread_join(
     libcthreads_thread_t **thread,
     libcerror_error_t **error )
{
	libcthreads_internal_thread_t *internal_thread = NULL;
	static char *function                          = "libcthreads_thread_join";
	int result                                     = 1;

#if defined( WINAPI )
	DWORD error_code                               = 0;
	DWORD wait_status                              = 0;

#elif defined( HAVE_PTHREAD_H )
	int *thread_return_value                       = NULL;
	int pthread_result                             = 0;
#endif

	if( thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid thread.",
		 function );

		return( -1 );
	}
	if( *thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing thread value.",
		 function );

		return( -1 );
	}
	internal_thread = (libcthreads_internal_thread_t *) *thread;
	*thread         = NULL;

#if defined( WINAPI )
	wait_status = WaitForSingleObject(
	               internal_thread->thread_handle,
	               INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: wait for thread failed.",
		 function );

		result = -1;
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_join(
	                  internal_thread->thread,
	                  (void **) &thread_return_value );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread.",
		 function );

		result = -1;
	}
	/* If the thread returns NULL it never got around to launching the callback function
	 */
	else if( ( thread_return_value != NULL )
	      && ( *thread_return_value != 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: thread returned an error status of: %d.",
		 function,
		 *thread_return_value );

		result = -1;
	}
	if( thread_return_value != NULL )
	{
		memory_free(
		 thread_return_value );

		thread_return_value = NULL;
	}
#endif
	memory_free(
	 internal_thread );

	return( result );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

