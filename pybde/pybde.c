/*
 * Python bindings module for libbde (pybde)
 *
 * Copyright (c) 2011-2013, Joachim Metz <joachim.metz@gmail.com>
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
#include "pybde_file_object_io_handle.h"
#include "pybde_python.h"
#include "pybde_unused.h"
#include "pybde_volume.h"

#if !defined( LIBBDE_HAVE_BFIO )
LIBBDE_EXTERN \
int libbde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libbde_error_t **error );
#endif

/* The pybde module methods
 */
PyMethodDef pybde_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pybde_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_volume_signature",
	  (PyCFunction) pybde_check_volume_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a BitLocker Drive Encryption (BDE) volume signature." },

	{ "check_volume_signature_file_object",
	  (PyCFunction) pybde_check_volume_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_volume_signature(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a volume has a BitLocker Drive Encryption (BDE) volume signature using a file-like object." },

	{ "open",
	  (PyCFunction) pybde_volume_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pybde_volume_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pybde/libbde version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_get_version(
           PyObject *self PYBDE_ATTRIBUTE_UNUSED,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYBDE_UNREFERENCED_PARAMETER( self )
	PYBDE_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libbde_get_version();

	Py_END_ALLOW_THREADS

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

/* Checks if the volume has a BitLocker Drive Encryption (BDE) volume signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_check_volume_signature(
           PyObject *self PYBDE_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYBDE_ERROR_STRING_SIZE ];

	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_check_volume_signature";
	static char *keyword_list[] = { "filename", NULL };
	const char *filename        = NULL;
	int result                  = 0;

	PYBDE_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_check_volume_signature(
	          filename,
	          &error );

	Py_END_ALLOW_THREADS

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

/* Checks if the volume has a BitLocker Drive Encryption (BDE) volume signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_check_volume_signature_file_object(
           PyObject *self PYBDE_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	char error_string[ PYBDE_ERROR_STRING_SIZE ];

	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pybde_check_volume_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYBDE_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pybde_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYBDE_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to initialize file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_check_volume_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

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

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		if( libcerror_error_backtrace_sprint(
		     error,
		     error_string,
		     PYBDE_ERROR_STRING_SIZE ) == -1 )
                {
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file IO handle.",
			 function );
		}
		else
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to free file IO handle.\n%s",
			 function,
			 error_string );
		}
		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		return( Py_True );
	}
	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
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

