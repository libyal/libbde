/*
 * Repeating thread functions
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

#include "libcthreads_condition.h"
#include "libcthreads_definitions.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_repeating_thread.h"
#include "libcthreads_thread_attributes.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

#if defined( WINAPI )
/* Start function helper function for WINAPI
 * Returns 0 if successful or 1 on error
 */
DWORD WINAPI libcthreads_repeating_thread_start_function_helper(
              void *arguments )
{
	libcthreads_internal_repeating_thread_t *internal_repeating_thread = NULL;
	DWORD result                                                       = 1;
	int start_function_result                                          = 0;

	if( arguments != NULL )
	{
		internal_repeating_thread = (libcthreads_internal_repeating_thread_t *) arguments;

		if( ( internal_repeating_thread != NULL )
		 && ( internal_repeating_thread->start_function != NULL ) )
		{
			result = 0;

			do
			{
				libcthreads_mutex_grab(
				 internal_repeating_thread->condition_mutex,
				 NULL );

				/* Check for exit status in case of the situation where the thread
				 * is created after the join has been called
				 */
				while( internal_repeating_thread->status != LIBCTHREADS_STATUS_EXIT )
				{
					libcthreads_condition_wait(
					 internal_repeating_thread->status_condition,
					 internal_repeating_thread->condition_mutex,
					 NULL );
				}
				libcthreads_mutex_release(
				 internal_repeating_thread->condition_mutex,
				 NULL );

				start_function_result = internal_repeating_thread->start_function(
				                         internal_repeating_thread->start_function_arguments );

				if( ( start_function_result != 1 )
				 && ( result == 0 ) )
				{
					start_function_result = 0;
				}
			}
			while( internal_repeating_thread->status != LIBCTHREADS_STATUS_EXIT );
		}
	}
	ExitThread(
	 result );
}

#elif defined( HAVE_PTHREAD_H )
/* Start function helper function for pthread
 * Returns a pointer to the start function result if successful or NULL on error
 */
void *libcthreads_repeating_thread_start_function_helper(
       void *arguments )
{
	libcthreads_internal_repeating_thread_t *internal_repeating_thread = NULL;
	void *result                                                       = NULL;
	int start_function_result                                          = 0;

	if( arguments != NULL )
	{
		internal_repeating_thread = (libcthreads_internal_repeating_thread_t *) arguments;

		if( ( internal_repeating_thread != NULL )
  		 && ( internal_repeating_thread->start_function != NULL ) )
		{
			internal_repeating_thread->start_function_result = 1;

			do
			{
				libcthreads_mutex_grab(
				 internal_repeating_thread->condition_mutex,
				 NULL );

				/* Check for exit status in case of the situation where the thread
				 * is created after the join has been called
				 */
				while( internal_repeating_thread->status != LIBCTHREADS_STATUS_EXIT )
				{
					libcthreads_condition_wait(
					 internal_repeating_thread->status_condition,
					 internal_repeating_thread->condition_mutex,
					 NULL );
				}
				libcthreads_mutex_release(
				 internal_repeating_thread->condition_mutex,
				 NULL );

				start_function_result = internal_repeating_thread->start_function(
				                         internal_repeating_thread->start_function_arguments );

				if( ( start_function_result != 1 )
				 && ( internal_repeating_thread->start_function_result == 1 ) )
				{
					internal_repeating_thread->start_function_result = start_function_result;
				}
			}
			while( internal_repeating_thread->status != LIBCTHREADS_STATUS_EXIT );

			result = (void *) &( internal_repeating_thread->start_function_result );
		}
	}
	pthread_exit(
	 result );
}

#endif

/* Creates a repeating thread
 * Make sure the value repeating_thread is referencing, is set to NULL
 *
 * The start_function should return 1 if successful and -1 on error
 * Returns 1 if successful or -1 on error
 */
