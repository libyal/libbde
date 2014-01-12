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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libcthreads_condition.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_mutex.h"
#include "libcthreads_queue.h"
#include "libcthreads_types.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT )

/* Creates a queue
 * Make sure the value queue is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_initialize(
     libcthreads_queue_t **queue,
     int maximum_number_of_values,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_initialize";
	size_t values_array_size                     = 0;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid queue value already set.",
		 function );

		return( -1 );
	}
	if( maximum_number_of_values < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid maximum number of values value less than zero.",
		 function );

		return( -1 );
	}
#if SIZEOF_INT <= SIZEOF_SIZE_T
	if( (size_t) maximum_number_of_values > (size_t) ( SSIZE_MAX / sizeof( intptr_t * ) ) )
#else
	if( maximum_number_of_values > (int) ( SSIZE_MAX / sizeof( intptr_t * ) ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid maximum number of values value exceeds maximum.",
		 function );

		return( -1 );
	}
	values_array_size = sizeof( intptr_t * ) * maximum_number_of_values;

	if( values_array_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid values array size value exceeds maximum.",
		 function );

		goto on_error;
	}
	internal_queue = memory_allocate_structure(
	                  libcthreads_internal_queue_t );

	if( internal_queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create queue.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_queue,
	     0,
	     sizeof( libcthreads_internal_queue_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear queue.",
		 function );

		memory_free(
		 internal_queue );

		return( -1 );
	}
	internal_queue->values_array = (intptr_t **) memory_allocate(
	                                              values_array_size );

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_queue->values_array,
	     0,
	     values_array_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear values array.",
		 function );

		goto on_error;
	}
	internal_queue->allocated_number_of_values = maximum_number_of_values;

	if( libcthreads_mutex_initialize(
	     &( internal_queue->condition_mutex ),
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
	     &( internal_queue->empty_condition ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create empty condition.",
		 function );

		goto on_error;
	}
	if( libcthreads_condition_initialize(
	     &( internal_queue->full_condition ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create full condition.",
		 function );

		goto on_error;
	}
	*queue = (libcthreads_queue_t *) internal_queue;

	return( 1 );

on_error:
	if( internal_queue != NULL )
	{
		if( internal_queue->empty_condition != NULL )
		{
			libcthreads_condition_free(
			 &( internal_queue->empty_condition ),
			 NULL );
		}
		if( internal_queue->condition_mutex != NULL )
		{
			libcthreads_mutex_free(
			 &( internal_queue->condition_mutex ),
			 NULL );
		}
		if( internal_queue->values_array != NULL )
		{
			memory_free(
			 internal_queue->values_array );
		}
		memory_free(
		 internal_queue );
	}
	return( -1 );
}

/* Frees a queue
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_free(
     libcthreads_queue_t **queue,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_free";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	if( *queue != NULL )
	{
		internal_queue = (libcthreads_internal_queue_t *) *queue;
		*queue         = NULL;

		if( libcthreads_condition_free(
		     &( internal_queue->full_condition ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free full condition.",
			 function );

			result = -1;
		}
		if( libcthreads_condition_free(
		     &( internal_queue->empty_condition ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free empty condition.",
			 function );

			result = -1;
		}
		if( libcthreads_mutex_free(
		     &( internal_queue->condition_mutex ),
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
		 internal_queue );
	}
	return( result );
}

/* Empties a queue
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_empty(
     libcthreads_queue_t *queue,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_empty";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	internal_queue = (libcthreads_internal_queue_t *) queue;

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid queue - missing values array.",
		 function );

		return( -1 );
	}
	if( libcthreads_mutex_grab(
	     internal_queue->condition_mutex,
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
	while( internal_queue->number_of_values != 0 )
	{
		if( libcthreads_condition_wait(
		     internal_queue->full_condition,
		     internal_queue->condition_mutex,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to wait for full condition.",
			 function );

			result = -1;

			break;
		}
	}
	if( libcthreads_mutex_release(
	     internal_queue->condition_mutex,
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

/* Pops a value off the queue
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_pop(
     libcthreads_queue_t *queue,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_pop";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	internal_queue = (libcthreads_internal_queue_t *) queue;

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid queue - missing values array.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( libcthreads_mutex_grab(
	     internal_queue->condition_mutex,
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
	while( internal_queue->number_of_values == 0 )
	{
		if( libcthreads_condition_wait(
		     internal_queue->empty_condition,
		     internal_queue->condition_mutex,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to wait for empty condition.",
			 function );

			result = -1;

			break;
		}
	}
	if( result == 1 )
	{
		*value = internal_queue->values_array[ internal_queue->pop_index ];

		internal_queue->pop_index += 1;

		if( internal_queue->pop_index >= internal_queue->allocated_number_of_values )
		{
			internal_queue->pop_index = 0;
		}
		internal_queue->number_of_values -= 1;

		/* The condition broadcast must be protected by the mutex for the WINAPI version
		 */
		if( libcthreads_condition_broadcast(
		     internal_queue->full_condition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to broadcast full condition.",
			 function );

			result = -1;
		}
	}
	if( libcthreads_mutex_release(
	     internal_queue->condition_mutex,
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

/* Tries to pop a value off the queue
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libcthreads_queue_try_pop(
     libcthreads_queue_t *queue,
     intptr_t **value,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_try_pop";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	internal_queue = (libcthreads_internal_queue_t *) queue;

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid queue - missing values array.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( libcthreads_mutex_grab(
	     internal_queue->condition_mutex,
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
	if( internal_queue->number_of_values != 0 )
	{
		*value = internal_queue->values_array[ internal_queue->pop_index ];

		internal_queue->pop_index += 1;

		if( internal_queue->pop_index >= internal_queue->allocated_number_of_values )
		{
			internal_queue->pop_index = 0;
		}
		internal_queue->number_of_values -= 1;

		/* The condition broadcast must be protected by the mutex for the WINAPI version
		 */
		if( libcthreads_condition_broadcast(
		     internal_queue->full_condition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to broadcast full condition.",
			 function );

			result = -1;
		}
	}
	else
	{
		result = 0;
	}
	if( libcthreads_mutex_release(
	     internal_queue->condition_mutex,
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

/* Pushes a value onto the queue
 * Returns 1 if successful or -1 on error
 */
int libcthreads_queue_push(
     libcthreads_queue_t *queue,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_push";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	internal_queue = (libcthreads_internal_queue_t *) queue;

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid queue - missing values array.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( libcthreads_mutex_grab(
	     internal_queue->condition_mutex,
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
	while( internal_queue->number_of_values == internal_queue->allocated_number_of_values )
	{
		if( libcthreads_condition_wait(
		     internal_queue->full_condition,
		     internal_queue->condition_mutex,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to wait for full condition.",
			 function );

			result = -1;

			break;
		}
	}
	if( result == 1 )
	{
		internal_queue->values_array[ internal_queue->push_index ] = value;

		internal_queue->push_index += 1;

		if( internal_queue->push_index >= internal_queue->allocated_number_of_values )
		{
			internal_queue->push_index = 0;
		}
		internal_queue->number_of_values += 1;

		/* The condition broadcast must be protected by the mutex for the WINAPI version
		 */
		if( libcthreads_condition_broadcast(
		     internal_queue->empty_condition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to broadcast empty condition.",
			 function );

			result = -1;
		}
	}
	if( libcthreads_mutex_release(
	     internal_queue->condition_mutex,
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

/* Tries to push a value onto the queue
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libcthreads_queue_try_push(
     libcthreads_queue_t *queue,
     intptr_t *value,
     libcerror_error_t **error )
{
	libcthreads_internal_queue_t *internal_queue = NULL;
	static char *function                        = "libcthreads_queue_try_push";
	int result                                   = 1;

	if( queue == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid queue.",
		 function );

		return( -1 );
	}
	internal_queue = (libcthreads_internal_queue_t *) queue;

	if( internal_queue->values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid queue - missing values array.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( libcthreads_mutex_grab(
	     internal_queue->condition_mutex,
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
	if( internal_queue->number_of_values < internal_queue->allocated_number_of_values )
	{
		internal_queue->values_array[ internal_queue->push_index ] = value;

		internal_queue->push_index += 1;

		if( internal_queue->push_index >= internal_queue->allocated_number_of_values )
		{
			internal_queue->push_index = 0;
		}
		internal_queue->number_of_values += 1;

		/* The condition broadcast must be protected by the mutex for the WINAPI version
		 */
		if( libcthreads_condition_broadcast(
		     internal_queue->empty_condition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to broadcast empty condition.",
			 function );

			result = -1;
		}
	}
	else
	{
		result = 0;
	}
	if( libcthreads_mutex_release(
	     internal_queue->condition_mutex,
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

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) || defined( HAVE_MULTI_THREAD_SUPPORT ) */

