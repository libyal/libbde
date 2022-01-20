/*
 * Library volume_master_key type test program
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

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_libbde.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_volume_master_key.h"

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_volume_master_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_master_key_initialize(
     void )
{
	libbde_volume_master_key_t *volume_master_key = NULL;
	libcerror_error_t *error                      = NULL;
	int result                                    = 0;

#if defined( HAVE_BDE_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libbde_volume_master_key_initialize(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_master_key_free(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_volume_master_key_initialize(
	          NULL,
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

	volume_master_key = (libbde_volume_master_key_t *) 0x12345678UL;

	result = libbde_volume_master_key_initialize(
	          &volume_master_key,
	          &error );

	volume_master_key = NULL;

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libbde_volume_master_key_initialize with malloc failing
		 */
		bde_test_malloc_attempts_before_fail = test_number;

		result = libbde_volume_master_key_initialize(
		          &volume_master_key,
		          &error );

		if( bde_test_malloc_attempts_before_fail != -1 )
		{
			bde_test_malloc_attempts_before_fail = -1;

			if( volume_master_key != NULL )
			{
				libbde_volume_master_key_free(
				 &volume_master_key,
				 NULL );
			}
		}
		else
		{
			BDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			BDE_TEST_ASSERT_IS_NULL(
			 "volume_master_key",
			 volume_master_key );

			BDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libbde_volume_master_key_initialize with memset failing
		 */
		bde_test_memset_attempts_before_fail = test_number;

		result = libbde_volume_master_key_initialize(
		          &volume_master_key,
		          &error );

		if( bde_test_memset_attempts_before_fail != -1 )
		{
			bde_test_memset_attempts_before_fail = -1;

			if( volume_master_key != NULL )
			{
				libbde_volume_master_key_free(
				 &volume_master_key,
				 NULL );
			}
		}
		else
		{
			BDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			BDE_TEST_ASSERT_IS_NULL(
			 "volume_master_key",
			 volume_master_key );

			BDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_BDE_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume_master_key != NULL )
	{
		libbde_volume_master_key_free(
		 &volume_master_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_master_key_free function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_master_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbde_volume_master_key_free(
	          NULL,
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

/* Tests the libbde_volume_master_key_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_master_key_get_identifier(
     void )
{
	uint8_t guid_data[ 16 ];

	libbde_volume_master_key_t *volume_master_key = NULL;
	libcerror_error_t *error                      = NULL;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libbde_volume_master_key_initialize(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbde_volume_master_key_get_identifier(
	          volume_master_key,
	          guid_data,
	          16,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_volume_master_key_get_identifier(
	          NULL,
	          guid_data,
	          16,
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

	result = libbde_volume_master_key_get_identifier(
	          volume_master_key,
	          NULL,
	          16,
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

	result = libbde_volume_master_key_get_identifier(
	          volume_master_key,
	          guid_data,
	          0,
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

	result = libbde_volume_master_key_get_identifier(
	          volume_master_key,
	          guid_data,
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

	/* Clean up
	 */
	result = libbde_volume_master_key_free(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume_master_key != NULL )
	{
		libbde_volume_master_key_free(
		 &volume_master_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_master_key_get_protection_type function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_master_key_get_protection_type(
     void )
{
	libbde_volume_master_key_t *volume_master_key = NULL;
	libcerror_error_t *error                      = NULL;
	uint16_t protection_type                      = 0;
	int protection_type_is_set                    = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libbde_volume_master_key_initialize(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbde_volume_master_key_get_protection_type(
	          volume_master_key,
	          &protection_type,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	protection_type_is_set = result;

	/* Test error cases
	 */
	result = libbde_volume_master_key_get_protection_type(
	          NULL,
	          &protection_type,
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

	if( protection_type_is_set != 0 )
	{
		result = libbde_volume_master_key_get_protection_type(
		          volume_master_key,
		          NULL,
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
	/* Clean up
	 */
	result = libbde_volume_master_key_free(
	          &volume_master_key,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume_master_key",
	 volume_master_key );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume_master_key != NULL )
	{
		libbde_volume_master_key_free(
		 &volume_master_key,
		 NULL );
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
	 "libbde_volume_master_key_initialize",
	 bde_test_volume_master_key_initialize );

	BDE_TEST_RUN(
	 "libbde_volume_master_key_free",
	 bde_test_volume_master_key_free );

	/* TODO: add tests for libbde_volume_master_key_read */

	/* TODO: add tests for libbde_volume_master_key_get_identifier */

	BDE_TEST_RUN(
	 "libbde_volume_master_key_get_protection_type",
	 bde_test_volume_master_key_get_protection_type );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

