/* 
 * Info handle
 *
 * Copyright (c) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#include "bdetools_libbde.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct info_handle info_handle_t;

struct info_handle
{
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

int info_handle_open(
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

