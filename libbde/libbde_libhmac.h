/*
 * The internal libhmac header
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

#if !defined( _LIBBDE_LIBHMAC_H )
#define _LIBBDE_LIBHMAC_H

#include <common.h>

/* Define HAVE_LOCAL_LIBHMAC for local use of libhmac
 */
#if defined( HAVE_LOCAL_LIBHMAC )

#include <libhmac_definitions.h>
#include <libhmac_md5.h>
#include <libhmac_sha1.h>
#include <libhmac_sha256.h>
#include <libhmac_support.h>
#include <libhmac_types.h>

#else

/* If libtool DLL support is enabled set LIBHMAC_DLL_IMPORT
 * before including libhmac.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBHMAC_DLL_IMPORT
#endif

#include <libhmac.h>

#endif

#endif

