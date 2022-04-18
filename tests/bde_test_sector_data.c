/*
 * Library sector_data type test program
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

#include "bde_test_functions.h"
#include "bde_test_libbde.h"
#include "bde_test_libbfio.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"
#include "bde_test_unused.h"

#include "../libbde/libbde_definitions.h"
#include "../libbde/libbde_encryption_context.h"
#include "../libbde/libbde_io_handle.h"
#include "../libbde/libbde_sector_data.h"

uint8_t bde_test_sector_data_data1[ 512 ] = {
	0xb1, 0x24, 0x3d, 0xf3, 0x34, 0x83, 0x9c, 0x86, 0xc9, 0xad, 0x52, 0xc3, 0xa1, 0x9d, 0x02, 0x2b,
	0x26, 0x8d, 0xde, 0xfb, 0x7b, 0x20, 0x0a, 0x40, 0x86, 0xfd, 0x61, 0xa6, 0xdf, 0x9c, 0x68, 0x73,
	0x6d, 0x01, 0x96, 0x4e, 0x49, 0xe1, 0x98, 0xfd, 0x21, 0x9b, 0xba, 0xcc, 0xa0, 0xc9, 0x60, 0x49,
	0xe7, 0x9c, 0x44, 0xd9, 0x92, 0x42, 0x50, 0x17, 0x3f, 0x46, 0xd5, 0x36, 0x3e, 0xb6, 0x38, 0xd6,
	0x62, 0x08, 0xa1, 0xc4, 0x2c, 0xca, 0xf6, 0x08, 0x84, 0x13, 0x90, 0x71, 0x40, 0x66, 0xd2, 0x5d,
	0x39, 0xc4, 0x38, 0xe4, 0x69, 0xb2, 0x86, 0xcc, 0xd9, 0x4b, 0x2b, 0x9d, 0xbf, 0xdc, 0x56, 0x12,
	0xa2, 0xd1, 0x83, 0x70, 0xa1, 0xd1, 0x45, 0xad, 0x87, 0xcd, 0x37, 0x7a, 0x17, 0x85, 0x5f, 0x2f,
	0x72, 0xd2, 0x85, 0x87, 0xc0, 0x0c, 0x9c, 0xd1, 0x65, 0x01, 0x2d, 0xc1, 0x2d, 0xe0, 0xd1, 0xa9,
	0xf6, 0x94, 0xc2, 0x96, 0xa9, 0xb3, 0x35, 0xc5, 0x29, 0x2d, 0x11, 0x28, 0x01, 0x0f, 0x77, 0xef,
	0x45, 0x27, 0x0b, 0x9a, 0x2e, 0x58, 0xc5, 0x96, 0xf8, 0x5c, 0x01, 0xa7, 0x53, 0x2d, 0x0b, 0xbb,
	0xc9, 0x40, 0x6f, 0x26, 0x39, 0xb0, 0x7d, 0x08, 0x7d, 0x2f, 0xe2, 0xa1, 0xc3, 0x47, 0x52, 0x27,
	0x03, 0x38, 0x8b, 0xe7, 0x7d, 0x6c, 0x2f, 0x6a, 0x71, 0xb4, 0x4f, 0xa8, 0x66, 0xd3, 0x49, 0x90,
	0xeb, 0x67, 0xbb, 0xd2, 0xdb, 0xb5, 0xc2, 0x76, 0x2c, 0x26, 0xc2, 0x3b, 0xc7, 0xec, 0xaf, 0x23,
	0x31, 0xe3, 0xa3, 0xb4, 0x11, 0xdb, 0x99, 0x90, 0x0b, 0xee, 0xeb, 0x8d, 0x0d, 0xc3, 0xc4, 0x53,
	0xbb, 0xb8, 0xf8, 0x17, 0xcc, 0x15, 0x67, 0x84, 0x9e, 0xa0, 0x33, 0xec, 0x53, 0xd1, 0x48, 0x84,
	0xc7, 0xad, 0x6d, 0x95, 0x2c, 0x2f, 0x36, 0x8d, 0x40, 0xf6, 0xd3, 0x16, 0xd5, 0x66, 0xf6, 0xac,
	0x02, 0x9a, 0x5c, 0xa1, 0x29, 0x35, 0x05, 0xb1, 0xd8, 0xc0, 0x4b, 0x5e, 0x83, 0x60, 0x29, 0xfa,
	0x09, 0xf9, 0x86, 0x57, 0xe4, 0xca, 0x29, 0x40, 0xeb, 0xde, 0x5c, 0x94, 0x89, 0xaf, 0xd5, 0xc0,
	0xeb, 0x0f, 0x94, 0xf0, 0x60, 0x6c, 0xe9, 0x31, 0xfe, 0x3a, 0xe8, 0x9b, 0xf5, 0x93, 0xa5, 0xfd,
	0xc1, 0x68, 0xd3, 0xd4, 0x04, 0xab, 0xa5, 0x62, 0x5d, 0x74, 0x23, 0xe6, 0xfa, 0x2c, 0x8e, 0xda,
	0x6e, 0xd6, 0x92, 0xf6, 0x85, 0xc1, 0x07, 0x61, 0x2a, 0xce, 0x23, 0xd7, 0x2b, 0x46, 0x32, 0xe2,
	0xf5, 0xc1, 0xc6, 0x65, 0xa5, 0x19, 0x71, 0x23, 0xf1, 0x2e, 0x4d, 0x76, 0x8a, 0xe4, 0xd3, 0x79,
	0x7c, 0x6a, 0x4c, 0x80, 0x06, 0x5c, 0xef, 0x6c, 0x0b, 0x55, 0x17, 0xdc, 0x8e, 0x35, 0xff, 0x05,
	0x3d, 0x4c, 0x99, 0x3e, 0x3e, 0x56, 0x85, 0x70, 0x2f, 0x6a, 0xcf, 0x86, 0x51, 0xca, 0xb2, 0x60,
	0x65, 0xe4, 0xf9, 0xb2, 0xe9, 0x87, 0x98, 0xc5, 0x10, 0x5e, 0xc5, 0x91, 0x8b, 0x25, 0xcf, 0x8f,
	0x3a, 0x47, 0xb1, 0xb3, 0x1d, 0x97, 0x44, 0xa7, 0x18, 0x9f, 0xa5, 0xd4, 0x41, 0x6b, 0x40, 0xa8,
	0xea, 0x3f, 0xf5, 0x3f, 0xc5, 0x08, 0xd8, 0xf2, 0x9a, 0xd1, 0x26, 0x0f, 0xf1, 0x1d, 0xf4, 0x64,
	0xcf, 0x79, 0xea, 0xb4, 0x93, 0x85, 0x1d, 0xc8, 0x71, 0x47, 0x0f, 0x7e, 0xe5, 0x74, 0xff, 0x63,
	0xb9, 0x0d, 0x6f, 0xbc, 0x48, 0x5c, 0xec, 0x31, 0x32, 0xc6, 0x33, 0x7f, 0xa5, 0x47, 0xa2, 0x5e,
	0xa2, 0xca, 0x3e, 0xd1, 0x82, 0x45, 0x01, 0xa6, 0x1d, 0x6b, 0x83, 0x51, 0x86, 0x33, 0x8a, 0x8b,
	0xf1, 0xad, 0x85, 0xb8, 0xb5, 0x63, 0xc6, 0x9e, 0x60, 0x79, 0xd1, 0x77, 0xb3, 0xd9, 0x4a, 0x9c,
	0xd7, 0x2b, 0x29, 0x2f, 0x53, 0x61, 0x50, 0x59, 0x41, 0xdb, 0x84, 0x6f, 0x56, 0x35, 0x25, 0xc8 };

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
	int number_of_malloc_fail_tests   = 2;
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
	uint8_t key_data[ 16 ] = {
		0x54, 0xe6, 0x30, 0x48, 0x2e, 0xb9, 0xdb, 0x51, 0x53, 0x33, 0xc4, 0x1e, 0x4a, 0x5a, 0xe8, 0xa6 };

	libbde_encryption_context_t *encryption_context = NULL;
	libbde_io_handle_t *io_handle                   = NULL;
	libbde_sector_data_t *sector_data               = NULL;
	libbfio_handle_t *file_io_handle                = NULL;
	libcerror_error_t *error                        = NULL;
	int result                                      = 0;

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

	result = libbde_io_handle_initialize(
	          &io_handle,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_encryption_context_initialize(
	          &encryption_context,
	          LIBBDE_ENCRYPTION_METHOD_AES_128_CBC,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "encryption_context",
	 encryption_context );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_encryption_context_set_keys(
	          encryption_context,
	          key_data,
	          16,
	          NULL,
	          0,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          bde_test_sector_data_data1,
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

	/* Test regular cases
	 */
	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          io_handle,
	          file_io_handle,
	          0,
	          encryption_context,
	          1,
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
	result = libbde_sector_data_read_file_io_handle(
	          NULL,
	          io_handle,
	          file_io_handle,
	          0,
	          encryption_context,
	          1,
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

	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          NULL,
	          file_io_handle,
	          0,
	          encryption_context,
	          1,
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

	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          io_handle,
	          NULL,
	          0,
	          encryption_context,
	          1,
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

	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          io_handle,
	          file_io_handle,
	          -1,
	          encryption_context,
	          1,
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

	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          io_handle,
	          file_io_handle,
	          0,
	          NULL,
	          1,
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

	/* Test data too small
	 */
	result = bde_test_open_file_io_handle(
	          &file_io_handle,
	          bde_test_sector_data_data1,
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

	result = libbde_sector_data_read_file_io_handle(
	          sector_data,
	          io_handle,
	          file_io_handle,
	          0,
	          encryption_context,
	          1,
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

	/* Clean up
	 */
	result = libbde_encryption_context_free(
	          &encryption_context,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "encryption_context",
	 encryption_context );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_io_handle_free(
	          &io_handle,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( encryption_context != NULL )
	{
		libbde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libbde_io_handle_free(
		 &io_handle,
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

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */
}

