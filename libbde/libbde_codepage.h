/*
 * Codepage functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_INTERNAL_CODEPAGE_H )
#define _LIBBDE_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBBDE for local use of libbde
 * The definitions in <libbde/codepage.h> are copied here
 * for local use of libbde
 */
#if !defined( HAVE_LOCAL_LIBBDE )

#include <libbde/codepage.h>

#else

/* The codepage definitions
 */
enum LIBBDE_CODEPAGES
{
	LIBBDE_CODEPAGE_ASCII		= 20127,

	LIBBDE_CODEPAGE_ISO_8859_1	= 28591,
	LIBBDE_CODEPAGE_ISO_8859_2	= 28592,
	LIBBDE_CODEPAGE_ISO_8859_3	= 28593,
	LIBBDE_CODEPAGE_ISO_8859_4	= 28594,
	LIBBDE_CODEPAGE_ISO_8859_5	= 28595,
	LIBBDE_CODEPAGE_ISO_8859_6	= 28596,
	LIBBDE_CODEPAGE_ISO_8859_7	= 28597,
	LIBBDE_CODEPAGE_ISO_8859_8	= 28598,
	LIBBDE_CODEPAGE_ISO_8859_9	= 28599,
	LIBBDE_CODEPAGE_ISO_8859_10	= 28600,
	LIBBDE_CODEPAGE_ISO_8859_11	= 28601,
	LIBBDE_CODEPAGE_ISO_8859_13	= 28603,
	LIBBDE_CODEPAGE_ISO_8859_14	= 28604,
	LIBBDE_CODEPAGE_ISO_8859_15	= 28605,
	LIBBDE_CODEPAGE_ISO_8859_16	= 28606,

	LIBBDE_CODEPAGE_KOI8_R		= 20866,
	LIBBDE_CODEPAGE_KOI8_U		= 21866,

	LIBBDE_CODEPAGE_WINDOWS_874	= 874,
	LIBBDE_CODEPAGE_WINDOWS_932	= 932,
	LIBBDE_CODEPAGE_WINDOWS_936	= 936,
	LIBBDE_CODEPAGE_WINDOWS_949	= 949,
	LIBBDE_CODEPAGE_WINDOWS_950	= 950,
	LIBBDE_CODEPAGE_WINDOWS_1250	= 1250,
	LIBBDE_CODEPAGE_WINDOWS_1251	= 1251,
	LIBBDE_CODEPAGE_WINDOWS_1252	= 1252,
	LIBBDE_CODEPAGE_WINDOWS_1253	= 1253,
	LIBBDE_CODEPAGE_WINDOWS_1254	= 1254,
	LIBBDE_CODEPAGE_WINDOWS_1255	= 1255,
	LIBBDE_CODEPAGE_WINDOWS_1256	= 1256,
	LIBBDE_CODEPAGE_WINDOWS_1257	= 1257,
	LIBBDE_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBBDE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBBDE_INTERNAL_CODEPAGE_H ) */

