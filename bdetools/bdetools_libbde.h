/*
 * The internal libbde header
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

#if !defined( _BDETOOLS_LIBBDE_H )
#define _BDETOOLS_LIBBDE_H

#include <common.h>

/* If Cygwin libtool DLL support is enabled set LIBBDE_DLL_IMPORT
 * before including libbde.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBBDE_DLL_IMPORT
#endif

#include <libbde.h>

#endif

