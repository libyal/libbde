if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBBDE_DLL_IMPORT@

pyexec_LTLIBRARIES = pybde.la

pybde_la_SOURCES = \
	pybde.c pybde.h \
	pybde_datetime.c pybde_datetime.h \
	pybde_encryption_methods.c pybde_encryption_methods.h \
	pybde_error.c pybde_error.h \
	pybde_file_object_io_handle.c pybde_file_object_io_handle.h \
	pybde_guid.c pybde_guid.h \
	pybde_integer.c pybde_integer.h \
	pybde_key_protection_types.c pybde_key_protection_types.h \
	pybde_key_protector.c pybde_key_protector.h \
	pybde_key_protectors.c pybde_key_protectors.h \
	pybde_libbde.h \
	pybde_libbfio.h \
	pybde_libcerror.h \
	pybde_libclocale.h \
	pybde_libfguid.h \
	pybde_python.h \
	pybde_unused.h \
	pybde_volume.c pybde_volume.h

pybde_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libbde/libbde.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pybde_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pybde_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

