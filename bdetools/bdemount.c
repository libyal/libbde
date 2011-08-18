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

#if defined( HAVE_ERRNO_H )
#include <errno.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <libsystem.h>

#if defined( HAVE_FUSE_H )
#define FUSE_USE_VERSION	26

#include <fuse.h>
#endif

#include "bdeoutput.h"
#include "bdetools_libfdatetime.h"
#include "bdetools_libbde.h"
#include "mount_handle.h"

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
	fprintf( stream, "Usage: bdemount [ - o offset ] [ -r password ]\n"
	                 "                [ -hvV ] source mount_point\n\n" );

	fprintf( stream, "\tsource:      the source file or device\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-h:          shows this help\n" );
#ifdef TODO
	fprintf( stream, "\t-k:          specify the file containing the external key.\n"
	                 "\t             typically this file has the extension .BEK\n" );
#endif
	fprintf( stream, "\t-o:          specify the volume offset\n" );
#ifdef TODO
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

#if defined( HAVE_FUSE_H )

#if ( SIZEOF_OFF_T != 8 ) && ( SIZEOF_OFF_T != 4 )
#error Size of off_t not supported
#endif

static char *bdemount_fuse_path         = "/bde1";
static size_t bdemount_fuse_path_length = 5;

/* Opens a file
 * Returns 0 if successful or a negative errno value otherwise
 */
