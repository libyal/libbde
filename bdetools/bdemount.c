/*
 * Mounts a Windows NT BitLocker Drive Encryption (BDE) encrypted volume
 *
 * Copyright (C) 2011, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

	fprintf( stream, "Usage: bdemount [ - o offset ] [ -p password ] [ -r password ]\n"
	                 "                [ -s filename ] [ -hvV ] source mount_point\n\n" );

	fprintf( stream, "\tsource:      the source file or device\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-o:          specify the volume offset\n" );
	fprintf( stream, "\t-p:          specify the password/passphrase\n" );
	fprintf( stream, "\t-r:          specify the recovery password\n" );
	fprintf( stream, "\t-s:          specify the file containing the startup key.\n"
	                 "\t             typically this file has the extension .BEK\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n"
	                 "\t             bdemount will remain running in the foregroud\n" );
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
	if( mount_handle_seek_offset(
	     bdemount_mount_handle,
	     (off64_t) offset,
	     SEEK_SET,
	     &error ) == -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in mount handle.",
		 function );

		result = -EIO;

		goto on_error;
	}
	read_count = mount_handle_read_buffer(
	              bdemount_mount_handle,
	              (uint8_t *) buffer,
	              size,
	              &error );

	if( read_count == -1 )
	{
		liberror_error_set(
		 &error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from mount handle.",
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

			if( mount_handle_get_size(
			     bdemount_mount_handle,
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
		stat_info->st_atime = libsystem_date_time_time(
		                       NULL );

		stat_info->st_mtime = libsystem_date_time_time(
		                       NULL );

		stat_info->st_ctime = libsystem_date_time_time(
		                       NULL );

#if defined( HAVE_GETEUID )
		stat_info->st_uid = geteuid();
#else
		stat_info->st_uid = 0;
#endif
#if defined( HAVE_GETEGID )
		stat_info->st_gid = getegid();
#else
		stat_info->st_gid = 0;
#endif
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
	libbde_error_t *error                                      = NULL;
	libcstring_system_character_t *mount_point                 = NULL;
	libcstring_system_character_t *option_password             = NULL;
	libcstring_system_character_t *option_recovery_password    = NULL;
	libcstring_system_character_t *option_startup_key_filename = NULL;
	libcstring_system_character_t *option_volume_offset        = NULL;
	libcstring_system_character_t *source                      = NULL;
	char *program                                              = "bdemount";
	libcstring_system_integer_t option                         = 0;
	size_t string_length                                       = 0;
	int result                                                 = 0;
	int verbose                                                = 0;

#if defined( HAVE_FUSE_H )
	struct fuse_operations bdemount_fuse_operations;
	struct fuse_chan *bdemount_fuse_channel                    = NULL;
	struct fuse *bdemount_fuse_handle                          = NULL;
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
	                   _LIBCSTRING_SYSTEM_STRING( "ho:p:r:s:vV" ) ) ) != (libcstring_system_integer_t) -1 )
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

			case (libcstring_system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (libcstring_system_integer_t) 'p':
				option_password = optarg;

				break;

			case (libcstring_system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (libcstring_system_integer_t) 's':
				option_startup_key_filename = optarg;

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
	if( option_password != NULL )
	{
/* TODO */
		fprintf(
		 stderr,
		 "Password not yet supported.\n" );

		goto on_error;

		if( mount_handle_set_password(
		     bdemount_mount_handle,
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
	if( option_startup_key_filename != NULL )
	{
		if( mount_handle_read_startup_key(
		     bdemount_mount_handle,
		     option_startup_key_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to read startup key.\n" );

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
	result = mount_handle_open_input(
	          bdemount_mount_handle,
	          source,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "Unable to unlock keys.\n" );

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
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          bdemount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 bdemount_fuse_handle );

	return( EXIT_SUCCESS );
#else
	fprintf(
	 stderr,
	 "No sub system to mount BDE volume.\n" );

	return( EXIT_FAILURE );
#endif

on_error:
	if( error != NULL )
	{
		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );
	}
#if defined( HAVE_FUSE_H )
	if( bdemount_fuse_handle != NULL )
	{
		fuse_destroy(
		 bdemount_fuse_handle );
	}
#endif
	if( bdemount_mount_handle != NULL )
	{
		mount_handle_free(
		 &bdemount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

