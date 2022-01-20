/*
 * Library volume type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "bde_test_functions.h"
#include "bde_test_getopt.h"
#include "bde_test_libbde.h"
#include "bde_test_libbfio.h"
#include "bde_test_libcerror.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"

#include "../libbde/libbde_volume.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make bde_test_volume generate verbose output
#define BDE_TEST_VOLUME_VERBOSE
 */

#define BDE_TEST_VOLUME_READ_BUFFER_SIZE	4096

#if !defined( LIBBDE_HAVE_BFIO )

LIBBDE_EXTERN \
int libbde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libbde_error_t **error );

#endif /* !defined( LIBBDE_HAVE_BFIO ) */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int bde_test_volume_open_source(
     libbde_volume_t **volume,
     libbfio_handle_t *file_io_handle,
     const system_character_t *password,
     const system_character_t *recovery_password,
     const system_character_t *startup_key,
     libcerror_error_t **error )
{
	static char *function = "bde_test_volume_open_source";
	size_t string_length  = 0;
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbde_volume_initialize(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_password(
		          *volume,
		          (uint16_t *) password,
		          string_length,
		          error );
#else
		result = libbde_volume_set_utf8_password(
		          *volume,
		          (uint8_t *) password,
		          string_length,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
	if( recovery_password != NULL )
	{
		string_length = system_string_length(
		                 recovery_password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_recovery_password(
		          *volume,
		          (uint16_t *) recovery_password,
		          string_length,
		          error );
#else
		result = libbde_volume_set_utf8_recovery_password(
		          *volume,
		          (uint8_t *) recovery_password,
		          string_length,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			goto on_error;
		}
	}
	if( startup_key != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_read_startup_key_wide(
		          *volume,
		          startup_key,
		          error );
#else
		result = libbde_volume_read_startup_key(
		          *volume,
		          startup_key,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read startup key.",
			 function );

			goto on_error;
		}
	}
	result = libbde_volume_open_file_io_handle(
	          *volume,
	          file_io_handle,
	          LIBBDE_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume != NULL )
	{
		libbde_volume_free(
		 volume,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source volume
 * Returns 1 if successful or -1 on error
 */
int bde_test_volume_close_source(
     libbde_volume_t **volume,
     libcerror_error_t **error )
{
	static char *function = "bde_test_volume_close_source";
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( libbde_volume_close(
	     *volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libbde_volume_free(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libbde_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_initialize(
     void )
{
	libbde_volume_t *volume         = NULL;
	libcerror_error_t *error        = NULL;
	int result                      = 0;

#if defined( HAVE_BDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libbde_volume_initialize(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_free(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libbde_volume_initialize(
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

	volume = (libbde_volume_t *) 0x12345678UL;

	result = libbde_volume_initialize(
	          &volume,
	          &error );

	volume = NULL;

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
		/* Test libbde_volume_initialize with malloc failing
		 */
		bde_test_malloc_attempts_before_fail = test_number;

		result = libbde_volume_initialize(
		          &volume,
		          &error );

		if( bde_test_malloc_attempts_before_fail != -1 )
		{
			bde_test_malloc_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libbde_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
		/* Test libbde_volume_initialize with memset failing
		 */
		bde_test_memset_attempts_before_fail = test_number;

		result = libbde_volume_initialize(
		          &volume,
		          &error );

		if( bde_test_memset_attempts_before_fail != -1 )
		{
			bde_test_memset_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libbde_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_free function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbde_volume_free(
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

/* Tests the libbde_volume_open function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_open(
     const system_character_t *source,
     const system_character_t *password,
     const system_character_t *recovery_password,
     const system_character_t *startup_key )
{
	char narrow_source[ 256 ];

	libbde_volume_t *volume  = NULL;
	libcerror_error_t *error = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = bde_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_initialize(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( recovery_password != NULL )
	{
		string_length = system_string_length(
		                 recovery_password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_recovery_password(
		          volume,
		          (uint16_t *) recovery_password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_recovery_password(
		          volume,
		          (uint8_t *) recovery_password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( startup_key != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_read_startup_key_wide(
		          volume,
		          startup_key,
		          &error );
#else
		result = libbde_volume_read_startup_key(
		          volume,
		          startup_key,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libbde_volume_open(
	          volume,
	          narrow_source,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_open(
	          NULL,
	          narrow_source,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open(
	          volume,
	          NULL,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open(
	          volume,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbde_volume_open(
	          volume,
	          narrow_source,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_free(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libbde_volume_open_wide function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_open_wide(
     const system_character_t *source,
     const system_character_t *password,
     const system_character_t *recovery_password,
     const system_character_t *startup_key )
{
	wchar_t wide_source[ 256 ];

	libbde_volume_t *volume  = NULL;
	libcerror_error_t *error = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = bde_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_initialize(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( recovery_password != NULL )
	{
		string_length = system_string_length(
		                 recovery_password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_recovery_password(
		          volume,
		          (uint16_t *) recovery_password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_recovery_password(
		          volume,
		          (uint8_t *) recovery_password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( startup_key != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_read_startup_key_wide(
		          volume,
		          startup_key,
		          &error );
#else
		result = libbde_volume_read_startup_key(
		          volume,
		          startup_key,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libbde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_open_wide(
	          NULL,
	          wide_source,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open_wide(
	          volume,
	          NULL,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open_wide(
	          volume,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_free(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libbde_volume_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_open_file_io_handle(
     const system_character_t *source,
     const system_character_t *password,
     const system_character_t *recovery_password,
     const system_character_t *startup_key )
{
	libbde_volume_t *volume          = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_initialize(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( recovery_password != NULL )
	{
		string_length = system_string_length(
		                 recovery_password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_recovery_password(
		          volume,
		          (uint16_t *) recovery_password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_recovery_password(
		          volume,
		          (uint8_t *) recovery_password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( startup_key != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_read_startup_key_wide(
		          volume,
		          startup_key,
		          &error );
#else
		result = libbde_volume_read_startup_key(
		          volume,
		          startup_key,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libbde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open_file_io_handle(
	          volume,
	          NULL,
	          LIBBDE_OPEN_READ,
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

	result = libbde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libbde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBBDE_OPEN_READ,
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
	result = libbde_volume_free(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_close function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libbde_volume_close(
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

/* Tests the libbde_volume_open and libbde_volume_close functions
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_open_close(
     const system_character_t *source,
     const system_character_t *password,
     const system_character_t *recovery_password,
     const system_character_t *startup_key )
{
	libbde_volume_t *volume  = NULL;
	libcerror_error_t *error = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = libbde_volume_initialize(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( recovery_password != NULL )
	{
		string_length = system_string_length(
		                 recovery_password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_set_utf16_recovery_password(
		          volume,
		          (uint16_t *) recovery_password,
		          string_length,
		          &error );
#else
		result = libbde_volume_set_utf8_recovery_password(
		          volume,
		          (uint8_t *) recovery_password,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	if( startup_key != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_read_startup_key_wide(
		          volume,
		          startup_key,
		          &error );
#else
		result = libbde_volume_read_startup_key(
		          volume,
		          startup_key,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbde_volume_open_wide(
	          volume,
	          source,
	          LIBBDE_OPEN_READ,
	          &error );
#else
	result = libbde_volume_open(
	          volume,
	          source,
	          LIBBDE_OPEN_READ,
	          &error );
#endif

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_close(
	          volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbde_volume_open_wide(
	          volume,
	          source,
	          LIBBDE_OPEN_READ,
	          &error );
#else
	result = libbde_volume_open(
	          volume,
	          source,
	          LIBBDE_OPEN_READ,
	          &error );
#endif

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbde_volume_close(
	          volume,
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
	result = libbde_volume_free(
	          &volume,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_signal_abort(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_signal_abort(
	          volume,
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
	result = libbde_volume_signal_abort(
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

/* Tests the libbde_volume_is_locked function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_is_locked(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_is_locked(
	          volume,
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
	result = libbde_volume_is_locked(
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

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

/* Tests the libbde_internal_volume_read_buffer_from_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int bde_test_internal_volume_read_buffer_from_file_io_handle(
     libbde_volume_t *volume )
{
	uint8_t buffer[ BDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libbde_volume_get_size(
	          volume,
	          &size,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = BDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < BDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
	              (libbde_internal_volume_t *) volume,
	              ( (libbde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libbde_volume_seek_offset(
		          volume,
		          -8,
		          SEEK_END,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
		              (libbde_internal_volume_t *) volume,
		              ( (libbde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
		              (libbde_internal_volume_t *) volume,
		              ( (libbde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		BDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % BDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( BDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libbde_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
		              (libbde_internal_volume_t *) volume,
		              ( (libbde_internal_volume_t *) volume )->file_io_handle,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libbde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libbde_volume_seek_offset(
			          volume,
			          0,
			          SEEK_SET,
			          &error );

			BDE_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			BDE_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
	              NULL,
	              ( (libbde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
	              (libbde_internal_volume_t *) volume,
	              ( (libbde_internal_volume_t *) volume )->file_io_handle,
	              NULL,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_internal_volume_read_buffer_from_file_io_handle(
	              (libbde_internal_volume_t *) volume,
	              ( (libbde_internal_volume_t *) volume )->file_io_handle,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

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

/* Tests the libbde_volume_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_read_buffer(
     libbde_volume_t *volume )
{
	uint8_t buffer[ BDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libbde_volume_get_size(
	          volume,
	          &size,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = BDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < BDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libbde_volume_read_buffer(
	              volume,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libbde_volume_seek_offset(
		          volume,
		          -8,
		          SEEK_END,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libbde_volume_read_buffer(
		              volume,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libbde_volume_read_buffer(
		              volume,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		BDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % BDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( BDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libbde_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libbde_volume_read_buffer(
		              volume,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libbde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libbde_volume_seek_offset(
			          volume,
			          0,
			          SEEK_SET,
			          &error );

			BDE_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			BDE_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libbde_volume_read_buffer(
	              NULL,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_volume_read_buffer(
	              volume,
	              NULL,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_volume_read_buffer(
	              volume,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BDE_TEST_RWLOCK )

	/* Test libbde_volume_read_buffer with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libbde_volume_read_buffer(
	              volume,
	              buffer,
	              BDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( bde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbde_volume_read_buffer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libbde_volume_read_buffer(
	              volume,
	              buffer,
	              BDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( bde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_volume_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_read_buffer_at_offset(
     libbde_volume_t *volume )
{
	uint8_t buffer[ BDE_TEST_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libbde_volume_get_size(
	          volume,
	          &size,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = BDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < BDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Read buffer on size boundary
		 */
		read_count = libbde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size - 8,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libbde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size + 8,
		              &error );

		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		BDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		if( size > 0 )
		{
			read_offset = (off64_t) random_number % size;
		}
		read_size = (size_t) random_number % BDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( BDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libbde_volume_read_buffer_at_offset: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libbde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              read_size,
		              read_offset,
		              &error );

		remaining_size = size - read_offset;

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libbde_volume_get_offset(
		          volume,
		          &offset,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	read_count = libbde_volume_read_buffer_at_offset(
	              NULL,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              NULL,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              -1,
	              &error );

	BDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BDE_TEST_RWLOCK )

	/* Test libbde_volume_read_buffer_at_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( bde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbde_volume_read_buffer_at_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libbde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              BDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( bde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_volume_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_seek_offset(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_END,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libbde_volume_seek_offset(
	          volume,
	          1024,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libbde_volume_seek_offset(
	          volume,
	          -512,
	          SEEK_CUR,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libbde_volume_seek_offset(
	          volume,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	offset = libbde_volume_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libbde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_SET,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libbde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_CUR,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libbde_volume_seek_offset(
	          volume,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_BDE_TEST_RWLOCK )

	/* Test libbde_volume_seek_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	bde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( bde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libbde_volume_seek_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	bde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	offset = libbde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( bde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		bde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		BDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_BDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_volume_get_offset function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_offset(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_offset(
	          volume,
	          &offset,
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
	result = libbde_volume_get_offset(
	          NULL,
	          &offset,
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

	result = libbde_volume_get_offset(
	          volume,
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

/* Tests the libbde_volume_get_size function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_size(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_size(
	          volume,
	          &size,
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
	result = libbde_volume_get_size(
	          NULL,
	          &size,
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

	result = libbde_volume_get_size(
	          volume,
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

/* Tests the libbde_volume_get_encryption_method function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_encryption_method(
     libbde_volume_t *volume )
{
	libcerror_error_t *error   = NULL;
	uint16_t encryption_method = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_encryption_method(
	          volume,
	          &encryption_method,
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
	result = libbde_volume_get_encryption_method(
	          NULL,
	          &encryption_method,
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

	result = libbde_volume_get_encryption_method(
	          volume,
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

/* Tests the libbde_volume_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_creation_time(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	uint64_t creation_time   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_creation_time(
	          volume,
	          &creation_time,
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
	result = libbde_volume_get_creation_time(
	          NULL,
	          &creation_time,
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

	result = libbde_volume_get_creation_time(
	          volume,
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

/* Tests the libbde_volume_get_utf8_description_size function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_utf8_description_size(
     libbde_volume_t *volume )
{
	libcerror_error_t *error         = NULL;
	size_t utf8_description_size     = 0;
	int result                       = 0;
	int utf8_description_size_is_set = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_utf8_description_size(
	          volume,
	          &utf8_description_size,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_description_size_is_set = result;

	/* Test error cases
	 */
	result = libbde_volume_get_utf8_description_size(
	          NULL,
	          &utf8_description_size,
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

	if( utf8_description_size_is_set != 0 )
	{
		result = libbde_volume_get_utf8_description_size(
		          volume,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_volume_get_utf8_description function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_utf8_description(
     libbde_volume_t *volume )
{
	uint8_t utf8_description[ 512 ];

	libcerror_error_t *error    = NULL;
	int result                  = 0;
	int utf8_description_is_set = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_utf8_description(
	          volume,
	          utf8_description,
	          512,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf8_description_is_set = result;

	/* Test error cases
	 */
	result = libbde_volume_get_utf8_description(
	          NULL,
	          utf8_description,
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

	if( utf8_description_is_set != 0 )
	{
		result = libbde_volume_get_utf8_description(
		          volume,
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

		result = libbde_volume_get_utf8_description(
		          volume,
		          utf8_description,
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

		result = libbde_volume_get_utf8_description(
		          volume,
		          utf8_description,
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

/* Tests the libbde_volume_get_utf16_description_size function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_utf16_description_size(
     libbde_volume_t *volume )
{
	libcerror_error_t *error          = NULL;
	size_t utf16_description_size     = 0;
	int result                        = 0;
	int utf16_description_size_is_set = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_utf16_description_size(
	          volume,
	          &utf16_description_size,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_description_size_is_set = result;

	/* Test error cases
	 */
	result = libbde_volume_get_utf16_description_size(
	          NULL,
	          &utf16_description_size,
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

	if( utf16_description_size_is_set != 0 )
	{
		result = libbde_volume_get_utf16_description_size(
		          volume,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libbde_volume_get_utf16_description function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_utf16_description(
     libbde_volume_t *volume )
{
	uint16_t utf16_description[ 512 ];

	libcerror_error_t *error     = NULL;
	int result                   = 0;
	int utf16_description_is_set = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_utf16_description(
	          volume,
	          utf16_description,
	          512,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	utf16_description_is_set = result;

	/* Test error cases
	 */
	result = libbde_volume_get_utf16_description(
	          NULL,
	          utf16_description,
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

	if( utf16_description_is_set != 0 )
	{
		result = libbde_volume_get_utf16_description(
		          volume,
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

		result = libbde_volume_get_utf16_description(
		          volume,
		          utf16_description,
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

		result = libbde_volume_get_utf16_description(
		          volume,
		          utf16_description,
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

/* Tests the libbde_volume_get_number_of_key_protectors function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_number_of_key_protectors(
     libbde_volume_t *volume )
{
	libcerror_error_t *error     = NULL;
	int number_of_key_protectors = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_number_of_key_protectors(
	          volume,
	          &number_of_key_protectors,
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
	result = libbde_volume_get_number_of_key_protectors(
	          NULL,
	          &number_of_key_protectors,
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

	result = libbde_volume_get_number_of_key_protectors(
	          volume,
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

/* Tests the libbde_volume_get_key_protector_by_index function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_key_protector_by_index(
     libbde_volume_t *volume )
{
	libbde_key_protector_t *key_protector = NULL;
	libcerror_error_t *error              = NULL;
	int result                            = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_key_protector_by_index(
	          volume,
	          0,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "key_protector",
	 key_protector );

	result = libbde_key_protector_free(
	          &key_protector,
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
	result = libbde_volume_get_key_protector_by_index(
	          NULL,
	          0,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbde_volume_get_key_protector_by_index(
	          volume,
	          -1,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbde_volume_get_key_protector_by_index(
	          volume,
	          0,
	          NULL,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

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
	if( key_protector != NULL )
	{
		libbde_key_protector_free(
		 &key_protector,
		 NULL );
	}
	return( 0 );
}

/* Tests the libbde_volume_get_key_protector function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_key_protector(
     libbde_volume_t *volume )
{
	libbde_key_protector_t *key_protector = NULL;
	libcerror_error_t *error              = NULL;
	int result                            = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_key_protector(
	          volume,
	          0,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "key_protector",
	 key_protector );

	result = libbde_key_protector_free(
	          &key_protector,
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
	result = libbde_volume_get_key_protector(
	          NULL,
	          0,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbde_volume_get_key_protector(
	          volume,
	          -1,
	          &key_protector,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

	BDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libbde_volume_get_key_protector(
	          volume,
	          0,
	          NULL,
	          &error );

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "key_protector",
	 key_protector );

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
	if( key_protector != NULL )
	{
		libbde_key_protector_free(
		 &key_protector,
		 NULL );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbde_volume_t *volume                      = NULL;
	libbfio_handle_t *file_io_handle             = NULL;
	libcerror_error_t *error                     = NULL;
	system_character_t *option_offset            = NULL;
	system_character_t *option_password          = NULL;
	system_character_t *option_recovery_password = NULL;
	system_character_t *option_startup_key       = NULL;
	system_character_t *source                   = NULL;
	system_integer_t option                      = 0;
	size_t string_length                         = 0;
	off64_t volume_offset                        = 0;
	int result                                   = 0;

	while( ( option = bde_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "o:p:r:s:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (system_integer_t) 's':
				option_startup_key = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	if( option_offset != NULL )
	{
		string_length = system_string_length(
		                 option_offset );

		result = bde_test_system_string_copy_from_64_bit_in_decimal(
		          option_offset,
		          string_length + 1,
		          (uint64_t *) &volume_offset,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( BDE_TEST_VOLUME_VERBOSE )
	libbde_notify_set_verbose(
	 1 );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
#endif

	BDE_TEST_RUN(
	 "libbde_volume_initialize",
	 bde_test_volume_initialize );

	BDE_TEST_RUN(
	 "libbde_volume_free",
	 bde_test_volume_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_range_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_range_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_range_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libbfio_file_range_set(
		          file_io_handle,
		          volume_offset,
		          0,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libbde_check_volume_signature_file_io_handle(
		          file_io_handle,
		          &error );

		BDE_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( ( result != 0 )
	 && ( volume_offset == 0 ) )
	{
		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open",
		 bde_test_volume_open,
		 source,
		 option_password,
		 option_recovery_password,
		 option_startup_key );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open_wide",
		 bde_test_volume_open_wide,
		 source,
		 option_password,
		 option_recovery_password,
		 option_startup_key );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open_file_io_handle",
		 bde_test_volume_open_file_io_handle,
		 source,
		 option_password,
		 option_recovery_password,
		 option_startup_key );

		BDE_TEST_RUN(
		 "libbde_volume_close",
		 bde_test_volume_close );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open_close",
		 bde_test_volume_open_close,
		 source,
		 option_password,
		 option_recovery_password,
		 option_startup_key );

	}
	if( result != 0 )
	{
		/* Initialize volume for tests
		 */
		result = bde_test_volume_open_source(
		          &volume,
		          file_io_handle,
		          option_password,
		          option_recovery_password,
		          option_startup_key,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BDE_TEST_ASSERT_IS_NOT_NULL(
		 "volume",
		 volume );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_signal_abort",
		 bde_test_volume_signal_abort,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

		/* TODO: add tests for libbde_volume_open_read */

		/* TODO: add tests for libbde_volume_open_read_keys_from_metadata */

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_is_locked",
		 bde_test_volume_is_locked,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_internal_volume_read_buffer_from_file_io_handle",
		 bde_test_internal_volume_read_buffer_from_file_io_handle,
		 volume );

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_read_buffer",
		 bde_test_volume_read_buffer,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_read_buffer_at_offset",
		 bde_test_volume_read_buffer_at_offset,
		 volume );

		/* TODO: add tests for libbde_volume_write_buffer */

		/* TODO: add tests for libbde_volume_write_buffer_at_offset */

#if defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT )

		/* TODO: add tests for libbde_internal_volume_seek_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBBDE_DLL_IMPORT ) */

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_seek_offset",
		 bde_test_volume_seek_offset,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_offset",
		 bde_test_volume_get_offset,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_size",
		 bde_test_volume_get_size,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_encryption_method",
		 bde_test_volume_get_encryption_method,
		 volume );

		/* TODO: add tests for libbde_volume_get_volume_identifier */

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_creation_time",
		 bde_test_volume_get_creation_time,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_utf8_description_size",
		 bde_test_volume_get_utf8_description_size,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_utf8_description",
		 bde_test_volume_get_utf8_description,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_utf16_description_size",
		 bde_test_volume_get_utf16_description_size,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_utf16_description",
		 bde_test_volume_get_utf16_description,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_number_of_key_protectors",
		 bde_test_volume_get_number_of_key_protectors,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_key_protector_by_index",
		 bde_test_volume_get_key_protector_by_index,
		 volume );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_get_key_protector",
		 bde_test_volume_get_key_protector,
		 volume );

		/* TODO: add tests for libbde_volume_set_keys */

		/* TODO: add tests for libbde_volume_set_utf8_password */

		/* TODO: add tests for libbde_volume_set_utf16_password */

		/* TODO: add tests for libbde_volume_set_utf8_recovery_password */

		/* TODO: add tests for libbde_volume_set_utf16_recovery_password */

		/* TODO: add tests for libbde_volume_read_startup_key */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		/* TODO: add tests for libbde_volume_read_startup_key_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO: add tests for libbde_volume_read_startup_key_file_io_handle */

		/* Clean up
		 */
		result = bde_test_volume_close_source(
		          &volume,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		BDE_TEST_ASSERT_IS_NULL(
		 "volume",
		 volume );

		BDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		BDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		BDE_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        BDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

