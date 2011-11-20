/* 
 * Info handle
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

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

typedef struct info_handle info_handle_t;

struct info_handle
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

int info_handle_initialize(
     info_handle_t **info_handle,
     liberror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     liberror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     liberror_error_t **error );

int info_handle_set_password(
     info_handle_t *info_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int info_handle_set_recovery_password(
     info_handle_t *info_handle,
     const libcstring_system_character_t *string,
     liberror_error_t **error );

int info_handle_read_startup_key(
     info_handle_t *info_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int info_handle_close(
     info_handle_t *info_handle,
     liberror_error_t **error );

int info_handle_volume_fprint(
     info_handle_t *info_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

