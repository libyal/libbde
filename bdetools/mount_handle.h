/* 
 * Mount handle
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

#if !defined( _MOUNT_HANDLE_H )
#define _MOUNT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "bdetools_libbde.h"
#include "bdetools_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_handle mount_handle_t;

struct mount_handle
{
	/* The volume offset
	 */
	off64_t volume_offset;

	/* The libbfio input file IO handle
	 */
	libbfio_handle_t *input_file_io_handle;

	/* The libbde input volume
	 */
	libbde_volume_t *input_volume;

	/* The nofication output stream
	 */
	FILE *notify_stream;
};

int mount_handle_initialize(
     mount_handle_t **mount_handle,
     liberror_error_t **error );

int mount_handle_free(
     mount_handle_t **mount_handle,
     liberror_error_t **error );

int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     liberror_error_t **error );

int mount_handle_set_password(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int mount_handle_set_recovery_password(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int mount_handle_read_startup_key(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int mount_handle_open_input(
     mount_handle_t *mount_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int mount_handle_close(
     mount_handle_t *mount_handle,
     liberror_error_t **error );

ssize_t mount_handle_read_buffer(
         mount_handle_t *mount_handle,
         uint8_t *buffer,
         size_t size,
         liberror_error_t **error );

off64_t mount_handle_seek_offset(
         mount_handle_t *mount_handle,
         off64_t offset,
         int whence,
         liberror_error_t **error );

int mount_handle_get_size(
     mount_handle_t *mount_handle,
     size64_t *size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

