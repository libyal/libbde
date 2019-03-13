/*
 * Library sector_data type test program
 *
 * Copyright (C) 2011-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_functions.h"
#include "bde_test_libbfio.h"
#include "bde_test_libcerror.h"
#include "bde_test_libbde.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_sector_data.h"

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_sector_data_initialize function
 * Returns 1 if successful or 0 if not
 */
int bde_test_sector_data_initialize(
     void )
{
	libbde_sector_data_t *sector_data = NULL;
	libcerror_error_t *error          = NULL;
	int result                        = 0;

#if defined( HAVE_BDE_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 3;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libbde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_sector_data_free(
	          &sector_data,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_sector_data_initialize(
	          NULL,
	          512,
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

	sector_data = (libbde_sector_data_t *) 0x12345678UL;

	result = libbde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	sector_data = NULL;

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbde_sector_data_initialize(
	          &sector_data,
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

	result = libbde_sector_data_initialize(
	          &sector_data,
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

#if defined( HAVE_BDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libbde_sector_data_initialize with malloc failing
		 */
		bde_test_malloc_attempts_before_fail = test_number;

		result = libbde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( bde_test_malloc_attempts_before_fail != -1 )
		{
			bde_test_malloc_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libbde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

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
		/* Test libbde_sector_data_initialize with memset failing
		 */
		bde_test_memset_attempts_before_fail = test_number;

		result = libbde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( bde_test_memset_attempts_before_fail != -1 )
		{
			bde_test_memset_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libbde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

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
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_sector_data_free function
 * Returns 1 if successful or 0 if not
 */
int bde_test_sector_data_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbde_sector_data_free(
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

/* Tests the libbde_sector_data_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bde_test_sector_data_read_file_io_handle(
     void )
{
	uint8_t test_data[ 512 ];

	libbde_sector_data_t *sector_data = NULL;
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libbde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          test_data,
	          512,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO: test regular cases */

	/* Test error cases
	 */
	result = libbde_sector_data_read_file_io_handle(
	          NULL,
	          NULL,
	          file_io_handle,
	          0,
	          NULL,
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

/* TODO add more tests */

	/* Clean up file IO handle
	 */
	result = bde_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO test data too small */

	/* Clean up
	 */
	result = libbde_sector_data_free(
	          &sector_data,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( sector_data != NULL )
	{
		libbde_sector_data_free(
		 &sector_data,
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
	 "libbde_sector_data_initialize",
	 bde_test_sector_data_initialize );

	BDE_TEST_RUN(
	 "libbde_sector_data_free",
	 bde_test_sector_data_free );

	BDE_TEST_RUN(
	 "libbde_sector_data_read_file_io_handle",
	 bde_test_sector_data_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

