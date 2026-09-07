/*
 * Library password type test program
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_libbde.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_recovery.h"

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_utf8_recovery_password_calculate_hash function
 * Returns 1 if successful or 0 if not
 */
int bde_test_utf8_recovery_password_calculate_hash(
     void )
{
	uint8_t expected_password_hash[ 32 ] = {
		0xa4, 0xc9, 0x24, 0x44, 0x78, 0x11, 0x08, 0x05, 0x01, 0x5c, 0x94, 0xd1, 0xb0, 0x01, 0xa9, 0x33,
		0xad, 0xa2, 0xab, 0x0b, 0x71, 0x6b, 0xac, 0x34, 0x2b, 0xb8, 0x6c, 0x84, 0x4b, 0x63, 0x49, 0x1f };

	uint8_t password[ 56 ] = {
		0x34, 0x37, 0x31, 0x32, 0x30, 0x37, 0x2d, 0x32, 0x37, 0x38, 0x34, 0x39, 0x38, 0x2d, 0x34, 0x32,
		0x32, 0x31, 0x32, 0x35, 0x2d, 0x31, 0x37, 0x37, 0x31, 0x37, 0x37, 0x2d, 0x35, 0x36, 0x31, 0x39,
		0x30, 0x32, 0x2d, 0x35, 0x33, 0x37, 0x34, 0x30, 0x35, 0x2d, 0x34, 0x36, 0x38, 0x30, 0x30, 0x36,
		0x2d, 0x36, 0x39, 0x33, 0x34, 0x35, 0x31, 0x00 };

	uint8_t password_hash[ 32 ] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_utf8_recovery_password_calculate_hash(
	          password,
	          56,
	          password_hash,
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
	          expected_password_hash,
	          password_hash,
	          32 );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libbde_utf8_recovery_password_calculate_hash(
	          NULL,
	          4,
	          password_hash,
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

	result = libbde_utf8_recovery_password_calculate_hash(
	          password,
	          (size_t) SSIZE_MAX + 1,
	          password_hash,
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

	result = libbde_utf8_recovery_password_calculate_hash(
	          password,
	          4,
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

	result = libbde_utf8_recovery_password_calculate_hash(
	          password,
	          4,
	          password_hash,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libbde_utf16_recovery_password_calculate_hash function
 * Returns 1 if successful or 0 if not
 */
int bde_test_utf16_recovery_password_calculate_hash(
     void )
{
	uint8_t expected_password_hash[ 32 ] = {
		0xa4, 0xc9, 0x24, 0x44, 0x78, 0x11, 0x08, 0x05, 0x01, 0x5c, 0x94, 0xd1, 0xb0, 0x01, 0xa9, 0x33,
		0xad, 0xa2, 0xab, 0x0b, 0x71, 0x6b, 0xac, 0x34, 0x2b, 0xb8, 0x6c, 0x84, 0x4b, 0x63, 0x49, 0x1f };

	uint16_t password[ 56 ] = {
		0x34, 0x37, 0x31, 0x32, 0x30, 0x37, 0x2d, 0x32, 0x37, 0x38, 0x34, 0x39, 0x38, 0x2d, 0x34, 0x32,
		0x32, 0x31, 0x32, 0x35, 0x2d, 0x31, 0x37, 0x37, 0x31, 0x37, 0x37, 0x2d, 0x35, 0x36, 0x31, 0x39,
		0x30, 0x32, 0x2d, 0x35, 0x33, 0x37, 0x34, 0x30, 0x35, 0x2d, 0x34, 0x36, 0x38, 0x30, 0x30, 0x36,
		0x2d, 0x36, 0x39, 0x33, 0x34, 0x35, 0x31, 0x00 };

	uint8_t password_hash[ 32 ] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_utf16_recovery_password_calculate_hash(
	          password,
	          56,
	          password_hash,
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
	          expected_password_hash,
	          password_hash,
	          32 );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libbde_utf16_recovery_password_calculate_hash(
	          NULL,
	          4,
	          password_hash,
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

	result = libbde_utf16_recovery_password_calculate_hash(
	          password,
	          (size_t) SSIZE_MAX + 1,
	          password_hash,
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

	result = libbde_utf16_recovery_password_calculate_hash(
	          password,
	          4,
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

	result = libbde_utf16_recovery_password_calculate_hash(
	          password,
	          4,
	          password_hash,
	          (size_t) SSIZE_MAX + 1,
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
	 "libbde_utf8_recovery_password_calculate_hash",
	 bde_test_utf8_recovery_password_calculate_hash );

	BDE_TEST_RUN(
	 "libbde_utf16_recovery_password_calculate_hash",
	 bde_test_utf16_recovery_password_calculate_hash );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */
}