int libcthreads_repeating_thread_create(
     libcthreads_repeating_thread_t **repeating_thread,
     const libcthreads_thread_attributes_t *thread_attributes,
     int (*start_function)(
            void *arguments ),
     void *start_function_arguments,
     libcerror_error_t **error )
{
	libcthreads_internal_repeating_thread_t *internal_repeating_thread = NULL;
	static char *function                                              = "libcthreads_repeating_thread_create";

#if defined( WINAPI )
	SECURITY_ATTRIBUTES *security_attributes                           = NULL;
	DWORD error_code                                                   = 0;

#elif defined( HAVE_PTHREAD_H )
	pthread_attr_t *attributes                                         = NULL;
	int pthread_result                                                 = 0;
#endif

	if( repeating_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid repeating thread.",
		 function );

		return( -1 );
	}
	if( *repeating_thread != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid repeating thread value already set.",
		 function );

		return( -1 );
	}
	if( start_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid start function.",
		 function );

		return( -1 );
	}
	internal_repeating_thread = memory_allocate_structure(
	                             libcthreads_internal_repeating_thread_t );

	if( internal_repeating_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create repeating thread.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_repeating_thread,
	     0,
	     sizeof( libcthreads_internal_repeating_thread_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear repeating thread.",
		 function );

		goto on_error;
	}
	if( libcthreads_mutex_initialize(
	     &( internal_repeating_thread->condition_mutex ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create condition mutex.",
		 function );

		goto on_error;
	}
	if( libcthreads_condition_initialize(
	     &( internal_repeating_thread->status_condition ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create status condition.",
		 function );

		goto on_error;
	}
	internal_repeating_thread->start_function           = start_function;
	internal_repeating_thread->start_function_arguments = start_function_arguments;

#if defined( WINAPI )
	if( thread_attributes != NULL )
	{
		security_attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->security_attributes );
	}
	internal_repeating_thread->thread_handle = CreateThread(
	                                            security_attributes,
	                                            0, /* stack size */
	                                            &libcthreads_repeating_thread_start_function_helper,
	                                            (void *) internal_repeating_thread,
	                                            0, /* creation flags */
	                                            &( internal_repeating_thread->thread_identifier ) );

	if( internal_repeating_thread->thread_handle == NULL )
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
#elif defined( HAVE_PTHREAD_H )
	if( thread_attributes != NULL )
	{
		attributes = &( ( (libcthreads_internal_thread_attributes_t *) thread_attributes )->attributes );
	}
	pthread_result = pthread_create(
		          &( internal_repeating_thread->thread ),
	                  attributes,
	                  &libcthreads_repeating_thread_start_function_helper,
	                  (void *) internal_repeating_thread );

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
	*repeating_thread = (libcthreads_repeating_thread_t *) internal_repeating_thread;

	return( 1 );

on_error:
	if( internal_repeating_thread != NULL )
	{
		if( internal_repeating_thread->status_condition != NULL )
		{
			libcthreads_condition_free(
			 &( internal_repeating_thread->status_condition ),
			 NULL );
		}
		if( internal_repeating_thread->condition_mutex != NULL )
		{
			libcthreads_mutex_free(
			 &( internal_repeating_thread->condition_mutex ),
			 NULL );
		}
		memory_free(
		 internal_repeating_thread );
	}
	return( -1 );
}

/* Gives the thread a push
 * Returns 1 if successful or -1 on error
 */
int libcthreads_repeating_thread_push(
     libcthreads_repeating_thread_t *repeating_thread,
     libcerror_error_t **error )
{
	libcthreads_internal_repeating_thread_t *internal_repeating_thread = NULL;
	static char *function                                              = "libcthreads_repeating_thread_push";
	int result                                                         = 1;

	if( repeating_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid repeating thread.",
		 function );

		return( -1 );
	}
	internal_repeating_thread = (libcthreads_internal_repeating_thread_t *) repeating_thread;

/* TODO some work */

	if( libcthreads_mutex_grab(
	     internal_repeating_thread->condition_mutex,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab condition mutex.",
		 function );

		return( -1 );
	}
	if( libcthreads_condition_signal(
	     internal_repeating_thread->status_condition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal status condition.",
		 function );

		result = -1;
	}
	if( libcthreads_mutex_release(
	     internal_repeating_thread->condition_mutex,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release condition mutex.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Joins the current with a specified repeating thread
 * The the thread is freed after join
 * Returns 1 if successful or -1 on error
 */
int libcthreads_repeating_thread_join(
     libcthreads_repeating_thread_t **repeating_thread,
     libcerror_error_t **error )
{
	libcthreads_internal_repeating_thread_t *internal_repeating_thread = NULL;
	static char *function                                              = "libcthreads_repeating_thread_join";
	int result                                                         = 1;

#if defined( WINAPI )
	DWORD error_code                                                   = 0;
	DWORD wait_status                                                  = 0;

#elif defined( HAVE_PTHREAD_H )
	int *thread_return_value                                           = NULL;
	int pthread_result                                                 = 0;
#endif

	if( repeating_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid repeating thread.",
		 function );

		return( -1 );
	}
	if( *repeating_thread == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing repeating thread value.",
		 function );

		return( -1 );
	}
	internal_repeating_thread = (libcthreads_internal_repeating_thread_t *) *repeating_thread;
	*repeating_thread         = NULL;

	if( libcthreads_mutex_grab(
	     internal_repeating_thread->condition_mutex,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab condition mutex.",
		 function );

		return( -1 );
	}
	internal_repeating_thread->status = LIBCTHREADS_STATUS_EXIT;

	if( libcthreads_condition_signal(
	     internal_repeating_thread->status_condition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal status condition.",
		 function );

		result = -1;
	}
	if( libcthreads_mutex_release(
	     internal_repeating_thread->condition_mutex,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release condition mutex.",
		 function );

		return( -1 );
	}
	if( result != 1 )
	{
		return( -1 );
	}
#if defined( WINAPI )
	wait_status = WaitForSingleObject(
	               internal_repeating_thread->thread_handle,
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
	                  internal_repeating_thread->thread,
	                  (void **) &thread_return_value );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to join thread.",
		 function );

		result = -1;
	}
	else if( ( thread_return_value == NULL )
	      || ( thread_return_value != &( internal_repeating_thread->start_function_result ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: invalid thread return value.",
		 function );

		result = -1;
	}
	else if( *thread_return_value != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: thread returned an error status.",
		 function,
		 *thread_return_value );

		result = -1;
	}
#endif
	if( libcthreads_condition_free(
	     &( internal_repeating_thread->status_condition ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free status condition.",
		 function );

		result = -1;
	}
	if( libcthreads_mutex_free(
	     &( internal_repeating_thread->condition_mutex ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free condition mutex.",
		 function );

		result = -1;
	}
	memory_free(
	 internal_repeating_thread );

	return( result );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

