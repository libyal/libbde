/*
 * Python object definition of the key protectors sequence and iterator
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYBDE_KEY_PROTECTORS_H )
#define _PYBDE_KEY_PROTECTORS_H

#include <common.h>
#include <types.h>

#include "pybde_libbde.h"
#include "pybde_python.h"
#include "pybde_volume.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pybde_key_protectors pybde_key_protectors_t;

struct pybde_key_protectors
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The volume object
	 */
	pybde_volume_t *volume_object;

	/* The get key protector by index callback function
	 */
	PyObject* (*get_key_protector_by_index)(
	             pybde_volume_t *volume_object,
	             int key_protector_index );

	/* The (current) key protector index
	 */
	int key_protector_index;

	/* The number of key protectors
	 */
	int number_of_key_protectors;
};

extern PyTypeObject pybde_key_protectors_type_object;

PyObject *pybde_key_protectors_new(
           pybde_volume_t *volume_object,
           PyObject* (*get_key_protector_by_index)(
                        pybde_volume_t *volume_object,
                        int key_protector_index ),
           int number_of_key_protectors );

int pybde_key_protectors_init(
     pybde_key_protectors_t *pybde_key_protectors );

void pybde_key_protectors_free(
      pybde_key_protectors_t *pybde_key_protectors );

Py_ssize_t pybde_key_protectors_len(
            pybde_key_protectors_t *pybde_key_protectors );

PyObject *pybde_key_protectors_getitem(
           pybde_key_protectors_t *pybde_key_protectors,
           Py_ssize_t item_index );

PyObject *pybde_key_protectors_iter(
           pybde_key_protectors_t *pybde_key_protectors );

PyObject *pybde_key_protectors_iternext(
           pybde_key_protectors_t *pybde_key_protectors );

#if defined( __cplusplus )
}
#endif

#endif

