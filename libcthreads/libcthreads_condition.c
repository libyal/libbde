/*
 * Condition functions
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

#include <errno.h>

#if defined( WINAPI ) && ( WINVER >= 0x0602 )
#include <Synchapi.h>
#endif

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_condition.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates a condition
 * Make sure the value condition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_initialize(
     libcthreads_condition_t **condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_initialize";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                                     = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	if( *condition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid condition value already set.",
		 function );

		return( -1 );
	}
	internal_condition = memory_allocate_structure(
	                      libcthreads_internal_condition_t );

	if( internal_condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create condition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_condition,
	     0,
	     sizeof( libcthreads_internal_condition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear condition.",
		 function );

		goto on_error;
	}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	InitializeConditionVariable(
	 &( internal_condition->condition_variable ) );

#elif defined( WINAPI )
	InitializeCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	internal_condition->signal_semaphore_handle = CreateSemaphore (
	                                               NULL,
	                                               0,
	                                               INT_MAX,
	                                               NULL );

	if( internal_condition->signal_semaphore_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize signal semaphore handle.",
		 function );

		goto on_error;
	}
	internal_condition->signal_event_handle = CreateEvent (
	                                           NULL,
	                                           FALSE,
	                                           FALSE,
	                                           NULL );

	if( internal_condition->signal_event_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize signal event handle.",
		 function );

		goto on_error;
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_init(
		          &( internal_condition->condition ),
	                  NULL );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize condition.",
		 function );

		goto on_error;
	}
#endif
	*condition = (libcthreads_condition_t *) internal_condition;

	return( 1 );

on_error:
	if( internal_condition != NULL )
	{
#if defined( WINAPI ) && ( WINVER < 0x0600 )
		if( internal_condition->signal_semaphore_handle != NULL )
		{
			CloseHandle(
			 internal_condition->signal_semaphore_handle );
		}
		DeleteCriticalSection(
		 &( internal_condition->wait_critical_section ) );
#endif
		memory_free(
		 internal_condition );
	}
	return( -1 );
}

/* Frees a condition
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_free(
     libcthreads_condition_t **condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_free";
	int result                                           = 1;

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                                     = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	if( *condition != NULL )
	{
		internal_condition = (libcthreads_internal_condition_t *) *condition;
		*condition         = NULL;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )

#elif defined( WINAPI )
		if( CloseHandle(
		     internal_condition->signal_event_handle ) == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free signal event handle.",
			 function );

			result = -1;
		}
		if( CloseHandle(
		     internal_condition->signal_semaphore_handle ) == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free signal semaphore handle.",
			 function );

			result = -1;
		}
		DeleteCriticalSection(
		 &( internal_condition->wait_critical_section ) );

#elif defined( HAVE_PTHREAD_H )
		pthread_result = pthread_cond_destroy(
		                  &( internal_condition->condition ) );

		if( pthread_result != 0 )
		{
			switch( pthread_result )
			{
				case EBUSY:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy condition with error: Resource busy.",
					 function );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 pthread_result,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy condition.",
					 function );

					break;
			}
			result = -1;
		}
#endif
		memory_free(
		 internal_condition );
	}
	return( result );
}

/* Broadcasts a condition
 * The of this function must be locked by the same mutex as used to wait
 * This is necessary for the WINAPI pre Vista (0x0600) implementation
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_broadcast(
     libcthreads_condition_t *condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_broadcast";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                                     = 0;
	DWORD wait_status                                    = 0;
	BOOL result                                          = 1;
	int number_of_waiting_threads                        = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	internal_condition = (libcthreads_internal_condition_t *) condition;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	WakeAllConditionVariable(
	 &( internal_condition->condition_variable ) );

#elif defined( WINAPI )
	EnterCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	number_of_waiting_threads = internal_condition->number_of_waiting_threads;

	if( number_of_waiting_threads > 0 )
	{
		internal_condition->signal_is_broadcast = 1;

		result = ReleaseSemaphore(
		          internal_condition->signal_semaphore_handle,
		          number_of_waiting_threads,
		          0 );

		if( result == 0 )
		{
			error_code = GetLastError();

			internal_condition->signal_is_broadcast = 0;
		}
	}
	LeaveCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	if( result == 0 )
	{
		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release signal semaphore handle.",
		 function );

		return( -1 );
	}
	if( number_of_waiting_threads > 0 )
	{
		wait_status = WaitForSingleObject(
		               internal_condition->signal_event_handle,
		               INFINITE );

		if( wait_status == WAIT_FAILED )
		{
			error_code = GetLastError();

			internal_condition->signal_is_broadcast = 0;

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: wait for no read event handle failed.",
			 function );

			return( -1 );
		}
		internal_condition->signal_is_broadcast = 0;
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_broadcast(
	                  &( internal_condition->condition ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to broadcast condition.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Signals a condition
 * The of this function must be locked by the same mutex as used to wait
 * This is necessary for the WINAPI pre Vista (0x0600) implementation
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_signal(
     libcthreads_condition_t *condition,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	static char *function                                = "libcthreads_condition_signal";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                                     = 0;
	BOOL result                                          = 1;
	int number_of_waiting_threads                        = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	internal_condition = (libcthreads_internal_condition_t *) condition;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	WakeConditionVariable(
	 &( internal_condition->condition_variable ) );

#elif defined( WINAPI )
	EnterCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	number_of_waiting_threads = internal_condition->number_of_waiting_threads;

	LeaveCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	if( number_of_waiting_threads > 0 )
	{
		result = ReleaseSemaphore(
		          internal_condition->signal_semaphore_handle,
		          1,
		          0 );

		if( result == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to release signal semaphore handle.",
			 function );

			return( -1 );
		}
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_signal(
	                  &( internal_condition->condition ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal condition.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Waits for a condition
 * Returns 1 if successful or -1 on error
 */
