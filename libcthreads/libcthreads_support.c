/*
 * Support functions
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

#include <common.h>
#include <types.h>

#include "libcthreads_definitions.h"
#include "libcthreads_libcerror.h"
#include "libcthreads_libcstring.h"
#include "libcthreads_support.h"

#if !defined( HAVE_LOCAL_LIBCTHREADS )

/* Returns the library version as a string
 */
const char *libcthreads_get_version(
             void )
{
	return( (const char *) LIBCTHREADS_VERSION_STRING );
}

#endif /* !defined( HAVE_LOCAL_LIBCTHREADS ) */

