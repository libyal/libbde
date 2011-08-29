AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libbde.la

libbde_la_SOURCES = \
	libbde.c \
	libbde_aes.c libbde_aes.h \
	libbde_aes_ccm_encrypted_key.c libbde_aes_ccm_encrypted_key.h \
	libbde_array_type.c libbde_array_type.h \
	libbde_codepage.h \
	libbde_debug.c libbde_debug.h \
	libbde_definitions.h \
	libbde_diffuser.c libbde_diffuser.h \
	libbde_encryption.c libbde_encryption.h \
	libbde_error.c libbde_error.h \
	libbde_error_string.c libbde_error_string.h \
	libbde_extern.h \
	libbde_external_key.c libbde_external_key.h \
	libbde_io_handle.c libbde_io_handle.h \
	libbde_key.c libbde_key.h \
	libbde_libbfio.h \
	libbde_libfdata.h \
	libbde_libfdatetime.h \
	libbde_libfguid.h \
	libbde_libfvalue.h \
	libbde_libhmac.h \
	libbde_libuna.h \
	libbde_metadata.c libbde_metadata.h \
	libbde_metadata_entry.c libbde_metadata_entry.h \
	libbde_notify.c libbde_notify.h \
	libbde_password.c libbde_password.h \
	libbde_recovery.c libbde_recovery.h \
	libbde_sector_data.c libbde_sector_data.h \
	libbde_stretch_key.c libbde_stretch_key.h \
	libbde_support.c libbde_support.h \
	libbde_types.h \
	libbde_unused.h \
	libbde_volume.c libbde_volume.h \
	libbde_volume_master_key.c libbde_volume_master_key.h \
	bde_metadata.h \
	bde_volume.h

libbde_la_LIBADD = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@

libbde_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libbde_definitions.h.in \
	libbde.rc \
	libbde.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libbde_definitions.h
	/bin/rm -f libbde.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libbde ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libbde_la_SOURCES) 

