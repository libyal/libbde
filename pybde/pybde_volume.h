/*
 * Python object wrapper of libbde_volume_t
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

#if !defined( _PYBDE_VOLUME_H )
#define _PYBDE_VOLUME_H

#include <common.h>
#include <types.h>

#include "pybde_libbde.h"
#include "pybde_libbfio.h"
#include "pybde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pybde_volume pybde_volume_t;

struct pybde_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libbde volume
	 */
	libbde_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pybde_volume_object_methods[];
extern PyTypeObject pybde_volume_type_object;

int pybde_volume_init(
     pybde_volume_t *pybde_volume );

void pybde_volume_free(
      pybde_volume_t *pybde_volume );

PyObject *pybde_volume_signal_abort(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_open(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_open_file_object(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_close(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_is_locked(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_unlock(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_read_buffer(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_read_buffer_at_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_seek_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_get_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_size(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_encryption_method(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_volume_identifier(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_creation_time(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_creation_time_as_integer(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_description(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_number_of_key_protectors(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_get_key_protector_by_index(
           PyObject *pybde_volume,
           int key_protector_index );

PyObject *pybde_volume_get_key_protector(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_get_key_protectors(
           pybde_volume_t *pybde_volume,
           PyObject *arguments );

PyObject *pybde_volume_set_keys(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_set_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_set_recovery_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pybde_volume_read_startup_key(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYBDE_VOLUME_H ) */

