/*
 * Python object definition of the libbde key protection types
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

#if !defined( _PYBDE_KEY_PROTECTION_TYPES_H )
#define _PYBDE_KEY_PROTECTION_TYPES_H

#include <common.h>
#include <types.h>

#include "pybde_libbde.h"
#include "pybde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pybde_key_protection_types pybde_key_protection_types_t;

struct pybde_key_protection_types
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pybde_key_protection_types_type_object;

int pybde_key_protection_types_init_type(
     PyTypeObject *type_object );

PyObject *pybde_key_protection_types_new(
           void );

int pybde_key_protection_types_init(
     pybde_key_protection_types_t *definitions_object );

void pybde_key_protection_types_free(
      pybde_key_protection_types_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYBDE_KEY_PROTECTION_TYPES_H ) */

