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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pybde_key_protector.h"
#include "pybde_key_protectors.h"
#include "pybde_libcerror.h"
#include "pybde_libbde.h"
#include "pybde_python.h"
#include "pybde_volume.h"

PySequenceMethods pybde_key_protectors_sequence_methods = {
	/* sq_length */
	(lenfunc) pybde_key_protectors_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pybde_key_protectors_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pybde_key_protectors_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pybde._key_protectors",
	/* tp_basicsize */
	sizeof( pybde_key_protectors_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pybde_key_protectors_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pybde_key_protectors_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pybde key protectors sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pybde_key_protectors_iter,
	/* tp_iternext */
	(iternextfunc) pybde_key_protectors_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pybde_key_protectors_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new key protectors object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_key_protectors_new(
           pybde_volume_t *volume_object,
           PyObject* (*get_key_protector_by_index)(
                        pybde_volume_t *volume_object,
                        int key_protector_index ),
           int number_of_key_protectors )
{
	pybde_key_protectors_t *pybde_key_protectors = NULL;
	static char *function                        = "pybde_key_protectors_new";

	if( volume_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume object.",
		 function );

		return( NULL );
	}
	if( get_key_protector_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get key protector by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the key protectors values are initialized
	 */
	pybde_key_protectors = PyObject_New(
	                        struct pybde_key_protectors,
	                        &pybde_key_protectors_type_object );

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize key protectors.",
		 function );

		goto on_error;
	}
	if( pybde_key_protectors_init(
	     pybde_key_protectors ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize key protectors.",
		 function );

		goto on_error;
	}
	pybde_key_protectors->volume_object              = volume_object;
	pybde_key_protectors->get_key_protector_by_index = get_key_protector_by_index;
	pybde_key_protectors->number_of_key_protectors   = number_of_key_protectors;

	Py_IncRef(
	 (PyObject *) pybde_key_protectors->volume_object );

	return( (PyObject *) pybde_key_protectors );

on_error:
	if( pybde_key_protectors != NULL )
	{
		Py_DecRef(
		 (PyObject *) pybde_key_protectors );
	}
	return( NULL );
}

/* Intializes a key protectors object
 * Returns 0 if successful or -1 on error
 */
int pybde_key_protectors_init(
     pybde_key_protectors_t *pybde_key_protectors )
{
	static char *function = "pybde_key_protectors_init";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return( -1 );
	}
	/* Make sure the key protectors values are initialized
	 */
	pybde_key_protectors->volume_object              = NULL;
	pybde_key_protectors->get_key_protector_by_index = NULL;
	pybde_key_protectors->key_protector_index        = 0;
	pybde_key_protectors->number_of_key_protectors   = 0;

	return( 0 );
}

/* Frees a key protectors object
 */
void pybde_key_protectors_free(
      pybde_key_protectors_t *pybde_key_protectors )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pybde_key_protectors_free";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return;
	}
	if( pybde_key_protectors->volume_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pybde_key_protectors->volume_object );
	}
	ob_type = Py_TYPE(
	           pybde_key_protectors );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	ob_type->tp_free(
	 (PyObject*) pybde_key_protectors );
}

/* The key protectors len() function
 */
Py_ssize_t pybde_key_protectors_len(
            pybde_key_protectors_t *pybde_key_protectors )
{
	static char *function = "pybde_key_protectors_len";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pybde_key_protectors->number_of_key_protectors );
}

/* The key protectors getitem() function
 */
PyObject *pybde_key_protectors_getitem(
           pybde_key_protectors_t *pybde_key_protectors,
           Py_ssize_t item_index )
{
	PyObject *key_protector_object = NULL;
	static char *function          = "pybde_key_protectors_getitem";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->get_key_protector_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors - missing get key protector by index function.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->number_of_key_protectors < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors - invalid number of key protectors.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pybde_key_protectors->number_of_key_protectors ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	key_protector_object = pybde_key_protectors->get_key_protector_by_index(
	                        pybde_key_protectors->volume_object,
	                        (int) item_index );

	return( key_protector_object );
}

/* The key protectors iter() function
 */
PyObject *pybde_key_protectors_iter(
           pybde_key_protectors_t *pybde_key_protectors )
{
	static char *function = "pybde_key_protectors_iter";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pybde_key_protectors );

	return( (PyObject *) pybde_key_protectors );
}

/* The key protectors iternext() function
 */
PyObject *pybde_key_protectors_iternext(
           pybde_key_protectors_t *pybde_key_protectors )
{
	PyObject *key_protector_object = NULL;
	static char *function          = "pybde_key_protectors_iternext";

	if( pybde_key_protectors == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->get_key_protector_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors - missing get key protector by index function.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->key_protector_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors - invalid key protector index.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->number_of_key_protectors < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors - invalid number of key protectors.",
		 function );

		return( NULL );
	}
	if( pybde_key_protectors->key_protector_index >= pybde_key_protectors->number_of_key_protectors )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	key_protector_object = pybde_key_protectors->get_key_protector_by_index(
	                        pybde_key_protectors->volume_object,
	                        pybde_key_protectors->key_protector_index );

	if( key_protector_object != NULL )
	{
		pybde_key_protectors->key_protector_index++;
	}
	return( key_protector_object );
}

