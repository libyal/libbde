/*
 * The libbfio header wrapper
 *
 * Copyright (C) 2011-2012, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined( _LIBBDE_LIBBFIO_H )
#define _LIBBDE_LIBBFIO_H

#include <common.h>

/* Define HAVE_LOCAL_LIBBFIO for local use of libbfio
 */
#if defined( HAVE_LOCAL_LIBBFIO )

#include <libbfio_definitions.h>
#include <libbfio_file.h>
#include <libbfio_file_pool.h>
#include <libbfio_file_range.h>
#include <libbfio_handle.h>
#include <libbfio_memory_range.h>
#include <libbfio_pool.h>
#include <libbfio_types.h>

#elif defined( HAVE_LIBBFIO_H )

/* If libtool DLL support is enabled set LIBBFIO_DLL_IMPORT
 * before including libbfio.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBBFIO_DLL_IMPORT
#endif

#include <libbfio.h>

#else
#error Missing libbfio.h
#endif

#endif

