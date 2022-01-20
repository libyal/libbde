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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pybde_error.h"
#include "pybde_guid.h"
#include "pybde_key_protector.h"
#include "pybde_libbde.h"
#include "pybde_libcerror.h"
#include "pybde_python.h"
#include "pybde_unused.h"

PyMethodDef pybde_key_protector_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pybde_key_protector_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_type",
	  (PyCFunction) pybde_key_protector_get_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the type." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pybde_key_protector_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pybde_key_protector_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "type",
	  (getter) pybde_key_protector_get_type,
	  (setter) 0,
	  "The type.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pybde_key_protector_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pybde.key_protector",
	/* tp_basicsize */
	sizeof( pybde_key_protector_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pybde_key_protector_free,
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
	0,
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
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pybde key protector object (wraps libbde_key_protector_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pybde_key_protector_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pybde_key_protector_object_get_set_definitions,
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
	(initproc) pybde_key_protector_init,
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

/* Creates a new key protector object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_key_protector_new(
           libbde_key_protector_t *key_protector,
           PyObject *parent_object )
{
	pybde_key_protector_t *pybde_key_protector = NULL;
	static char *function                      = "pybde_key_protector_new";

	if( key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protector.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pybde_key_protector = PyObject_New(
	                       struct pybde_key_protector,
	                       &pybde_key_protector_type_object );

	if( pybde_key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize key protector.",
		 function );

		goto on_error;
	}
	pybde_key_protector->key_protector = key_protector;
	pybde_key_protector->parent_object = parent_object;

	if( pybde_key_protector->parent_object != NULL )
	{
		Py_IncRef(
		 pybde_key_protector->parent_object );
	}
	return( (PyObject *) pybde_key_protector );

on_error:
	if( pybde_key_protector != NULL )
	{
		Py_DecRef(
		 (PyObject *) pybde_key_protector );
	}
	return( NULL );
}

/* Initializes a key protector object
 * Returns 0 if successful or -1 on error
 */
int pybde_key_protector_init(
     pybde_key_protector_t *pybde_key_protector )
{
	static char *function = "pybde_key_protector_init";

	if( pybde_key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protector.",
		 function );

		return( -1 );
	}
	/* Make sure libbde key protector is set to NULL
	 */
	pybde_key_protector->key_protector = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of key protector not supported.",
	 function );

	return( -1 );
}

/* Frees a key protector object
 */
void pybde_key_protector_free(
      pybde_key_protector_t *pybde_key_protector )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_key_protector_free";
	int result                  = 0;

	if( pybde_key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protector.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pybde_key_protector );

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
	if( pybde_key_protector->key_protector != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbde_key_protector_free(
		          &( pybde_key_protector->key_protector ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbde key protector.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pybde_key_protector->parent_object != NULL )
	{
		Py_DecRef(
		 pybde_key_protector->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pybde_key_protector );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_key_protector_get_identifier(
           pybde_key_protector_t *pybde_key_protector,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_key_protector_get_identifier";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protector.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_key_protector_get_identifier(
	          pybde_key_protector->key_protector,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	string_object = pybde_string_new_from_guid(
	                 guid_data,
	                 16 );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert GUID into Unicode object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Retrieves the type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_key_protector_get_type(
           pybde_key_protector_t *pybde_key_protector,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_key_protector_get_type";
	uint16_t type            = 0;
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_key_protector == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key protector.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_key_protector_get_type(
	          pybde_key_protector->key_protector,
	          &type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) type );
#else
	integer_object = PyInt_FromLong(
	                  (long) type );
#endif
	return( integer_object );
}

