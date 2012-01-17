/*
 * The extern definition
 * This header should be included in header files that export or import
 * library functions
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

#if !defined( _LIBBDE_EXTERN_H )
#define _LIBBDE_EXTERN_H

/* To export functions from the libbde DLL define LIBBDE_DLL_EXPORT
 * To import functions from the libbde DLL define LIBBDE_DLL_IMPORT
 * Otherwise use default extern statement
 */
#if defined( LIBBDE_DLL_EXPORT )
#define LIBBDE_EXTERN __declspec(dllexport)

#elif defined( LIBBDE_DLL_IMPORT )
#define LIBBDE_EXTERN extern __declspec(dllimport)

#else
#define LIBBDE_EXTERN extern

#endif

#endif

