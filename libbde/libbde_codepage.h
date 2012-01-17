/*
 * Codepage functions
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

#if !defined( _LIBBDE_INTERNAL_CODEPAGE_H )
#define _LIBBDE_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBBDE )

#include <libbde/codepage.h>

/* Define HAVE_LOCAL_LIBBDE for local use of libbde
 * The definitions in <libbde/codepage.h> are copied here
 * for local use of libbde
 */
#else

/* The codepage definitions
 */
enum LIBBDE_CODEPAGE
{
	LIBBDE_CODEPAGE_ASCII				= 20127,

	LIBBDE_CODEPAGE_ISO_8859_1			= 28591,
	LIBBDE_CODEPAGE_ISO_8859_2			= 28592,
	LIBBDE_CODEPAGE_ISO_8859_3			= 28593,
	LIBBDE_CODEPAGE_ISO_8859_4			= 28594,
	LIBBDE_CODEPAGE_ISO_8859_5			= 28595,
	LIBBDE_CODEPAGE_ISO_8859_6			= 28596,
	LIBBDE_CODEPAGE_ISO_8859_7			= 28597,
	LIBBDE_CODEPAGE_ISO_8859_8			= 28598,
	LIBBDE_CODEPAGE_ISO_8859_9			= 28599,
	LIBBDE_CODEPAGE_ISO_8859_10			= 28600,
	LIBBDE_CODEPAGE_ISO_8859_11			= 28601,
	LIBBDE_CODEPAGE_ISO_8859_13			= 28603,
	LIBBDE_CODEPAGE_ISO_8859_14			= 28604,
	LIBBDE_CODEPAGE_ISO_8859_15			= 28605,
	LIBBDE_CODEPAGE_ISO_8859_16			= 28606,

	LIBBDE_CODEPAGE_KOI8_R				= 20866,
	LIBBDE_CODEPAGE_KOI8_U				= 21866,

	LIBBDE_CODEPAGE_WINDOWS_874			= 874,
	LIBBDE_CODEPAGE_WINDOWS_932			= 932,
	LIBBDE_CODEPAGE_WINDOWS_936			= 936,
	LIBBDE_CODEPAGE_WINDOWS_949			= 949,
	LIBBDE_CODEPAGE_WINDOWS_950			= 950,
	LIBBDE_CODEPAGE_WINDOWS_1250			= 1250,
	LIBBDE_CODEPAGE_WINDOWS_1251			= 1251,
	LIBBDE_CODEPAGE_WINDOWS_1252			= 1252,
	LIBBDE_CODEPAGE_WINDOWS_1253			= 1253,
	LIBBDE_CODEPAGE_WINDOWS_1254			= 1254,
	LIBBDE_CODEPAGE_WINDOWS_1255			= 1255,
	LIBBDE_CODEPAGE_WINDOWS_1256			= 1256,
	LIBBDE_CODEPAGE_WINDOWS_1257			= 1257,
	LIBBDE_CODEPAGE_WINDOWS_1258			= 1258
};

#endif

#if defined( __cplusplus )
}
#endif

#endif

