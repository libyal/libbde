/*
 * Notification functions
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
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "libbde_notify.h"

#if !defined( HAVE_LOCAL_LIBBDE )

/* Sets the verbose notification
 */
void libbde_notify_set_verbose(
      int verbose )
{
	libnotify_set_verbose(
	 verbose );
}

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
int libbde_notify_set_stream(
     FILE *stream,
     liberror_error_t **error )
{
	static char *function = "libbde_notify_set_stream";

	if( libnotify_set_stream(
	     stream,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
int libbde_notify_stream_open(
     const char *filename,
     liberror_error_t **error )
{
	static char *function = "libbde_notify_stream_open";

	if( libnotify_stream_open(
	     filename,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open stream.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
int libbde_notify_stream_close(
     liberror_error_t **error )
{
	static char *function = "libbde_notify_stream_close";

	if( libnotify_stream_close(
	     error ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open stream.",
		 function );

		return( -1 );
	}
	return( 0 );
}

#endif

