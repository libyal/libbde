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

#if !defined( _LIBCTHREADS_INTERNAL_READ_WRITE_LOCK_H )
#define _LIBCTHREADS_INTERNAL_READ_WRITE_LOCK_H

#include <common.h>
#include <types.h>

#if defined( WINAPI ) && ( WINVER >= 0x0602 )
#include <Synchapi.h>
#endif

#if defined( HAVE_PTHREAD_H ) && !defined( WINAPI )
#include <pthread.h>
#endif

#include "libcthreads_extern.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

typedef struct libcthreads_internal_read_write_lock libcthreads_internal_read_write_lock_t;

struct libcthreads_internal_read_write_lock
{
#if defined( WINAPI ) && ( WINVER >= 0x0600 )
	/* The slim read/write lock
	 */
	SRWLOCK slim_read_write_lock;

#elif defined( WINAPI ) && ( WINVER > 0x0500 )
	/* The read critical section
	 */
	CRITICAL_SECTION read_critical_section;

	/* The write critical section
	 */
	CRITICAL_SECTION write_critical_section;

	/* The number of readers
	 */
	int number_of_readers;

	/* The no read event handle
	 */
	HANDLE no_read_event_handle;

#elif defined( WINAPI )

/* TODO */
#error WINAPI read/write lock for Windows 2000 or earlier NOT implemented yet

#elif defined( HAVE_PTHREAD_H )
	/* The read/write lock
	 */
	pthread_rwlock_t read_write_lock;

#else
#error Missing read/write lock type
#endif
};

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_initialize(
     libcthreads_read_write_lock_t **read_write_lock,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_free(
     libcthreads_read_write_lock_t **read_write_lock,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_grab_for_read(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_grab_for_write(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_release_for_read(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_read_write_lock_release_for_write(
     libcthreads_read_write_lock_t *read_write_lock,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

#if defined( __cplusplus )
}
#endif

#endif

