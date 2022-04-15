AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@LIBFVALUE_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBBDE_DLL_EXPORT@

lib_LTLIBRARIES = libbde.la

libbde_la_SOURCES = \
	libbde.c \
	libbde_aes_ccm_encrypted_key.c libbde_aes_ccm_encrypted_key.h \
	libbde_codepage.h \
	libbde_debug.c libbde_debug.h \
	libbde_definitions.h \
	libbde_diffuser.c libbde_diffuser.h \
	libbde_encryption_context.c libbde_encryption_context.h \
	libbde_error.c libbde_error.h \
	libbde_extern.h \
	libbde_external_key.c libbde_external_key.h \
	libbde_io_handle.c libbde_io_handle.h \
	libbde_key.c libbde_key.h \
	libbde_key_protector.c libbde_key_protector.h \
	libbde_libbfio.h \
	libbde_libcaes.h \
	libbde_libcdata.h \
	libbde_libcerror.h \
	libbde_libclocale.h \
	libbde_libcnotify.h \
	libbde_libcthreads.h \
	libbde_libfcache.h \
	libbde_libfdatetime.h \
	libbde_libfguid.h \
	libbde_libfvalue.h \
	libbde_libhmac.h \
	libbde_libuna.h \
	libbde_metadata.c libbde_metadata.h \
	libbde_metadata_block_header.c libbde_metadata_block_header.h \
	libbde_metadata_entry.c libbde_metadata_entry.h \
	libbde_metadata_header.c libbde_metadata_header.h \
	libbde_notify.c libbde_notify.h \
	libbde_ntfs_volume_header.c libbde_ntfs_volume_header.h \
	libbde_password.c libbde_password.h \
	libbde_password_keep.c libbde_password_keep.h \
	libbde_recovery.c libbde_recovery.h \
	libbde_sector_data.c libbde_sector_data.h \
	libbde_sector_data_vector.c libbde_sector_data_vector.h \
	libbde_stretch_key.c libbde_stretch_key.h \
	libbde_support.c libbde_support.h \
	libbde_types.h \
	libbde_unused.h \
	libbde_volume.c libbde_volume.h \
	libbde_volume_header.c libbde_volume_header.h \
	libbde_volume_master_key.c libbde_volume_master_key.h \
	bde_metadata.h \
	bde_volume.h

libbde_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCAES_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@PTHREAD_LIBADD@

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

