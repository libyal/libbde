AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBBDE_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	bdeinfo \
	bdemount

bdeinfo_SOURCES = \
	bdeinfo.c \
	bdeoutput.c bdeoutput.h \
	bdetools_libbde.h \
	bdetools_libcerror.h \
	bdetools_libclocale.h \
	bdetools_libcnotify.h \
	bdetools_libcstring.h \
	bdetools_libcsystem.h \
	bdetools_libfdatetime.h \
	info_handle.c info_handle.h

bdeinfo_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libbde/libbde.la \
	@LIBBFIO_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

bdemount_SOURCES = \
	bdemount.c \
	bdeoutput.c bdeoutput.h \
	bdetools_libbfio.h \
	bdetools_libbde.h \
	bdetools_libcerror.h \
	bdetools_libclocale.h \
	bdetools_libcnotify.h \
	bdetools_libcstring.h \
	bdetools_libcsystem.h \
	mount_handle.c mount_handle.h

bdemount_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libbde/libbde.la \
	@LIBBFIO_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@LIBFUSE_LIBADD@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on bdeinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bdeinfo_SOURCES) 
	@echo "Running splint on bdemount ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bdemount_SOURCES) 

