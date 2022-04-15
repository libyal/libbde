/*
 * Library metadata_block_header type test program
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
#include <byte_stream.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_functions.h"
#include "bde_test_libbde.h"
#include "bde_test_libbfio.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_metadata_block_header.h"

uint8_t bde_test_metadata_block_header_data1[ 64 ] = {
	0x2d, 0x46, 0x56, 0x45, 0x2d, 0x46, 0x53, 0x2d, 0x2f, 0x00, 0x02, 0x00, 0x04, 0x00, 0x04, 0x00,
	0x00, 0x00, 0xa0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x28, 0x00, 0x00,
	0x00, 0x00, 0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xd5, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xa8, 0x5a, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x2b, 0x0d, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_metadata_block_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int bde_test_metadata_block_header_initialize(
     void )
{
	libbde_metadata_block_header_t *metadata_block_header = NULL;
	libcerror_error_t *error                              = NULL;
	int result                                            = 0;

#if defined( HAVE_BDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                       = 1;
	int number_of_memset_fail_tests                       = 1;
	int test_number                                       = 0;
#endif

	/* Test regular cases
	 */
	result = libbde_metadata_block_header_initialize(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata_block_header",
	 metadata_block_header );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_metadata_block_header_free(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "metadata_block_header",
	 metadata_block_header );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_metadata_block_header_initialize(
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

	metadata_block_header = (libbde_metadata_block_header_t *) 0x12345678UL;

	result = libbde_metadata_block_header_initialize(
	          &metadata_block_header,
	          &error );

	metadata_block_header = NULL;

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
		/* Test libbde_metadata_block_header_initialize with malloc failing
		 */
		bde_test_malloc_attempts_before_fail = test_number;

		result = libbde_metadata_block_header_initialize(
		          &metadata_block_header,
		          &error );

		if( bde_test_malloc_attempts_before_fail != -1 )
		{
			bde_test_malloc_attempts_before_fail = -1;

			if( metadata_block_header != NULL )
			{
				libbde_metadata_block_header_free(
				 &metadata_block_header,
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
			 "metadata_block_header",
			 metadata_block_header );

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
		/* Test libbde_metadata_block_header_initialize with memset failing
		 */
		bde_test_memset_attempts_before_fail = test_number;

		result = libbde_metadata_block_header_initialize(
		          &metadata_block_header,
		          &error );

		if( bde_test_memset_attempts_before_fail != -1 )
		{
			bde_test_memset_attempts_before_fail = -1;

			if( metadata_block_header != NULL )
			{
				libbde_metadata_block_header_free(
				 &metadata_block_header,
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
			 "metadata_block_header",
			 metadata_block_header );

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
	if( metadata_block_header != NULL )
	{
		libbde_metadata_block_header_free(
		 &metadata_block_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_metadata_block_header_free function
 * Returns 1 if successful or 0 if not
 */
int bde_test_metadata_block_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbde_metadata_block_header_free(
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

/* Tests the libbde_metadata_block_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bde_test_metadata_block_header_read_file_io_handle(
     void )
{
	libbde_metadata_block_header_t *metadata_block_header = NULL;
	libbfio_handle_t *file_io_handle                      = NULL;
	libcerror_error_t *error                              = NULL;
	int result                                            = 0;

	/* Initialize test
	 */
	result = libbde_metadata_block_header_initialize(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata_block_header",
	 metadata_block_header );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          bde_test_metadata_block_header_data1,
	          64,
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

	/* Test regular cases
	 */
	result = libbde_metadata_block_header_read_file_io_handle(
	          metadata_block_header,
	          file_io_handle,
	          0,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_metadata_block_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
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

	result = libbde_metadata_block_header_read_file_io_handle(
	          metadata_block_header,
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

	/* Test data too small
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          bde_test_metadata_block_header_data1,
	          8,
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

	result = libbde_metadata_block_header_read_file_io_handle(
	          metadata_block_header,
	          file_io_handle,
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

	/* Test invalid format version
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          bde_test_metadata_block_header_data1,
	          64,
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

	byte_stream_copy_from_uint16_little_endian(
	 &( bde_test_metadata_block_header_data1[ 10 ] ),
	 0xffff );

	result = libbde_metadata_block_header_read_file_io_handle(
	          metadata_block_header,
	          file_io_handle,
	          0,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( bde_test_metadata_block_header_data1[ 10 ] ),
	 0x0002 );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

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

	/* Clean up
	 */
	result = libbde_metadata_block_header_free(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "metadata_block_header",
	 metadata_block_header );

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
	if( metadata_block_header != NULL )
	{
		libbde_metadata_block_header_free(
		 &metadata_block_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_metadata_block_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int bde_test_metadata_block_header_read_data(
     void )
{
	libbde_metadata_block_header_t *metadata_block_header = NULL;
	libcerror_error_t *error                              = NULL;
	int result                                            = 0;

	/* Initialize test
	 */
	result = libbde_metadata_block_header_initialize(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata_block_header",
	 metadata_block_header );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          bde_test_metadata_block_header_data1,
	          64,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_metadata_block_header_read_data(
	          NULL,
	          bde_test_metadata_block_header_data1,
	          64,
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

	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          NULL,
	          64,
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

	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          bde_test_metadata_block_header_data1,
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

	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          bde_test_metadata_block_header_data1,
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

	/* Test bde_test_metadata_block_header_read_data with memcpy failing
	 */
	bde_test_memcpy_attempts_before_fail = 0;

	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          bde_test_metadata_block_header_data1,
	          64,
	          &error );

	if( bde_test_memcpy_attempts_before_fail != -1 )
	{
		bde_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
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
#endif /* defined( HAVE_BDE_TEST_MEMORY ) */

	/* Test invalid format version
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( bde_test_metadata_block_header_data1[ 10 ] ),
	 0xffff );

	result = libbde_metadata_block_header_read_data(
	          metadata_block_header,
	          bde_test_metadata_block_header_data1,
	          64,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( bde_test_metadata_block_header_data1[ 10 ] ),
	 0x0002 );

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
	result = libbde_metadata_block_header_free(
	          &metadata_block_header,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "metadata_block_header",
	 metadata_block_header );

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
	if( metadata_block_header != NULL )
	{
		libbde_metadata_block_header_free(
		 &metadata_block_header,
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
	 "libbde_metadata_block_header_initialize",
	 bde_test_metadata_block_header_initialize );

	BDE_TEST_RUN(
	 "libbde_metadata_block_header_free",
	 bde_test_metadata_block_header_free );

	BDE_TEST_RUN(
	 "libbde_metadata_block_header_read_file_io_handle",
	 bde_test_metadata_block_header_read_file_io_handle );

	BDE_TEST_RUN(
	 "libbde_metadata_block_header_read_data",
	 bde_test_metadata_block_header_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */
}

