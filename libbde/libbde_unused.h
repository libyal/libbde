/*
 * The internal unused definition
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

#if !defined( _LIBBDE_INTERNAL_UNUSED_H )
#define _LIBBDE_INTERNAL_UNUSED_H

#include <common.h>

#if !defined( LIBBDE_ATTRIBUTE_UNUSED )
#if defined( __GNUC__ ) && __GNUC__ >= 3
#define LIBBDE_ATTRIBUTE_UNUSED	__attribute__ ((__unused__))
#else
#define LIBBDE_ATTRIBUTE_UNUSED
#endif
#endif

#if defined( _MSC_VER )
#define LIBBDE_UNREFERENCED_PARAMETER( parameter ) \
	UNREFERENCED_PARAMETER( parameter );
#else
#define LIBBDE_UNREFERENCED_PARAMETER( parameter ) \
	/* parameter */
#endif

#endif

