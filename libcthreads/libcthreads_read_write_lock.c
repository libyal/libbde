/*
 * Read/Write lock functions
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
#include "libcthreads_read_write_lock.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates a read/write lock
 * Make sure the value read_write_lock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_initialize(
     libcthreads_read_write_lock_t **read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_initialize";

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
	DWORD error_code                                                 = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	if( *read_write_lock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid read/write lock value already set.",
		 function );

		return( -1 );
	}
	internal_read_write_lock = memory_allocate_structure(
	                            libcthreads_internal_read_write_lock_t );

	if( internal_read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create read/write lock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_read_write_lock,
	     0,
	     sizeof( libcthreads_internal_read_write_lock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear read/write lock.",
		 function );

		memory_free(
		 internal_read_write_lock );

		return( -1 );
	}
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	InitializeSRWLock(
	 &( internal_read_write_lock->slim_read_write_lock ) );

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	InitializeCriticalSection(
	 &( internal_read_write_lock->write_critical_section ) );

	InitializeCriticalSection(
	 &( internal_read_write_lock->read_critical_section ) );

        internal_read_write_lock->no_read_event_handle = CreateEvent(
	                                                  NULL,
	                                                  TRUE,
	                                                  TRUE,
	                                                  NULL );

	if( internal_read_write_lock->no_read_event_handle == NULL )
	{
		error_code = GetLastError();

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize no read event handle.",
		 function );

		goto on_error;
	}
#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_rwlock_init(
		          &( internal_read_write_lock->read_write_lock ),
	                  NULL );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*read_write_lock = (libcthreads_read_write_lock_t *) internal_read_write_lock;

	return( 1 );

on_error:
	if( internal_read_write_lock != NULL )
	{
#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
		DeleteCriticalSection(
		 &( internal_read_write_lock->read_critical_section ) );

		DeleteCriticalSection(
		 &( internal_read_write_lock->write_critical_section ) );
#endif
		memory_free(
		 internal_read_write_lock );
	}
	return( -1 );
}

/* Frees a read/write lock
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_free(
     libcthreads_read_write_lock_t **read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_free";
	int result                                                       = 1;

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
	DWORD error_code                                                 = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	if( *read_write_lock != NULL )
	{
		internal_read_write_lock = (libcthreads_internal_read_write_lock_t *) *read_write_lock;
		*read_write_lock         = NULL;

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
		if( CloseHandle(
		     internal_read_write_lock->no_read_event_handle ) == 0 )
		{
			error_code = GetLastError();

			libcerror_system_set_error(
			 error,
			 error_code,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free no read event handle.",
			 function );

			result = -1;
		}
		DeleteCriticalSection(
		 &( internal_read_write_lock->read_critical_section ) );

		DeleteCriticalSection(
		 &( internal_read_write_lock->write_critical_section ) );

#elif defined( HAVE_PTHREAD_H )
		pthread_result = pthread_rwlock_destroy(
		                  &( internal_read_write_lock->read_write_lock ) );

		if( pthread_result != 0 )
		{
			switch( pthread_result )
			{
				case EBUSY:
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy read/write lock with error: Resource busy.",
					 function );

					break;

				default:
					libcerror_system_set_error(
					 error,
					 pthread_result,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to destroy read/write lock.",
					 function );

					break;
			}
			result = -1;
		}
#endif
		memory_free(
		 internal_read_write_lock );
	}
	return( result );
}

/* Grabs a read/write lock for reading
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_grab_for_read(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_grab_for_read";

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
	DWORD error_code                                                 = 0;
	BOOL result                                                      = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	internal_read_write_lock = (libcthreads_internal_read_write_lock_t *) read_write_lock;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	AcquireSRWLockShared(
	 &( internal_read_write_lock->slim_read_write_lock ) );

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	EnterCriticalSection(
	 &( internal_read_write_lock->write_critical_section ) );

	EnterCriticalSection(
	 &( internal_read_write_lock->read_critical_section ) );

	internal_read_write_lock->number_of_readers += 1;

	if( internal_read_write_lock->number_of_readers == 1 )
	{
		result = ResetEvent(
		          internal_read_write_lock->no_read_event_handle );

		if( result == 0 )
		{
			error_code = GetLastError();

			internal_read_write_lock->number_of_readers -= 1;
		}
	}
	else
	{
		result = 1;
	}
	LeaveCriticalSection(
	 &( internal_read_write_lock->read_critical_section ) );

	LeaveCriticalSection(
	 &( internal_read_write_lock->write_critical_section ) );

	if( result == 0 )
	{
		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: reset of no read event failed.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_rwlock_rdlock(
	                  &( internal_read_write_lock->read_write_lock ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to lock read/write lock for read.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Grabs a read/write lock for writing
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_grab_for_write(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_grab_for_write";

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
	DWORD error_code                                                 = 0;
	DWORD wait_status                                                = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	internal_read_write_lock = (libcthreads_internal_read_write_lock_t *) read_write_lock;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	AcquireSRWLockExclusive(
	 &( internal_read_write_lock->slim_read_write_lock ) );

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	EnterCriticalSection(
	 &( internal_read_write_lock->write_critical_section ) );

	wait_status = WaitForSingleObject(
	               internal_read_write_lock->no_read_event_handle,
	               INFINITE );

	if( wait_status == WAIT_FAILED )
	{
		error_code = GetLastError();

		LeaveCriticalSection(
		 &( internal_read_write_lock->write_critical_section ) );

		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: wait for no read event handle failed.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_rwlock_wrlock(
	                  &( internal_read_write_lock->read_write_lock ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to lock read/write lock for write.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Releases a read/write lock for reading
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_release_for_read(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_release_for_read";

#if defined( WINAPI ) && ( WINVER > 0x0500 ) && ( WINVER < 0x0600 )
	DWORD error_code                                                 = 0;
	BOOL result                                                      = 0;

#elif defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	internal_read_write_lock = (libcthreads_internal_read_write_lock_t *) read_write_lock;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	ReleaseSRWLockShared(
	 &( internal_read_write_lock->slim_read_write_lock ) );

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	EnterCriticalSection(
	 &( internal_read_write_lock->read_critical_section ) );

	internal_read_write_lock->number_of_readers -= 1;

	if( internal_read_write_lock->number_of_readers == 0 )
	{
		result = SetEvent(
		          internal_read_write_lock->no_read_event_handle );

		if( result == 0 )
		{
			error_code = GetLastError();

			internal_read_write_lock->number_of_readers += 1;
		}
	}
	else
	{
		result = 1;
	}
	LeaveCriticalSection(
	 &( internal_read_write_lock->read_critical_section ) );

	if( result == 0 )
	{
		libcerror_system_set_error(
		 error,
		 error_code,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: set of no read event failed.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_rwlock_unlock(
	                  &( internal_read_write_lock->read_write_lock ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to unlock read/write lock.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Releases a read/write lock for writing
 * Returns 1 if successful or -1 on error
 */
int libcthreads_read_write_lock_release_for_write(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error )
{
	libcthreads_internal_read_write_lock_t *internal_read_write_lock = NULL;
	static char *function                                            = "libcthreads_read_write_lock_release_for_write";

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
	int pthread_result                                               = 0;
#endif

	if( read_write_lock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read/write lock.",
		 function );

		return( -1 );
	}
	internal_read_write_lock = (libcthreads_internal_read_write_lock_t *) read_write_lock;

#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	ReleaseSRWLockExclusive(
	 &( internal_read_write_lock->slim_read_write_lock ) );

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	LeaveCriticalSection(
	 &( internal_read_write_lock->write_critical_section ) );

#elif defined( HAVE_PTHREAD_H )
	pthread_result = pthread_rwlock_unlock(
	                  &( internal_read_write_lock->read_write_lock ) );

	if( pthread_result != 0 )
	{
		libcerror_system_set_error(
		 error,
		 pthread_result,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to unlock read/write lock.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

