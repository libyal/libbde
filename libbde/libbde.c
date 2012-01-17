/*
 * Library to support the BitLocker Drive Encryption (BDE) format
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

#include <common.h>

#if defined( WINAPI )
#include <windows.h>
#endif

#include "libbde_unused.h"

/* Define HAVE_LOCAL_LIBBDE for local use of libbde
 */
#if !defined( HAVE_LOCAL_LIBBDE )

#if defined( WINAPI )

#if defined( _MANAGED )
#pragma managed( push, off )
#endif

/* Defines the entry point for the DLL
 */
BOOL WINAPI DllMain(
             HINSTANCE hinstDLL,
             DWORD fdwReason,
             LPVOID lpvReserved )
{
	LIBBDE_UNREFERENCED_PARAMETER( lpvReserved )

	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(
			 hinstDLL );
		break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			break;
	}
	return( TRUE );
}

/* Function that indicates the library is a DLL
 * Returns 1
 */
int libbde_is_dll(
     void )
{
	return( 1 );
}

#endif /* defined( WINAPI ) */

#endif /* !defined( HAVE_LOCAL_LIBBDE ) */
