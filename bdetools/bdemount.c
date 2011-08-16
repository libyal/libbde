/*
 * Mounts a Windows NT BitLocker Drive Encryptrion (BDE) volume
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
#include "bdetools_fuse.h"
#include "bdetools_libfdatetime.h"
#include "bdetools_libbde.h"
#include "mount_handle.h"

#if defined( HAVE_FUSE_H )
static struct fuse_operations bdemount_fuse_operations;
#endif

mount_handle_t *bdemount_mount_handle = NULL;
int bdemount_abort                    = 0;

/* Prints the executable usage mountrmation
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use bdemount to mount a Windows NT BitLocker Drive Encryption (BDE)\n"
	                 " volume\n\n" );

#ifdef TODO
	fprintf( stream, "Usage: bdemount [ -k file ] [ -p password ] [ -r password ]\n"
	                 "                [ -hvV ] source mount_point\n\n" );
#endif
	fprintf( stream, "Usage: bdemount [ -r password ] [ -hvV ] source mount_point\n\n" );

	fprintf( stream, "\tsource:      the source file or device\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-h:          shows this help\n" );
#ifdef TODO
	fprintf( stream, "\t-k:          specify the file containing the external key.\n"
	                 "\t             typically this file has the extension .BEK\n" );
	fprintf( stream, "\t-p:          specify the password\n" );
#endif
	fprintf( stream, "\t-r:          specify the recovery password\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n" );
	fprintf( stream, "\t-V:          print version\n" );
}

/* Signal handler for bdemount
 */
void bdemount_signal_handler(
      libsystem_signal_t signal )
{
	liberror_error_t *error = NULL;
	static char *function   = "bdemount_signal_handler";

	bdemount_abort = 1;

	if( bdemount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     bdemount_mount_handle,
		     &error ) != 1 )
		{
			libsystem_notify_printf(
			 "%s: unable to signal mount handle to abort.\n",
			 function );

			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libsystem_file_io_close(
	     0 ) != 0 )
	{
		libsystem_notify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libbde_error_t *error                                   = NULL;
	libcstring_system_character_t *option_external_key_file = NULL;
	libcstring_system_character_t *option_password          = NULL;
	libcstring_system_character_t *option_recovery_password = NULL;
	libcstring_system_character_t *source                   = NULL;
	char *program                                           = "bdemount";
	libcstring_system_integer_t option                      = 0;
	int verbose                                             = 0;

#if defined( HAVE_FUSE_H )
	int result                                              = 0;
#endif

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

/* TODO add option to pass extenal password/key, what about BEK file */

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "hk:p:r:vV" ) ) ) != (libcstring_system_integer_t) -1 )
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

			case (libcstring_system_integer_t) 'k':
				option_external_key_file = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				option_password = optarg;

				break;

			case (libcstring_system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

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

	if( ( optind + 1 ) == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	libsystem_notify_set_verbose(
	 verbose );
	libbde_notify_set_stream(
	 stderr,
	 NULL );
	libbde_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &bdemount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_external_key_file != NULL )
	{
/* TODO */
		fprintf(
		 stderr,
		 "External key file not yet supported.\n" );

		goto on_error;
	}
	if( option_password != NULL )
	{
/* TODO */
		fprintf(
		 stderr,
		 "Password not yet supported.\n" );

		goto on_error;
	}
/* TODO make this a else if ? */
	if( option_recovery_password != NULL )
	{
		if( mount_handle_set_recovery_password(
		     bdemount_mount_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
	if( mount_handle_open(
	     bdemount_mount_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		goto on_error;
	}
#if defined( HAVE_FUSE_H )
	if( memory_set(
	     &bdemount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	bdemount_fuse_operations.open     = &mount_handle_fuse_open;
	bdemount_fuse_operations.read     = &mount_handle_fuse_read;
	bdemount_fuse_operations.readdir  = &mount_handle_fuse_readdir;
	bdemount_fuse_operations.fgetattr = &mount_handle_fuse_fgetattr;

/* TODO pass stripped arguments */
	result = fuse_main(
	          optind - argc + 1,
	          (char **) &( argv[ optind ] ),
	          &bdemount_fuse_operations,
	          bdemount_mount_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse main.\n" );

		goto on_error;
	}
#endif
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	if( bdemount_mount_handle != NULL )
	{
		mount_handle_free(
		 &bdemount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

#ifdef TODO
/* TODO test */
	uint8_t buffer[ 512 ];

	if( libbde_volume_seek_offset(
	     volume,
	     0x0d2ba000,
	     SEEK_SET,
	     &error ) == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to seek offset in volume.\n" );

		goto on_error;
	}
	if( libbde_volume_read_buffer(
	     volume,
	     buffer,
	     512,
	     &error ) != 512 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from volume.\n" );

		goto on_error;
	}
	libsystem_notify_print_data(
	 buffer,
	 512 );

	if( libbde_volume_read_buffer(
	     volume,
	     buffer,
	     512,
	     &error ) != 512 )
	{
		fprintf(
		 stderr,
		 "Unable to read buffer from volume.\n" );

		goto on_error;
	}
	libsystem_notify_print_data(
	 buffer,
	 512 );
/* TODO test */

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
#endif

