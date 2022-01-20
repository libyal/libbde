/*
 * Python object wrapper of libbde_key_protector_t
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

#if !defined( _PYBDE_KEY_PROTECTOR_H )
#define _PYBDE_KEY_PROTECTOR_H

#include <common.h>
#include <types.h>

#include "pybde_libbde.h"
#include "pybde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pybde_key_protector pybde_key_protector_t;

struct pybde_key_protector
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libbde key protector
	 */
	libbde_key_protector_t *key_protector;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pybde_key_protector_object_methods[];
extern PyTypeObject pybde_key_protector_type_object;

PyObject *pybde_key_protector_new(
           libbde_key_protector_t *key_protector,
           PyObject *parent_object );

int pybde_key_protector_init(
     pybde_key_protector_t *pybde_key_protector );

void pybde_key_protector_free(
      pybde_key_protector_t *pybde_key_protector );

PyObject *pybde_key_protector_get_identifier(
           pybde_key_protector_t *pybde_key_protector,
           PyObject *arguments );

PyObject *pybde_key_protector_get_type(
           pybde_key_protector_t *pybde_key_protector,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYBDE_KEY_PROTECTOR_H ) */

