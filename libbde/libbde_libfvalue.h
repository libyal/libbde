/*
 * The libfvalue header wrapper
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

#if !defined( _LIBBDE_LIBFVALUE_H )
#define _LIBBDE_LIBFVALUE_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFVALUE for local use of libfvalue
 */
#if defined( HAVE_LOCAL_LIBFVALUE )

#include <libfvalue_codepage.h>
#include <libfvalue_definitions.h>
#include <libfvalue_floating_point.h>
#include <libfvalue_integer.h>
#include <libfvalue_split_string.h>
#include <libfvalue_string.h>
#include <libfvalue_table.h>
#include <libfvalue_types.h>
#include <libfvalue_value.h>
#include <libfvalue_value_type.h>

#else

/* If libtool DLL support is enabled set LIBFVALUE_DLL_IMPORT
 * before including libfvalue.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFVALUE_DLL_IMPORT
#endif

#include <libfvalue.h>

#endif

#endif

