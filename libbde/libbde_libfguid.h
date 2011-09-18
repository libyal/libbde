/*
 * The libfguid header wrapper
 *
 * Copyright (C) 2011, Google Inc.
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

#if !defined( _LIBFBDE_LIBFGUID_H )
#define _LIBFBDE_LIBFGUID_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFGUID for local use of libfguid
 */
#if defined( HAVE_LOCAL_LIBFGUID )

#include <libfguid_definitions.h>
#include <libfguid_identifier.h>
#include <libfguid_types.h>

#elif defined( HAVE_LIBFGUID_H )

/* If libtool DLL support is enabled set LIBFGUID_DLL_IMPORT
 * before including libfguid.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFGUID_DLL_IMPORT
#endif

#include <libfguid.h>

#else
#error Missing libfguid.h
#endif

#endif

