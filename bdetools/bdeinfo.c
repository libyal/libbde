/*
 * Shows information obtained from a Windows NT BitLocker Drive Encryptrion (BDE) volume
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <libsystem.h>

#include "bdeoutput.h"
#include "bdetools_libfdatetime.h"
#include "bdetools_libbde.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use bdeinfo to determine information about a Windows NT BitLocker\n"
	                 " Drive Encryption (BDE) volume\n\n" );

	fprintf( stream, "Usage: bdeinfo [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Prints volume information
 * Returns 1 if successful or -1 on error
 */
int bdeinfo_volume_info_fprint(
     FILE *stream,
     libbde_volume_t *volume,
     libbde_error_t **error )
{
	static char *function = "bdeinfo_volume_info_fprint";

	if( stream == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( volume == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
/* TODO */
	return( 1 );

on_error:
	return( -1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libbde_error_t *error                 = NULL;
	libbde_volume_t *volume               = NULL;
	libcstring_system_character_t *source = NULL;
	char *program                         = "bdeinfo";
	char *recovery_password               = NULL;
	size_t recovery_password_length       = 0;
	libcstring_system_integer_t option    = 0;
	int verbose                           = 0;

	recovery_password = "004301-051986-278476-162294-184228-193919-575828-424457";

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

        if( libsystem_initialize(
             "bdetools",
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	bdeoutput_version_fprint(
	 stdout,
	 program );

/* TODO add option to pass recovery password */

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "hvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				bdeoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libsystem_notify_set_verbose(
	 verbose );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
	libbde_notify_set_verbose(
	 verbose );

	if( libbde_volume_initialize(
	     &volume,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize volume.\n" );

		goto on_error;
	}
	recovery_password_length = libcstring_system_string_length(
	                            recovery_password );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbde_volume_set_utf8_recovery_password(
	     volume,
	     (uint16_t *) recovery_password,
	     recovery_password_length + 1,
	     &error ) != 1 )
#else
	if( libbde_volume_set_utf8_recovery_password(
	     volume,
	     (uint8_t *) recovery_password,
	     recovery_password_length + 1,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to set recovery password.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbde_volume_open_wide(
	     volume,
	     source,
	     LIBBDE_OPEN_READ,
	     &error ) != 1 )
#else
	if( libbde_volume_open(
	     volume,
	     source,
	     LIBBDE_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open volume: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ optind ] );

		goto on_error;
	}
	if( bdeinfo_volume_info_fprint(
	     stdout,
	     volume,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print volume information.\n" );

		goto on_error;
	}
	if( libbde_volume_close(
	     volume,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close volume.\n" );

		goto on_error;
	}
	if( libbde_volume_free(
	     &volume,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free volume.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		libbde_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libbde_volume_free(
		 &volume,
		 NULL );
	}
	return( EXIT_FAILURE );
}

