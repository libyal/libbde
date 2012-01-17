/*
 * The libfcache header wrapper
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

#if !defined( _LIBBDE_LIBFCACHE_H )
#define _LIBBDE_LIBFCACHE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFCACHE for local use of libfcache
 *  */
#if defined( HAVE_LOCAL_LIBFCACHE )

#include <libfcache_cache.h>
#include <libfcache_date_time.h>
#include <libfcache_definitions.h>
#include <libfcache_types.h>

#elif defined( HAVE_LIBFCACHE_H )

/* If libtool DLL support is enabled set LIBFCACHE_DLL_IMPORT
 *  * before including libfcache.h
 *   */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFCACHE_DLL_IMPORT
#endif

#include <libfcache.h>

#else
#error Missing libfcache.h
#endif

#endif

