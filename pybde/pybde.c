/*
 * Python bindings module for libbde (pybde)
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "pybde.h"
#include "pybde_libcerror.h"
#include "pybde_libcstring.h"
#include "pybde_libbde.h"
#include "pybde_python.h"
#include "pybde_volume.h"

/* The pybde module methods
 */
PyMethodDef pybde_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pybde_get_version,
	  METH_NOARGS,
	  "Retrieves the version" },

	{ "get_access_flags_read",
	  (PyCFunction) pybde_get_access_flags_read,
	  METH_NOARGS,
	  "Retrieves the read access flags" },

	{ "check_volume_signature",
	  (PyCFunction) pybde_check_volume_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "Checks if a volume has a VSS volume signature" },

	{ "open",
	  (PyCFunction) pybde_volume_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "Creates a new volume and opens it" },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pybde/libbde version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_get_version(
           PyObject *self )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	version_string = libbde_get_version();

	version_string_length = libcstring_narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Retrieves the pybde/libbde read access flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_get_access_flags_read(
           PyObject *self )
{
	return( PyInt_FromLong(
	         (long) libbde_get_access_flags_read() ) );
}

/* Checks if the volume has a a VSS volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_check_volume_signature(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYBDE_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_check_volume_signature";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;
	int result                  = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	result = libbde_check_volume_signature(
	          filename,
	          &error );

	if( result == -1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYBDE_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to check volume signature.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_IOError,
			 "%s: unable to check volume signature.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		return( Py_True );
	}
	return( Py_False );
}

/* Declarations for DLL import/export
 */
#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif

/* Initializes the pybde module
 */
PyMODINIT_FUNC initpybde(
                void )
{
	PyObject *module                 = NULL;
	PyTypeObject *volume_type_object = NULL;
	PyGILState_STATE gil_state       = 0;

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
	module = Py_InitModule3(
	          "pybde",
	          pybde_module_methods,
	          "Python libbde module (pybde)." );

	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the volume type object
	 */
	pybde_volume_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pybde_volume_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pybde_volume_type_object );

	volume_type_object = &pybde_volume_type_object;

	PyModule_AddObject(
	 module,
	"volume",
	(PyObject *) volume_type_object );

on_error:
	PyGILState_Release(
	 gil_state );
}

