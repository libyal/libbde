/*
 * The internal definitions
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

#if !defined( LIBCTHREADS_INTERNAL_DEFINITIONS_H )
#define LIBCTHREADS_INTERNAL_DEFINITIONS_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBCTHREADS for local use of libcthreads
 */
#if !defined( HAVE_LOCAL_LIBCTHREADS )
#include <libcthreads/definitions.h>

/* The definitions in <libcthreads/definitions.h> are copied here
 * for local use of libcthreads
 */
#else

#define LIBCTHREADS_VERSION			20140505

/* The libcthreads version string
 */
#define LIBCTHREADS_VERSION_STRING		"20140505"

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) */

/* The status definitions
 */
enum LIBCTHREADS_STATUS
{
	LIBCTHREADS_STATUS_EXIT			= 1
};

#endif

