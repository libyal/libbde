/*
 * Library recovery type test program
 *
 * Copyright (C) 2011-2025, Joachim Metz <joachim.metz@gmail.com>
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

/* Tests the libbde_recovery_password_from_vmk function
 * Returns 1 if successful or 0 if not
 */
int bde_test_recovery_password_from_vmk(
     void )
{
	uint8_t recovery_password[ 56 ];

	uint8_t vmk_bytes[ 32 ] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	libbde_metadata_t metadata;
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test: NULL recovery_password_volume_master_key (soft return 0)
	 */
	memory_set(
	 &metadata,
	 0,
	 sizeof( libbde_metadata_t ) );

	result = libbde_recovery_password_from_vmk(
	          &metadata,
	          vmk_bytes,
	          32,
	          recovery_password,
	          56,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */

	/* Test: NULL metadata
	 */
	result = libbde_recovery_password_from_vmk(
	          NULL,
	          vmk_bytes,
	          32,
	          recovery_password,
	          56,
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

	/* Test: remaining argument errors
	 * Use stack-allocated metadata with non-NULL VMK key and aes_ccm key
	 * so argument validation for vmk_bytes, vmk_bytes_size, recovery_password,
	 * and recovery_password_size is reached.
	 */
	{
		libbde_aes_ccm_encrypted_key_t aes_ccm_key;
		libbde_volume_master_key_t vmk;

		memory_set(
		 &aes_ccm_key,
		 0,
		 sizeof( libbde_aes_ccm_encrypted_key_t ) );

		memory_set(
		 &vmk,
		 0,
		 sizeof( libbde_volume_master_key_t ) );

		vmk.aes_ccm_encrypted_key = &aes_ccm_key;

		memory_set(
		 &metadata,
		 0,
		 sizeof( libbde_metadata_t ) );

		metadata.recovery_password_volume_master_key = &vmk;

		/* Test: NULL vmk_bytes */
		result = libbde_recovery_password_from_vmk(
		          &metadata,
		          NULL,
		          32,
		          recovery_password,
		          56,
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

		/* Test: vmk_bytes_size too small */
		result = libbde_recovery_password_from_vmk(
		          &metadata,
		          vmk_bytes,
		          31,
		          recovery_password,
		          56,
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

		/* Test: NULL recovery_password */
		result = libbde_recovery_password_from_vmk(
		          &metadata,
		          vmk_bytes,
		          32,
		          NULL,
		          56,
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

		/* Test: recovery_password_size too small */
		result = libbde_recovery_password_from_vmk(
		          &metadata,
		          vmk_bytes,
		          32,
		          recovery_password,
		          55,
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
	}
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
	 "libbde_recovery_password_from_vmk",
	 bde_test_recovery_password_from_vmk );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}
