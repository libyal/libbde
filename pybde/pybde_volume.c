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

#include <common.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pybde_datetime.h"
#include "pybde_error.h"
#include "pybde_file_object_io_handle.h"
#include "pybde_guid.h"
#include "pybde_integer.h"
#include "pybde_key_protector.h"
#include "pybde_key_protectors.h"
#include "pybde_libbde.h"
#include "pybde_libbfio.h"
#include "pybde_libcerror.h"
#include "pybde_python.h"
#include "pybde_unused.h"
#include "pybde_volume.h"

#if !defined( LIBBDE_HAVE_BFIO )

LIBBDE_EXTERN \
int libbde_volume_open_file_io_handle(
     libbde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libbde_error_t **error );

#endif /* !defined( LIBBDE_HAVE_BFIO ) */

PyMethodDef pybde_volume_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pybde_volume_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the volume to abort the current activity." },

	{ "open",
	  (PyCFunction) pybde_volume_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pybde_volume_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	{ "close",
	  (PyCFunction) pybde_volume_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a volume." },

	{ "is_locked",
	  (PyCFunction) pybde_volume_is_locked,
	  METH_NOARGS,
	  "is_locked() -> Boolean or None\n"
	  "\n"
	  "Determines if the volume is locked." },

	{ "unlock",
	  (PyCFunction) pybde_volume_unlock,
	  METH_NOARGS,
	  "unlock() -> Boolean\n"
	  "\n"
	  "Unlocks the volume." },

	{ "read_buffer",
	  (PyCFunction) pybde_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of unencrypted data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pybde_volume_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of unencrypted data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pybde_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the unencrypted data." },

	{ "get_offset",
	  (PyCFunction) pybde_volume_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the unencrypted data." },

	{ "read",
	  (PyCFunction) pybde_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of unencrypted data." },

	{ "seek",
	  (PyCFunction) pybde_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the unencrypted data." },

	{ "tell",
	  (PyCFunction) pybde_volume_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the unencrypted data." },

	{ "get_size",
	  (PyCFunction) pybde_volume_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the unencrypted data." },

	{ "get_encryption_method",
	  (PyCFunction) pybde_volume_get_encryption_method,
	  METH_NOARGS,
	  "get_encryption_method() -> Integer\n"
	  "\n"
	  "Retrieves the encryption method." },

	{ "get_volume_identifier",
	  (PyCFunction) pybde_volume_get_volume_identifier,
	  METH_NOARGS,
	  "get_volume_identifier() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the volume identifier." },

	{ "get_creation_time",
	  (PyCFunction) pybde_volume_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime\n"
	  "\n"
	  "Retrieves the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pybde_volume_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the creation date and time as a 64-bit integer containing a FILETIME value." },

	{ "get_description",
	  (PyCFunction) pybde_volume_get_description,
	  METH_NOARGS,
	  "get_description() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the description." },

	{ "get_number_of_key_protectors",
	  (PyCFunction) pybde_volume_get_number_of_key_protectors,
	  METH_NOARGS,
	  "get_number_of_key_protectors() -> Integer\n"
	  "\n"
	  "Retrieves the number of volume master key protectors." },

	{ "get_key_protector",
	  (PyCFunction) pybde_volume_get_key_protector,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_key_protector(key_protector_index) -> Object\n"
	  "\n"
	  "Retrieves the volume master key protector specified by the index." },

	{ "set_keys",
	  (PyCFunction) pybde_volume_set_keys,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_keys(full_volume_encryption_key, tweak_key) -> None\n"
	  "\n"
	  "Sets the keys." },

	{ "set_password",
	  (PyCFunction) pybde_volume_set_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_password(password) -> None\n"
	  "\n"
	  "Sets the password." },

	{ "set_recovery_password",
	  (PyCFunction) pybde_volume_set_recovery_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_recovery_password(recovery_password) -> None\n"
	  "\n"
	  "Sets the recovery password." },

	{ "read_startup_key",
	  (PyCFunction) pybde_volume_read_startup_key,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_startup_key(filename) -> None\n"
	  "\n"
	  "Reads the startup key from a file." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pybde_volume_object_get_set_definitions[] = {

	{ "locked",
	  (getter) pybde_volume_is_locked,
	  (setter) 0,
	  "Value to indicate the volume is locked.",
	  NULL },

	{ "size",
	  (getter) pybde_volume_get_size,
	  (setter) 0,
	  "The size.",
	  NULL },

	{ "encryption_method",
	  (getter) pybde_volume_get_encryption_method,
	  (setter) 0,
	  "The encryption method.",
	  NULL },

	{ "identifier",
	  (getter) pybde_volume_get_volume_identifier,
	  (setter) 0,
	  "The volume identifier.",
	  NULL },

	{ "creation_time",
	  (getter) pybde_volume_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "description",
	  (getter) pybde_volume_get_description,
	  (setter) 0,
	  "The description.",
	  NULL },

	{ "number_of_key_protectors",
	  (getter) pybde_volume_get_number_of_key_protectors,
	  (setter) 0,
	  "The number of volume master key protectors.",
	  NULL },

	{ "key_protectors",
	  (getter) pybde_volume_get_key_protectors,
	  (setter) 0,
	  "The volume master key protectors.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pybde_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pybde.volume",
	/* tp_basicsize */
	sizeof( pybde_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pybde_volume_free,
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
	"pybde volume object (wraps libbde_volume_t)",
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
	pybde_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pybde_volume_object_get_set_definitions,
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
	(initproc) pybde_volume_init,
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

/* Initializes a volume object
 * Returns 0 if successful or -1 on error
 */
int pybde_volume_init(
     pybde_volume_t *pybde_volume )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_init";

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	/* Make sure libbde volume is set to NULL
	 */
	pybde_volume->volume         = NULL;
	pybde_volume->file_io_handle = NULL;

	if( libbde_volume_initialize(
	     &( pybde_volume->volume ),
	     &error ) != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a volume object
 */
void pybde_volume_free(
      pybde_volume_t *pybde_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_volume_free";
	int result                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pybde_volume );

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
	if( pybde_volume->file_io_handle != NULL )
	{
		if( pybde_volume_close(
		     pybde_volume,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pybde_volume->volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_free(
		          &( pybde_volume->volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbde volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pybde_volume );
}

/* Signals the volume to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_signal_abort(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_signal_abort";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_signal_abort(
	          pybde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_open(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	const char *filename_narrow  = NULL;
	static char *function        = "pybde_volume_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_open_wide(
		          pybde_volume->volume,
		          filename_wide,
		          LIBBDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pybde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_open(
		          pybde_volume->volume,
		          filename_narrow,
		          LIBBDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_open(
		          pybde_volume->volume,
		          filename_narrow,
		          LIBBDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a volume using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_open_file_object(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_volume_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pybde_volume->file_io_handle != NULL )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pybde_file_object_initialize(
	     &( pybde_volume->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_open_file_io_handle(
	          pybde_volume->volume,
	          pybde_volume->file_io_handle,
	          LIBBDE_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open volume.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pybde_volume->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pybde_volume->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_close(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_close";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_close(
	          pybde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pybde_volume->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pybde_volume->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Determines if the volume is locked
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_is_locked(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_is_locked";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_is_locked(
	          pybde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if volume is locked.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Unlocks the volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_unlock(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_unlock";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_unlock(
	          pybde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to unlock volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_read_buffer(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pybde_volume_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pybde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pybde_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pybde_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_get_size(
		          pybde_volume->volume,
		          (size64_t *) &read_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libbde_volume_read_buffer(
	              pybde_volume->volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_read_buffer_at_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pybde_volume_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pybde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pybde_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libbde_volume_read_buffer_at_offset(
	              pybde_volume->volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_seek_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pybde_volume_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libbde_volume_seek_offset(
	          pybde_volume->volume,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

 	if( offset == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset in the (volume) data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_offset(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_offset(
	          pybde_volume->volume,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pybde_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_size(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_size(
	          pybde_volume->volume,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pybde_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the encryption method
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_encryption_method(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object   = NULL;
	libcerror_error_t *error   = NULL;
	static char *function      = "pybde_volume_get_encryption_method";
	uint16_t encryption_method = 0;
	int result                 = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_encryption_method(
	          pybde_volume->volume,
	          &encryption_method,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve encryption method.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) encryption_method );
#else
	integer_object = PyInt_FromLong(
	                  (long) encryption_method );
#endif
	return( integer_object );
}

/* Retrieves the volume identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_volume_identifier(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	uint8_t guid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_get_volume_identifier";
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_volume_identifier(
	          pybde_volume->volume,
	          guid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
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

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_creation_time(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pybde_volume_get_creation_time";
	uint64_t filetime         = 0;
	int result                = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_creation_time(
	          pybde_volume->volume,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pybde_datetime_new_from_filetime(
	                   filetime );

	return( datetime_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_creation_time_as_integer(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pybde_volume_get_creation_time_as_integer";
	uint64_t filetime        = 0;
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_creation_time(
	          pybde_volume->volume,
	          &filetime,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pybde_integer_unsigned_new_from_64bit(
	                  (uint64_t) filetime );

	return( integer_object );
}

/* Retrieves the description
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_description(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *description     = NULL;
	static char *function    = "pybde_volume_get_description";
	size_t description_size  = 0;
	int result               = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_utf8_description_size(
	          pybde_volume->volume,
	          &description_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve description size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( description_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	description = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * description_size );

	if( description == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create description.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_utf8_description(
		  pybde_volume->volume,
		  description,
		  description_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve description.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) description,
			 (Py_ssize_t) description_size - 1,
			 errors );

	PyMem_Free(
	 description );

	return( string_object );

on_error:
	if( description != NULL )
	{
		PyMem_Free(
		 description );
	}
	return( NULL );
}

/* Retrieves the number of volume master key protectors
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_number_of_key_protectors(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object     = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pybde_volume_get_number_of_key_protectors";
	int number_of_key_protectors = 0;
	int result                   = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_number_of_key_protectors(
	          pybde_volume->volume,
	          &number_of_key_protectors,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of volume master key protectors.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_key_protectors );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_key_protectors );
#endif
	return( integer_object );
}

/* Retrieves a specific volume master key protector by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_key_protector_by_index(
           PyObject *pybde_volume,
           int key_protector_index )
{
	PyObject *key_protector_object        = NULL;
	libbde_key_protector_t *key_protector = NULL;
	libcerror_error_t *error              = NULL;
	static char *function                 = "pybde_volume_get_key_protector_by_index";
	int result                            = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_key_protector_by_index(
	          ( (pybde_volume_t *) pybde_volume )->volume,
	          key_protector_index,
	          &key_protector,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume master key protector: %d.",
		 function,
		 key_protector_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	key_protector_object = pybde_key_protector_new(
	                        key_protector,
	                        pybde_volume );

	if( key_protector_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create key protector object.",
		 function );

		goto on_error;
	}
	return( key_protector_object );

on_error:
	if( key_protector != NULL )
	{
		libbde_key_protector_free(
		 &key_protector,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific volume master key protector
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_key_protector(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *key_protector_object = NULL;
	static char *keyword_list[]    = { "key_protector_index", NULL };
	int key_protector_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &key_protector_index ) == 0 )
	{
		return( NULL );
	}
	key_protector_object = pybde_volume_get_key_protector_by_index(
	                        (PyObject *) pybde_volume,
	                        key_protector_index );

	return( key_protector_object );
}

/* Retrieves a sequence and iterator object for the key protectors
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_get_key_protectors(
           pybde_volume_t *pybde_volume,
           PyObject *arguments PYBDE_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object    = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pybde_volume_get_key_protectors";
	int number_of_key_protectors = 0;
	int result                   = 0;

	PYBDE_UNREFERENCED_PARAMETER( arguments )

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_get_number_of_key_protectors(
	          pybde_volume->volume,
	          &number_of_key_protectors,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of key protectors.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pybde_key_protectors_new(
	                   (PyObject *) pybde_volume,
	                   &pybde_volume_get_key_protector_by_index,
	                   number_of_key_protectors );

	if( sequence_object == NULL )
	{
		pybde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Sets the keys
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_set_keys(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error                        = NULL;
	char *full_volume_encryption_key_string         = NULL;
	char *tweak_key_string                          = NULL;
	static char *keyword_list[]                     = { "full_volume_encryption_key", "tweak_key", NULL };
	static char *function                           = "pybde_volume_set_keys";
	size_t full_volume_encryption_key_string_length = 0;
	size_t tweak_key_string_length                  = 0;
	int result                                      = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|s",
	     keyword_list,
	     &full_volume_encryption_key_string,
	     &tweak_key_string ) == 0 )
	{
		return( NULL );
	}
	if( full_volume_encryption_key_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid full volume encryption key string.",
		 function );

		return( NULL );
	}
	full_volume_encryption_key_string_length = narrow_string_length(
	                                            full_volume_encryption_key_string );

	if( tweak_key_string != NULL )
	{
		tweak_key_string_length = narrow_string_length(
		                           tweak_key_string );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_set_keys(
	          pybde_volume->volume,
	          (uint8_t *) full_volume_encryption_key_string,
	          full_volume_encryption_key_string_length,
	          (uint8_t *) tweak_key_string,
	          tweak_key_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set keys.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_set_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object             = NULL;
	libcerror_error_t *error            = NULL;
	static char *function               = "pybde_volume_set_password";
	static char *keyword_list[]         = { "password", NULL };
	const wchar_t *password_string_wide = NULL;
	const char *password_string_narrow  = NULL;
	size_t password_string_length       = 0;
	int result                          = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		password_string_wide = (wchar_t *) PyUnicode_AsUnicode(
		                                    string_object );

		password_string_length = wide_string_length(
		                          password_string_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_set_utf16_password(
		          pybde_volume->volume,
		          (uint16_t *) password_string_wide,
		          password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to set password.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		password_string_narrow = PyBytes_AsString(
				          string_object );
#else
		password_string_narrow = PyString_AsString(
				          string_object );
#endif
		password_string_length = narrow_string_length(
		                          password_string_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_set_utf8_password(
		          pybde_volume->volume,
		          (uint8_t *) password_string_narrow,
		          password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to set password.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_set_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error      = NULL;
	char *password_string         = NULL;
	static char *keyword_list[]   = { "password", NULL };
	static char *function         = "pybde_volume_set_password";
	size_t password_string_length = 0;
	int result                    = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &password_string ) == 0 )
	{
		return( NULL );
	}
	if( password_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid password string.",
		 function );

		return( NULL );
	}
	password_string_length = narrow_string_length(
	                          password_string );

	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_set_utf8_password(
	          pybde_volume->volume,
	          (uint8_t *) password_string,
	          password_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set password.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_set_recovery_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object                      = NULL;
	libcerror_error_t *error                     = NULL;
	static char *function                        = "pybde_volume_set_recovery_password";
	static char *keyword_list[]                  = { "recovery_password", NULL };
	const wchar_t *recovery_password_string_wide = NULL;
	const char *recovery_password_string_narrow  = NULL;
	size_t recovery_password_string_length       = 0;
	int result                                   = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		recovery_password_string_wide = (wchar_t *) PyUnicode_AsUnicode(
		                                             string_object );

		recovery_password_string_length = wide_string_length(
		                                   recovery_password_string_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_set_utf16_recovery_password(
		          pybde_volume->volume,
		          (uint16_t *) recovery_password_string_wide,
		          recovery_password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to set recovery password.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		recovery_password_string_narrow = PyBytes_AsString(
		                                   string_object );
#else
		recovery_password_string_narrow = PyString_AsString(
		                                   string_object );
#endif
		recovery_password_string_length = narrow_string_length(
		                                   recovery_password_string_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_set_utf8_recovery_password(
		          pybde_volume->volume,
		          (uint8_t *) recovery_password_string_narrow,
		          recovery_password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to set recovery password.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_set_recovery_password(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error               = NULL;
	char *recovery_password_string         = NULL;
	static char *keyword_list[]            = { "recovery_password", NULL };
	static char *function                  = "pybde_volume_set_recovery_password";
	size_t recovery_password_string_length = 0;
	int result                             = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &recovery_password_string ) == 0 )
	{
		return( NULL );
	}
	if( recovery_password_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid recovery password string.",
		 function );

		return( NULL );
	}
	recovery_password_string_length = narrow_string_length(
	                                   recovery_password_string );

	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_set_utf8_recovery_password(
	          pybde_volume->volume,
	          (uint8_t *) recovery_password_string,
	          recovery_password_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set recovery password.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Reads the startup key from a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_read_startup_key(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pybde_volume_read_startup_key";
	static char *keyword_list[]  = { "filename", NULL };
	const wchar_t *filename_wide = NULL;
	const char *filename_narrow  = NULL;
	int result                   = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_read_startup_key_wide(
		          pybde_volume->volume,
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to read startup key.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pybde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libbde_volume_read_startup_key(
		          pybde_volume->volume,
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pybde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to read startup key.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Reads the startup key from a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pybde_volume_read_startup_key(
           pybde_volume_t *pybde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	char *filename              = NULL;
	static char *keyword_list[] = { "filename", NULL };
	static char *function       = "pybde_volume_read_startup_key";
	int result                  = 0;

	if( pybde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	if( filename == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid filename.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libbde_volume_read_startup_key(
	          pybde_volume->volume,
	          filename,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pybde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read startup key.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */
