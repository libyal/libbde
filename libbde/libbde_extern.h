/*
 * The internal extern definition
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBBDE_INTERNAL_EXTERN_H )
#define _LIBBDE_INTERNAL_EXTERN_H

#include <common.h>

/* Define HAVE_LOCAL_LIBBDE for local use of libbde
 */
#if !defined( HAVE_LOCAL_LIBBDE )

/* If libtool DLL support is enabled set LIBBDE_DLL_EXPORT
 * before including libbde/extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBBDE_DLL_EXPORT
#endif

#include <libbde/extern.h>

#else
#define LIBBDE_EXTERN	/* extern */

#endif /* !defined( HAVE_LOCAL_LIBBDE ) */

#endif /* !defined( _LIBBDE_INTERNAL_EXTERN_H ) */

