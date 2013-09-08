/*
 * Thread pool functions
 *
 * Copyright (C) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCTHREADS_INTERNAL_THREAD_POOL_H )
#define _LIBCTHREADS_INTERNAL_THREAD_POOL_H

#include <common.h>
#include <types.h>

#if defined( WINAPI ) && ( WINVER >= 0x0602 )
#include <Threadpoolapiset.h>
#endif

#include "libcthreads_extern.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

typedef struct libcthreads_internal_thread_pool libcthreads_internal_thread_pool_t;

struct libcthreads_internal_thread_pool
{
#if defined( WINAPI ) && ( WINVER >= 0x0602 )
	/* The thread pool
	 */
	TP_POOL *thread_pool;

#else
	/* The number of threads in the pool
	 */
	int number_of_threads;

#if defined( WINAPI )
	/* The thread handles array
	 */
	HANDLE *thread_handles_array;

	/* The thread identifiers array
	 */
	DWORD *thread_identifiers_array;

#elif defined( HAVE_PTHREAD_H )
	/* The threads array
	 */
	pthread_t *threads_array;

#else
#error Missing thread type
#endif

	/* The callback function
	 */
	int (*callback_function)(
	       intptr_t *value,
	       void *arguments );

	/* The value function arguments
	 */
	void *callback_function_arguments;

	/* The (current) pop index
	 */
	int pop_index;

	/* The (current) push index
	 */
	int push_index;

	/* The number of values
	 */
	int number_of_values;

	/* The allocated number of values
	 */
	int allocated_number_of_values;

	/* The values array
	 */
	intptr_t **values_array;

	/* The condition mutex
	 */
	libcthreads_mutex_t *condition_mutex;

	/* The queue empty condition
	 */
	libcthreads_condition_t *empty_condition;

	/* The queue full condition
	 */
	libcthreads_condition_t *full_condition;

	/* The status
	 */
	uint8_t status;
#endif
};

LIBCTHREADS_EXTERN \
int libcthreads_thread_pool_create(
     libcthreads_thread_pool_t **thread_pool,
     const libcthreads_thread_attributes_t *thread_attributes,
     int number_of_threads,
     int maximum_number_of_values,
     int (*callback_function)(
            intptr_t *value,
            void *arguments ),
     void *callback_function_arguments,
     libcerror_error_t **error );

#if !( defined( WINAPI ) && ( WINVER >= 0x0602 ) )

int libcthreads_internal_thread_pool_pop(
     libcthreads_internal_thread_pool_t *internal_thread_pool,
     intptr_t **value,
     libcerror_error_t **error );

#endif

LIBCTHREADS_EXTERN \
int libcthreads_thread_pool_push(
     libcthreads_thread_pool_t *thread_pool,
     intptr_t *value,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_thread_pool_join(
     libcthreads_thread_pool_t **thread_pool,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

#if defined( __cplusplus )
}
#endif

#endif

