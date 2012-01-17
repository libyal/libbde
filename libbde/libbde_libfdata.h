/*
 * The libfdata header wrapper
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

#if !defined( _LIBBDE_LIBFDATA_H )
#define _LIBBDE_LIBFDATA_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFDATA for local use of libfdata
 */
#if defined( HAVE_LOCAL_LIBFDATA )

#include <libfdata_block.h>
#include <libfdata_block_reference.h>
#include <libfdata_buffer.h>
#include <libfdata_buffer_reference.h>
#include <libfdata_definitions.h>
#include <libfdata_list.h>
#include <libfdata_list_element.h>
#include <libfdata_reference.h>
#include <libfdata_tree.h>
#include <libfdata_tree_node.h>
#include <libfdata_types.h>
#include <libfdata_vector.h>

#elif defined( HAVE_LIBFDATA_H )

/* If libtool DLL support is enabled set LIBFDATA_DLL_IMPORT
 * before including libfdata.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFDATA_DLL_IMPORT
#endif

#include <libfdata.h>

#else
#error Missing libfdata.h
#endif

#endif

