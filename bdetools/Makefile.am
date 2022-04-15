AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@LIBBDE_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	bdeinfo \
	bdemount

bdeinfo_SOURCES = \
	byte_size_string.c byte_size_string.h \
	bdeinfo.c \
	bdetools_getopt.c bdetools_getopt.h \
	bdetools_i18n.h \
	bdetools_input.c bdetools_input.h \
	bdetools_libbde.h \
	bdetools_libcerror.h \
	bdetools_libclocale.h \
	bdetools_libcnotify.h \
	bdetools_libcsplit.h \
	bdetools_libfdatetime.h \
	bdetools_libfguid.h \
	bdetools_libuna.h \
	bdetools_output.c bdetools_output.h \
	bdetools_signal.c bdetools_signal.h \
	bdetools_unused.h \
	info_handle.c info_handle.h

bdeinfo_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

bdemount_SOURCES = \
	bdemount.c \
	bdetools_getopt.c bdetools_getopt.h \
	bdetools_i18n.h \
	bdetools_input.c bdetools_input.h \
	bdetools_libbfio.h \
	bdetools_libbde.h \
	bdetools_libcdata.h \
	bdetools_libcerror.h \
	bdetools_libclocale.h \
	bdetools_libcnotify.h \
	bdetools_libcpath.h \
	bdetools_libcsplit.h \
	bdetools_output.c bdetools_output.h \
	bdetools_signal.c bdetools_signal.h \
	bdetools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

bdemount_LDADD = \
	@LIBFUSE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on bdeinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bdeinfo_SOURCES)
	@echo "Running splint on bdemount ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bdemount_SOURCES)