int libcthreads_condition_wait(
     libcthreads_condition_t *condition,
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_condition_t *internal_condition = NULL;
	libcthreads_internal_mutex_t *internal_mutex         = NULL;
	static char *function                                = "libcthreads_condition_wait";

#if defined( WINAPI )
	DWORD error_code                                     = 0;
	DWORD wait_status                                    = 0;

#if ( WINVER >= 0x0600 )
	BOOL result                                          = 0;
#else
	int is_last_waiting_thread                           = 0;
#endif

#elif defined( HAVE_PTHREAD_H )
	int pthread_result                                   = 0;
#endif

	if( condition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid condition.",
		 function );

		return( -1 );
	}
	internal_condition = (libcthreads_internal_condition_t *) condition;

	if( mutex == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mutex.",
		 function );

		return( -1 );
	}
	internal_mutex = (libcthreads_internal_mutex_t *) mutex;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	result = SleepConditionVariableCS(
	          &( internal_condition->condition_variable ),
	          &( internal_mutex->critical_section ),
	          INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to sleep on condition variable.",
		 function );

		return( -1 );
	}

#elif defined( WINAPI ) && ( WINVER >= 0x0400 )
	EnterCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	internal_condition->number_of_waiting_threads += 1;

	LeaveCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	wait_status = SignalObjectAndWait(
	               internal_mutex->mutex_handle,
	               internal_condition->signal_semaphore_handle,
	               INFINITE,
	               FALSE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable signal mutex handle and wait for signal semaphore handle.",
		 function );

		return( -1 );
	}
	EnterCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	internal_condition->number_of_waiting_threads -= 1;

	if( ( internal_condition->number_of_waiting_threads == 0 )
	 && ( internal_condition->signal_is_broadcast != 0 ) )
	{
		is_last_waiting_thread = 1;
	}
	LeaveCriticalSection(
	 &( internal_condition->wait_critical_section ) );

	if( is_last_waiting_thread != 0 )
	{
		wait_status = SignalObjectAndWait(
		               internal_condition->signal_event_handle,
		               internal_mutex->mutex_handle,
		               INFINITE,
		               FALSE );

		if( wait_status == WAIT_FAILED )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to signal signal event handle and wait for mutex handle.",
			 function );

			return( -1 );
		}
	}
	else
	{
		wait_status = WaitForSingleObject(
			       internal_mutex->mutex_handle,
			       INFINITE );

		if( wait_status == WAIT_FAILED )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: wait for mutex handle failed.",
			 function );

			return( -1 );
		}
	}

#elif defined( WINAPI )

#error libcthreads_condition_wait for Windows earlier than NT4 not implemented

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_cond_wait(
	                  &( internal_condition->condition ),
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to wait for condition.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

