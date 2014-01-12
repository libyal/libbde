/*
 * Queue functions
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

#if !defined( _LIBCTHREADS_INTERNAL_QUEUE_H )
#define _LIBCTHREADS_INTERNAL_QUEUE_H

#include <common.h>
#include <types.h>

#include "libcthreads_extern.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

typedef struct libcthreads_internal_queue libcthreads_internal_queue_t;

struct libcthreads_internal_queue
{
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
};

LIBCTHREADS_EXTERN \
int libcthreads_queue_initialize(
     libcthreads_queue_t **queue,
     int maximum_number_of_values,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_empty(
     libcthreads_queue_t *queue,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_pop(
     libcthreads_queue_t *queue,
     intptr_t **value,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_try_pop(
     libcthreads_queue_t *queue,
     intptr_t **value,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_push(
     libcthreads_queue_t *queue,
     intptr_t *value,
     libcerror_error_t **error );

LIBCTHREADS_EXTERN \
int libcthreads_queue_try_push(
     libcthreads_queue_t *queue,
     intptr_t *value,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

#if defined( __cplusplus )
}
#endif

#endif

