/*
 * The internal libcaes header
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

#if !defined( _LIBBDE_LIBCAES_H )
#define _LIBBDE_LIBCAES_H

#include <common.h>

/* Define HAVE_LOCAL_LIBCAES for local use of libcaes
 */
#if defined( HAVE_LOCAL_LIBCAES )

#include <libcaes_context.h>
#include <libcaes_definitions.h>
#include <libcaes_support.h>
#include <libcaes_types.h>

#elif defined( HAVE_LIBCAES_H )

/* If libtool DLL support is enabled set LIBCAES_DLL_IMPORT
 * before including libcaes.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBCAES_DLL_IMPORT
#endif

#include <libcaes.h>

#else
#error Missing libcaes.h
#endif

#endif

