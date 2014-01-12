/*
 * Mutex functions
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

#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates a mutex
 * Make sure the value mutex is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_initialize(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_initialize";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                             = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	if( *mutex != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mutex value already set.",
		 function );

		return( -1 );
	}
	internal_mutex = memory_allocate_structure(
	                  libcthreads_internal_mutex_t );

	if( internal_mutex == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mutex.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_mutex,
	     0,
	     sizeof( libcthreads_internal_mutex_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mutex.",
		 function );

		goto on_error;
	}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	InitializeCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( WINAPI )
	internal_mutex->mutex_handle = CreateMutex(
	                                NULL,
	                                FALSE,
	                                NULL );

	if( internal_mutex->mutex_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize mutex handle.",
		 function );

		goto on_error;
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_init(
	                  &( internal_mutex->mutex ),
	                  NULL );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize mutex.",
		 function );

		goto on_error;
	}
#endif
	*mutex = (libcthreads_mutex_t *) internal_mutex;

	return( 1 );

on_error:
	if( internal_mutex != NULL )
	{
		memory_free(
		 internal_mutex );
	}
	return( -1 );
}

/* Frees a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_free(
     libcthreads_mutex_t **mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_free";
	int result                                   = 1;

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                             = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	if( *mutex != NULL )
	{
		internal_mutex = (libcthreads_internal_mutex_t *) *mutex;
		*mutex         = NULL;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
		DeleteCriticalSection(
		 &( internal_mutex->critical_section ) );

#elif defined( WINAPI )
		if( CloseHandle(
		     internal_mutex->mutex_handle ) == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mutex handle.",
			 function );

			result = -1;
		}

#elif defined( HAVE_PTHREAD_H )
		pthread_result = pthread_mutex_destroy(
		                  &( internal_mutex->mutex ) );

		if( pthread_result != 0 )
		{
			switch( pthread_result )
			{
				case EBUSY:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy mutex with error: Resource busy.",
					 function );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 pthread_result,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy mutex.",
					 function );

					break;
			}
			result = -1;
		}
#endif
		memory_free(
		 internal_mutex );
	}
	return( result );
}

/* Grabs a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_grab(
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_grab";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                             = 0;
	DWORD wait_status                            = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	EnterCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( WINAPI )
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

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_lock(
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to lock mutex.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Tries to grabs a mutex
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libcthreads_mutex_try_grab(
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_try_grab";
	int result                                   = 1;

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                             = 0;
	DWORD wait_status                            = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	if( TryEnterCriticalSection(
	     &( internal_mutex->critical_section ) ) != 0 )
	{
		result = 1;
	}
	else
	{
		result = 0;
	}

#elif defined( WINAPI )
	wait_status = WaitForSingleObject(
	               internal_mutex->mutex_handle,
	               0 );

	if( wait_status == WAIT_TIMEOUT )
	{
		result = 0;
	}
	else if( wait_status == WAIT_FAILED )
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

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_trylock(
	                  &( internal_mutex->mutex ) );

	if( pthread_result == EBUSY )
	{
		result = 0;
	}
	else if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to lock mutex.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Releases a mutex
 * Returns 1 if successful or -1 on error
 */
int libcthreads_mutex_release(
     libcthreads_mutex_t *mutex,
     libcerror_error_t **error )
{
	libcthreads_internal_mutex_t *internal_mutex = NULL;
	static char *function                        = "libcthreads_mutex_release";

#if defined( WINAPI ) && ( WINVER < 0x0600 )
	DWORD error_code                             = 0;
	BOOL result                                  = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                           = 0;
#endif

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
	LeaveCriticalSection(
	 &( internal_mutex->critical_section ) );

#elif defined( WINAPI )
	result = ReleaseMutex(
	          internal_mutex->mutex_handle );

	if( result == 0 )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to relates mutex handle.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_mutex_unlock(
	                  &( internal_mutex->mutex ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to unlock mutex.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

