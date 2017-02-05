/*
 * Library volume type testing program
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bde_test_getopt.h"
#include "bde_test_libbde.h"
#include "bde_test_libcerror.h"
#include "bde_test_libclocale.h"
#include "bde_test_libuna.h"
#include "bde_test_macros.h"
#include "bde_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make bde_test_volume generate verbose output
#define BDE_TEST_VOLUME_VERBOSE
 */

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int bde_test_volume_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "bde_test_volume_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int bde_test_volume_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "bde_test_volume_get_wide_source";
	size_t source_length    = 0;
	size_t string_length    = 0;
	size_t wide_source_size = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int bde_test_volume_open_source(
     libbde_volume_t **volume,
     const system_character_t *source,
     const system_character_t *password,
     const system_character_t *recovery_password,
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
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbde_volume_open_wide(
	          *volume,
	          source,
	          LIBBDE_OPEN_READ,
	          error );
#else
	result = libbde_volume_open(
	          *volume,
	          source,
	          LIBBDE_OPEN_READ,
	          error );
#endif
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

	BDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        BDE_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	volume = NULL;

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
     const system_character_t *recovery_password )
{
	char narrow_source[ 256 ];

	libbde_volume_t *volume  = NULL;
	libcerror_error_t *error = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = bde_test_volume_get_narrow_source(
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
     const system_character_t *recovery_password )
{
	wchar_t wide_source[ 256 ];

	libbde_volume_t *volume  = NULL;
	libcerror_error_t *error = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = bde_test_volume_get_wide_source(
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
     const system_character_t *recovery_password )
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

/* Tests the libbde_volume_get_offset function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_offset(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int offset_is_set        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_offset(
	          volume,
	          &offset,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset_is_set = result;

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

	if( offset_is_set != 0 )
	{
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

/* Tests the libbde_volume_get_size function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_size(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	int result               = 0;
	int size_is_set          = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_size(
	          volume,
	          &size,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size_is_set = result;

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

	if( size_is_set != 0 )
	{
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

/* Tests the libbde_volume_get_encryption_method function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_encryption_method(
     libbde_volume_t *volume )
{
	libcerror_error_t *error     = NULL;
	uint16_t encryption_method   = 0;
	int encryption_method_is_set = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_encryption_method(
	          volume,
	          &encryption_method,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	encryption_method_is_set = result;

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

	if( encryption_method_is_set != 0 )
	{
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

/* Tests the libbde_volume_get_creation_time function
 * Returns 1 if successful or 0 if not
 */
int bde_test_volume_get_creation_time(
     libbde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	uint64_t creation_time   = 0;
	int creation_time_is_set = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_creation_time(
	          volume,
	          &creation_time,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	creation_time_is_set = result;

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

	if( creation_time_is_set != 0 )
	{
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
	libcerror_error_t *error            = NULL;
	int number_of_key_protectors        = 0;
	int number_of_key_protectors_is_set = 0;
	int result                          = 0;

	/* Test regular cases
	 */
	result = libbde_volume_get_number_of_key_protectors(
	          volume,
	          &number_of_key_protectors,
	          &error );

	BDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	BDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_key_protectors_is_set = result;

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

	if( number_of_key_protectors_is_set != 0 )
	{
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
	libcerror_error_t *error                     = NULL;
	system_character_t *option_password          = NULL;
	system_character_t *option_recovery_password = NULL;
	system_character_t *source                   = NULL;
	system_integer_t option                      = 0;
	int result                                   = 0;

	while( ( option = bde_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "p:r:" ) ) ) != (system_integer_t) -1 )
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

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'r':
				option_recovery_password = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
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
		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open",
		 bde_test_volume_open,
		 source,
		 option_password,
		 option_recovery_password );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open_wide",
		 bde_test_volume_open_wide,
		 source,
		 option_password,
		 option_recovery_password );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBBDE_HAVE_BFIO )

		/* TODO add test for libbde_volume_open_file_io_handle */

#endif /* defined( LIBBDE_HAVE_BFIO ) */

		BDE_TEST_RUN(
		 "libbde_volume_close",
		 bde_test_volume_close );

		BDE_TEST_RUN_WITH_ARGS(
		 "libbde_volume_open_close",
		 bde_test_volume_open_close,
		 source,
		 option_password,
		 option_recovery_password );

		/* Initialize test
		 */
		result = bde_test_volume_open_source(
		          &volume,
		          source,
		          option_password,
		          option_recovery_password,
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

#if defined( __GNUC__ )

		/* TODO: add tests for libbde_volume_open_read */

		/* TODO: add tests for libbde_volume_open_read_keys_from_metadata */

#endif /* defined( __GNUC__ ) */

		/* TODO: add tests for libbde_volume_is_locked */

		/* TODO: add tests for libbde_volume_read_buffer */

		/* TODO: add tests for libbde_volume_read_buffer_at_offset */

		/* TODO: add tests for libbde_volume_write_buffer */

		/* TODO: add tests for libbde_volume_write_buffer_at_offset */

		/* TODO: add tests for libbde_volume_seek_offset */

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

		/* TODO: add tests for libbde_volume_get_key_protector */

		/* TODO: add tests for libbde_volume_set_keys */

		/* TODO: add tests for libbde_volume_set_utf8_password */

		/* TODO: add tests for libbde_volume_set_utf16_password */

		/* TODO: add tests for libbde_volume_set_utf8_recovery_password */

		/* TODO: add tests for libbde_volume_set_utf16_recovery_password */

		/* TODO: add tests for libbde_volume_read_startup_key */

		/* TODO: add tests for libbde_volume_read_startup_key_wide */

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
		bde_test_volume_close_source(
		 &volume,
		 NULL );
	}
	return( EXIT_FAILURE );
}

