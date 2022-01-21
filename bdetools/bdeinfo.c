/*
 * Shows information obtained from a BitLocker Drive Encrypted (BDE) volume
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
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "bdetools_getopt.h"
#include "bdetools_libbde.h"
#include "bdetools_libcerror.h"
#include "bdetools_libclocale.h"
#include "bdetools_libcnotify.h"
#include "bdetools_output.h"
#include "bdetools_signal.h"
#include "bdetools_unused.h"
#include "info_handle.h"

info_handle_t *bdeinfo_info_handle = NULL;
int bdeinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use bdeinfo to determine information about a BitLocker Drive\n"
	                 " Encrypted (BDE) volume\n\n" );

	fprintf( stream, "Usage: bdeinfo [ -k keys ] [ -o offset ] [ -p password ]\n"
	                 "               [ -r password ] [ -s filename ] [ -huvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-k:     the full volume encryption key and tweak key\n"
	                 "\t        formatted in base16 and separated by a : character\n"
	                 "\t        e.g. FVEK:TWEAK\n" );
	fprintf( stream, "\t-o:     specify the volume offset in bytes\n" );
	fprintf( stream, "\t-p:     specify the password/passphrase\n" );
	fprintf( stream, "\t-r:     specify the recovery password\n" );
	fprintf( stream, "\t-s:     specify the file containing the startup key.\n"
	                 "\t        typically this file has the extension .BEK\n" );
	fprintf( stream, "\t-u:     unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for bdeinfo
 */
void bdeinfo_signal_handler(
      bdetools_signal_t signal BDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "bdeinfo_signal_handler";

	BDETOOLS_UNREFERENCED_PARAMETER( signal )

	bdeinfo_abort = 1;

	if( bdeinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     bdeinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libbde_error_t *error                        = NULL;
	system_character_t *option_keys              = NULL;
	system_character_t *option_password          = NULL;
	system_character_t *option_recovery_password = NULL;
	system_character_t *option_startup_key_path  = NULL;
	system_character_t *option_volume_offset     = NULL;
	system_character_t *source                   = NULL;
	char *program                                = "bdeinfo";
	system_integer_t option                      = 0;
	int result                                   = 0;
	int unattended_mode                          = 0;
	int verbose                                  = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "bdetools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( bdetools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	bdetools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = bdetools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "hk:o:p:r:s:uvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'k':
				option_keys = optarg;

				break;

			case (system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (system_integer_t) 's':
				option_startup_key_path = optarg;

				break;

			case (system_integer_t) 'u':
				unattended_mode = 1;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				bdetools_output_copyright_fprint(
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

	libcnotify_verbose_set(
	 verbose );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
	libbde_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &bdeinfo_info_handle,
	     unattended_mode,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_keys != NULL )
	{
		if( info_handle_set_keys(
		     bdeinfo_info_handle,
		     option_keys,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set keys.\n" );

			goto on_error;
		}
	}
	if( option_password != NULL )
	{
		if( info_handle_set_password(
		     bdeinfo_info_handle,
		     option_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set password.\n" );

			goto on_error;
		}
	}
	if( option_recovery_password != NULL )
	{
		if( info_handle_set_recovery_password(
		     bdeinfo_info_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
	if( option_startup_key_path != NULL )
	{
		if( info_handle_set_startup_key(
		     bdeinfo_info_handle,
		     option_startup_key_path,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set startup key.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( info_handle_set_volume_offset(
		     bdeinfo_info_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set volume offset.\n" );

			goto on_error;
		}
	}
	result = info_handle_open(
	          bdeinfo_info_handle,
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( info_handle_volume_fprint(
	     bdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print volume information.\n" );

		goto on_error;
	}
	if( info_handle_close(
	     bdeinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &bdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( bdeinfo_info_handle != NULL )
	{
		info_handle_free(
		 &bdeinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

