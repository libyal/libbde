/*
 * The internal type definitions
 *
 * Copyright (c) 2012-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCTHREADS_INTERNAL_TYPES_H )
#define _LIBCTHREADS_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBCTHREADS for local use of libcthreads
 * The definitions in <libcthreads/types.h> are copied here
 * for local use of libcthreads
 */
#if defined( HAVE_LOCAL_LIBCTHREADS ) && defined( HAVE_MULTI_THREAD_SUPPORT )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libcthreads_condition {}		libcthreads_condition_t;
typedef struct libcthreads_lock {}		libcthreads_lock_t;
typedef struct libcthreads_mutex {}		libcthreads_mutex_t;
typedef struct libcthreads_queue {}		libcthreads_queue_t;
typedef struct libcthreads_read_write_lock {}	libcthreads_read_write_lock_t;
typedef struct libcthreads_repeating_thread {}	libcthreads_repeating_thread_t;
typedef struct libcthreads_thread {}		libcthreads_thread_t;
typedef struct libcthreads_thread_attributes {}	libcthreads_thread_attributes_t;
typedef struct libcthreads_thread_pool {}	libcthreads_thread_pool_t;

#else
typedef intptr_t libcthreads_condition_t;
typedef intptr_t libcthreads_lock_t;
typedef intptr_t libcthreads_mutex_t;
typedef intptr_t libcthreads_queue_t;
typedef intptr_t libcthreads_read_write_lock_t;
typedef intptr_t libcthreads_repeating_thread_t;
typedef intptr_t libcthreads_thread_t;
typedef intptr_t libcthreads_thread_attributes_t;
typedef intptr_t libcthreads_thread_pool_t;

#endif

#endif

#endif