int bdemount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	liberror_error_t *error = NULL;
	static char *function   = "bdemount_fuse_open";
	size_t path_length      = 0;
	int result              = 0;

	if( path == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ( path_length != bdemount_fuse_path_length )
	 || ( libcstring_narrow_string_compare(
	       path,
	       bdemount_fuse_path,
	       bdemount_fuse_path_length ) != 0 ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns number of bytes read if successful or a negative errno value otherwise
 */
int bdemount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info )
{
	liberror_error_t *error = NULL;
	static char *function   = "bdemount_fuse_read";
	size_t path_length      = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( path == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( size > (size_t) INT_MAX )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ( path_length != bdemount_fuse_path_length )
	 || ( libcstring_narrow_string_compare(
	       path,
	       bdemount_fuse_path,
	       bdemount_fuse_path_length ) != 0 ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
	if( libbde_volume_seek_offset(
	     bdemount_mount_handle->input_volume,
	     (off64_t) offset,
	     SEEK_SET,
	     &error ) == -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in input volume.",
		 function );

		result = -EIO;

		goto on_error;
	}
	read_count = libbde_volume_read_buffer(
	              bdemount_mount_handle->input_volume,
	              buffer,
	              size,
	              &error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from input volume.",
		 function );

		result = -EIO;

		goto on_error;
	}
	return( (int) read_count );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int bdemount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info )
{
	liberror_error_t *error = NULL;
	static char *function   = "bdemount_fuse_readdir";
	size_t path_length      = 0;
	int result              = 0;

	if( path == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( ( path_length != 1 )
	 || ( path[ 0 ] != '/' ) )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
#ifdef X
	if( file_info == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info->fh == (uint64_t) NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file info - missing file handle.",
		 function );

		result = -EBADF;

		goto on_error;
	}
#endif
	if( filler(
	     buffer,
	     ".",
	     NULL,
	     0 ) == 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( filler(
	     buffer,
	     "..",
	     NULL,
	     0 ) == 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( filler(
	     buffer,
	     &( bdemount_fuse_path[ 1 ] ),
	     NULL,
	     0 ) == 1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the file stat info
 * Returns 0 if successful or a negative errno value otherwise
 */
int bdemount_fuse_getattr(
     const char *path,
     struct stat *stat_info )
{
	liberror_error_t *error = NULL;
	static char *function   = "bdemount_fuse_getattr";
	size64_t volume_size    = 0;
	size_t path_length      = 0;
	int result              = -ENOENT;

	if( path == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( stat_info == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	path_length = libcstring_narrow_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] == '/' )
		{
			stat_info->st_mode  = S_IFDIR | 0755;
			stat_info->st_nlink = 2;

			result = 0;
		}
	}
	else if( path_length == bdemount_fuse_path_length )
	{
		if( libcstring_narrow_string_compare(
		     path,
		     bdemount_fuse_path,
		     bdemount_fuse_path_length ) == 0 )
		{
			stat_info->st_mode  = S_IFREG | 0444;
			stat_info->st_nlink = 1;

			if( libbde_volume_get_size(
			     bdemount_mount_handle->input_volume,
			     &volume_size,
			     &error ) != 1 )
			{
				liberror_error_set(
				 &error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unsupported to retrieve volume size.",
				 function );

				result = -EBADFD;

				goto on_error;
			}
#if SIZEOF_OFF_T == 4
			if( volume_size > (size64_t) UINT32_MAX )
			{
				liberror_error_set(
				 &error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: unsupported to volume size value out of bounds.",
				 function );

				result = -ERANGE;

				goto on_error;
			}
#endif
			stat_info->st_size = (off_t) volume_size;

			result = 0;
		}
	}
	if( result == 0 )
	{
		stat_info->st_atime = 0;
		stat_info->st_mtime = 0;
		stat_info->st_ctime = 0;
		stat_info->st_uid   = geteuid();
		stat_info->st_gid   = getegid();
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
	return( result );
}

#endif /* defined( HAVE_FUSE_H ) */

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libbde_error_t *error                                   = NULL;
	libcstring_system_character_t *mount_point              = NULL;
	libcstring_system_character_t *option_external_key_file = NULL;
	libcstring_system_character_t *option_password          = NULL;
	libcstring_system_character_t *option_recovery_password = NULL;
	libcstring_system_character_t *option_volume_offset     = NULL;
	libcstring_system_character_t *source                   = NULL;
	char *program                                           = "bdemount";
	libcstring_system_integer_t option                      = 0;
	size_t string_length                                    = 0;
	int verbose                                             = 0;

#if defined( HAVE_FUSE_H )
	struct fuse_operations bdemount_fuse_operations;
	struct fuse_chan *bdemount_fuse_channel                 = NULL;
	struct fuse *bdemount_fuse_handle                       = NULL;

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
	                   _LIBCSTRING_SYSTEM_STRING( "hk:o:p:r:vV" ) ) ) != (libcstring_system_integer_t) -1 )
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

			case (libcstring_system_integer_t) 'o':
				option_volume_offset = optarg;

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
	source = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ optind ];

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
	if( option_volume_offset != NULL )
	{
		string_length = libcstring_system_string_length(
				 option_volume_offset );

		if( libsystem_string_to_uint64(
		     option_volume_offset,
		     string_length + 1,
		     (uint64_t *) &( bdemount_mount_handle->volume_offset ),
		     &error ) != 1 )
		{
			libsystem_notify_print_error_backtrace(
			 error );
			liberror_error_free(
			 &error );

			bdemount_mount_handle->volume_offset = 0;

			fprintf(
			 stderr,
			 "Unsupported volume offset defaulting to: %" PRIu64 ".\n",
			 bdemount_mount_handle->volume_offset );
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
	bdemount_fuse_operations.open    = &bdemount_fuse_open;
	bdemount_fuse_operations.read    = &bdemount_fuse_read;
	bdemount_fuse_operations.readdir = &bdemount_fuse_readdir;
	bdemount_fuse_operations.getattr = &bdemount_fuse_getattr;

	bdemount_fuse_channel = fuse_mount(
	                         mount_point,
	                         NULL );

	if( bdemount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	bdemount_fuse_handle = fuse_new(
	                        bdemount_fuse_channel,
	                        NULL,
	                        &bdemount_fuse_operations,
	                        sizeof( struct fuse_operations ),
	                        bdemount_mount_handle );
	               
	if( bdemount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	result = fuse_loop(
	          bdemount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse.\n" );

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

