# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
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

am_pybde_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pybde.la

nodist_pybde_la_SOURCES = $(BUILT_SOURCES)

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

pybde_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pybde_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pybde/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pybde*.[ch]

distclean: clean
	/bin/rm -f Makefile

