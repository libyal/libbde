/*
 * The internal type definitions
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

#if !defined( _LIBBDE_INTERNAL_TYPES_H )
#define _LIBBDE_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBBDE for local use of libbde
 * The definitions in <libbde/types.h> are copied here
 * for local use of libbde
 */
#if defined( HAVE_LOCAL_LIBBDE )

/* The following type definitions hide internal data structures
 */
typedef intptr_t libbde_store_t;
typedef intptr_t libbde_volume_t;

#endif

#endif

