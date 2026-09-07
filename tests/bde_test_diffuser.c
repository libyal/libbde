/*
 * Library diffuser type test program
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_libbde.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_diffuser.h"

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_diffuser_decrypt function
 * Returns 1 if successful or 0 if not
 */
int bde_test_diffuser_decrypt(
     void )
{
	uint8_t data[ 32 ] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20
	};
	uint8_t expected_data[ 32 ] = {
		0x34, 0x96, 0x3f, 0x17, 0x74, 0x30, 0x4f, 0xfe, 0xa4, 0x9e, 0x27, 0x2b, 0xcc, 0xef, 0xf8, 0xd0,
		0xb5, 0x71, 0xec, 0x7b, 0x95, 0x43, 0x72, 0xb6, 0xdd, 0x1b, 0x39, 0x74, 0xa8, 0xb5, 0x80, 0xe2,
	};
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_diffuser_decrypt(
	          data,
	          32,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          expected_data,
	          data,
	          32 );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libbde_diffuser_decrypt(
	          NULL,
	          32,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_diffuser_encrypt function
 * Returns 1 if successful or 0 if not
 */
int bde_test_diffuser_encrypt(
     void )
{
	uint8_t data[ 32 ] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	};
	uint8_t expected_data[ 32 ] = {
		0xc4, 0x65, 0x11, 0xb8, 0xa8, 0x2e, 0x9f, 0x69, 0x52, 0x12, 0x8a, 0xe6, 0xed, 0x0e, 0xd2, 0xee,
		0x09, 0x0a, 0xa0, 0xe7, 0xfc, 0x7f, 0x0e, 0x80, 0xd4, 0xdb, 0x00, 0x39, 0xe3, 0x88, 0x5d, 0x68
	};
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_diffuser_encrypt(
	          data,
	          32,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          expected_data,
	          data,
	          32 );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libbde_diffuser_encrypt(
	          NULL,
	          32,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc BDE_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] BDE_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc BDE_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] BDE_TEST_ATTRIBUTE_UNUSED )
#endif
{
	BDE_TEST_UNREFERENCED_PARAMETER( argc )
	BDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

	BDE_TEST_RUN(
	 "libbde_diffuser_decrypt",
	 bde_test_diffuser_decrypt );

	BDE_TEST_RUN(
	 "libbde_diffuser_encrypt",
	 bde_test_diffuser_encrypt );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */
}

