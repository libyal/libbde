/*
 * Python object definition of the sequence and iterator object of key protectors
 *
 * Copyright (C) 2011-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include "pybde_libbde.h"
#include "pybde_libcerror.h"
#include "pybde_python.h"

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
	"pybde internal sequence and iterator object of key protectors",
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
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pybde_key_protectors_t *key_protectors_object = NULL;
	static char *function                         = "pybde_key_protectors_new";

	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	if( get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get item by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the key protectors values are initialized
	 */
	key_protectors_object = PyObject_New(
	                         struct pybde_key_protectors,
	                         &pybde_key_protectors_type_object );

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create key protectors object.",
		 function );

		goto on_error;
	}
	if( pybde_key_protectors_init(
	     key_protectors_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize key protectors object.",
		 function );

		goto on_error;
	}
	key_protectors_object->parent_object     = parent_object;
	key_protectors_object->get_item_by_index = get_item_by_index;
	key_protectors_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) key_protectors_object->parent_object );

	return( (PyObject *) key_protectors_object );

on_error:
	if( key_protectors_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) key_protectors_object );
	}
	return( NULL );
}

/* Intializes a key protectors object
 * Returns 0 if successful or -1 on error
 */
int pybde_key_protectors_init(
     pybde_key_protectors_t *key_protectors_object )
{
	static char *function = "pybde_key_protectors_init";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return( -1 );
	}
	/* Make sure the key protectors values are initialized
	 */
	key_protectors_object->parent_object     = NULL;
	key_protectors_object->get_item_by_index = NULL;
	key_protectors_object->current_index     = 0;
	key_protectors_object->number_of_items   = 0;

	return( 0 );
}

/* Frees a key protectors object
 */
void pybde_key_protectors_free(
      pybde_key_protectors_t *key_protectors_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pybde_key_protectors_free";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           key_protectors_object );

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
	if( key_protectors_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) key_protectors_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) key_protectors_object );
}

/* The key protectors len() function
 */
Py_ssize_t pybde_key_protectors_len(
            pybde_key_protectors_t *key_protectors_object )
{
	static char *function = "pybde_key_protectors_len";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) key_protectors_object->number_of_items );
}

/* The key protectors getitem() function
 */
PyObject *pybde_key_protectors_getitem(
           pybde_key_protectors_t *key_protectors_object,
           Py_ssize_t item_index )
{
	PyObject *key_protector_object = NULL;
	static char *function          = "pybde_key_protectors_getitem";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) key_protectors_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	key_protector_object = key_protectors_object->get_item_by_index(
	                        key_protectors_object->parent_object,
	                        (int) item_index );

	return( key_protector_object );
}

/* The key protectors iter() function
 */
PyObject *pybde_key_protectors_iter(
           pybde_key_protectors_t *key_protectors_object )
{
	static char *function = "pybde_key_protectors_iter";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) key_protectors_object );

	return( (PyObject *) key_protectors_object );
}

/* The key protectors iternext() function
 */
PyObject *pybde_key_protectors_iternext(
           pybde_key_protectors_t *key_protectors_object )
{
	PyObject *key_protector_object = NULL;
	static char *function          = "pybde_key_protectors_iternext";

	if( key_protectors_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object - invalid current index.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protectors object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( key_protectors_object->current_index >= key_protectors_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	key_protector_object = key_protectors_object->get_item_by_index(
	                        key_protectors_object->parent_object,
	                        key_protectors_object->current_index );

	if( key_protector_object != NULL )
	{
		key_protectors_object->current_index++;
	}
	return( key_protector_object );
}

