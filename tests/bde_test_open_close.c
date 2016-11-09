/*
 * Library open close testing program
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "bde_test_libbde.h"
#include "bde_test_libcerror.h"
#include "bde_test_libcsystem.h"

/* Tests single open and close of a volume
 * Returns 1 if successful, 0 if not or -1 on error
 */
int bde_test_single_open_close_volume(
     const system_character_t *filename,
     const system_character_t *password,
     const system_character_t *recovery_password,
     int access_flags,
     int expected_result )
{
	libcerror_error_t *error = NULL;
	libbde_volume_t *volume  = NULL;
	static char *function    = "bde_test_single_open_close_volume";
	size_t string_length     = 0;
	int result               = 0;

	if( libbde_volume_initialize(
	     &volume,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libbde_volume_set_utf16_password(
		     volume,
		     (uint16_t *) password,
		     string_length,
		     &error ) != 1 )
#else
		if( libbde_volume_set_utf8_password(
		     volume,
		     (uint8_t *) password,
		     string_length,
		     &error ) != 1 )
#endif
		{
			libcerror_error_set(
			 &error,
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
		if( libbde_volume_set_utf16_recovery_password(
		     volume,
		     (uint16_t *) recovery_password,
		     string_length,
		     &error ) != 1 )
#else
		if( libbde_volume_set_utf8_recovery_password(
		     volume,
		     (uint8_t *) recovery_password,
		     string_length,
		     &error ) != 1 )
#endif
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbde_volume_open_wide(
	          volume,
	          filename,
	          access_flags,
	          &error );
#else
	result = libbde_volume_open(
	          volume,
	          filename,
	          access_flags,
	          &error );
#endif
	if( result == 1 )
	{
		if( libbde_volume_close(
		     volume,
		     &error ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close volume.",
			 function );

			goto on_error;
		}
	}
	if( libbde_volume_free(
	     &volume,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		goto on_error;
	}
	result = ( expected_result == result );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL);
	}
	return( -1 );
}

/* Tests multiple open and close of a volume
 * Returns 1 if successful, 0 if not or -1 on error
 */
int bde_test_multi_open_close_volume(
     const system_character_t *filename,
     const system_character_t *password,
     const system_character_t *recovery_password,
     int access_flags,
     int expected_result )
{
	libcerror_error_t *error = NULL;
	libbde_volume_t *volume  = NULL;
	static char *function    = "bde_test_multi_open_close_volume";
	size_t string_length     = 0;
	int result               = 0;

	if( libbde_volume_initialize(
	     &volume,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libbde_volume_set_utf16_password(
		     volume,
		     (uint16_t *) password,
		     string_length,
		     &error ) != 1 )
#else
		if( libbde_volume_set_utf8_password(
		     volume,
		     (uint8_t *) password,
		     string_length,
		     &error ) != 1 )
#endif
		{
			libcerror_error_set(
			 &error,
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
		if( libbde_volume_set_utf16_recovery_password(
		     volume,
		     (uint16_t *) recovery_password,
		     string_length,
		     &error ) != 1 )
#else
		if( libbde_volume_set_utf8_recovery_password(
		     volume,
		     (uint8_t *) recovery_password,
		     string_length,
		     &error ) != 1 )
#endif
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbde_volume_open_wide(
	          volume,
	          filename,
	          access_flags,
	          &error );
#else
	result = libbde_volume_open(
	          volume,
	          filename,
	          access_flags,
	          &error );
#endif
	if( result == 1 )
	{
		if( libbde_volume_close(
		     volume,
		     &error ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close volume.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbde_volume_open_wide(
		          volume,
		          filename,
		          access_flags,
		          &error );
#else
		result = libbde_volume_open(
		          volume,
		          filename,
		          access_flags,
		          &error );
#endif
		if( result == 1 )
		{
			if( libbde_volume_close(
			     volume,
			     &error ) != 0 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				goto on_error;
			}
		}
	}
	if( libbde_volume_free(
	     &volume,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		goto on_error;
	}
	result = ( expected_result == result );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL);
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t *option_password          = NULL;
	system_character_t *option_recovery_password = NULL;
	system_character_t *source                   = NULL;
	system_integer_t option                      = 0;

	while( ( option = libcsystem_getopt(
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
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( BDE_TEST_OPEN_CLOSE_VERBOSE )
	libbde_notify_set_verbose(
	 1 );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
#endif

	/* Case 0: single open and close of a volume using filename
	 */
	fprintf(
	 stdout,
	 "Testing single open close of: %s with access: read\t",
	 source );

	if( bde_test_single_open_close_volume(
	     source,
	     option_password,
	     option_recovery_password,
	     LIBBDE_OPEN_READ,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Testing single open close of: NULL with access: read\t" );

	if( bde_test_single_open_close_volume(
	     NULL,
	     option_password,
	     option_recovery_password,
	     LIBBDE_OPEN_READ,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Testing single open close of: %s with access: write\t",
	 source );

	if( bde_test_single_open_close_volume(
	     source,
	     option_password,
	     option_recovery_password,
	     LIBBDE_OPEN_WRITE,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	/* Case 1: multiple open and close of a volume using filename
	 */
	fprintf(
	 stdout,
	 "Testing multi open close of: %s with access: read\t",
	 source );

	if( bde_test_multi_open_close_volume(
	     source,
	     option_password,
	     option_recovery_password,
	     LIBBDE_OPEN_READ,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test multi open close.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

