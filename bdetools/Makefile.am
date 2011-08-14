AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBBDE_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	bdeinfo

bdeinfo_SOURCES = \
	bdeinfo.c \
	bdeoutput.c bdeoutput.h \
	bdetools_libfdatetime.h \
	bdetools_libbde.h \
	info_handle.c info_handle.h

bdeinfo_LDADD  = \
	../liberror/liberror.la \
	../libbde/libbde.la \
	../libsystem/libsystem.la \
	@MEMWATCH_LIBADD@

bdemount_SOURCES = \
	bdemount.c \
	bdeoutput.c bdeoutput.h \
	bdetools_libbfio.h \
	bdetools_libbde.h \
	mount_handle.c mount_handle.h

bdemount_LDADD  = \
	../liberror/liberror.la \
	../libbde/libbde.la \
	../libsystem/libsystem.la \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on bdeinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(bdeinfo_SOURCES) 

