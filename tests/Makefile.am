AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBBDE_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYBDE = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_bdeinfo.sh \
	$(TESTS_PYBDE)

check_SCRIPTS = \
	pybde_test_support.py \
	pybde_test_volume.py \
	test_bdeinfo.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	bde_test_aes_ccm_encrypted_key \
	bde_test_encryption_context \
	bde_test_error \
	bde_test_external_key \
	bde_test_io_handle \
	bde_test_key \
	bde_test_key_protector \
	bde_test_metadata \
	bde_test_metadata_block_header \
	bde_test_metadata_entry \
	bde_test_metadata_header \
	bde_test_notify \
	bde_test_password \
	bde_test_password_keep \
	bde_test_sector_data \
	bde_test_sector_data_vector \
	bde_test_stretch_key \
	bde_test_support \
	bde_test_tools_info_handle \
	bde_test_tools_output \
	bde_test_tools_signal \
	bde_test_volume \
	bde_test_volume_header \
	bde_test_volume_master_key

bde_test_aes_ccm_encrypted_key_SOURCES = \
	bde_test_aes_ccm_encrypted_key.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_aes_ccm_encrypted_key_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_encryption_context_SOURCES = \
	bde_test_encryption_context.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_encryption_context_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_error_SOURCES = \
	bde_test_error.c \
	bde_test_libbde.h \
	bde_test_macros.h \
	bde_test_unused.h

bde_test_error_LDADD = \
	../libbde/libbde.la

bde_test_external_key_SOURCES = \
	bde_test_external_key.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_external_key_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_io_handle_SOURCES = \
	bde_test_io_handle.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_io_handle_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_key_SOURCES = \
	bde_test_key.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_key_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_key_protector_SOURCES = \
	bde_test_key_protector.c \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h

bde_test_key_protector_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_metadata_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_metadata.c \
	bde_test_unused.h

bde_test_metadata_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_metadata_block_header_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_metadata_block_header.c \
	bde_test_unused.h

bde_test_metadata_block_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_metadata_entry_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_metadata_entry.c \
	bde_test_unused.h

bde_test_metadata_entry_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_metadata_header_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_metadata_header.c \
	bde_test_unused.h

bde_test_metadata_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_notify_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcnotify.h \
	bde_test_macros.h \
	bde_test_notify.c \
	bde_test_unused.h

bde_test_notify_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_password_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_password.c \
	bde_test_unused.h

bde_test_password_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_password_keep_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_password_keep.c \
	bde_test_unused.h

bde_test_password_keep_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_sector_data_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_sector_data.c \
	bde_test_unused.h

bde_test_sector_data_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_sector_data_vector_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_sector_data_vector.c \
	bde_test_unused.h

bde_test_sector_data_vector_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_stretch_key_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_stretch_key.c \
	bde_test_unused.h

bde_test_stretch_key_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_support_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_getopt.c bde_test_getopt.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_libclocale.h \
	bde_test_libcnotify.h \
	bde_test_libuna.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_support.c \
	bde_test_unused.h

bde_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_tools_info_handle_SOURCES = \
	../bdetools/bdetools_input.c ../bdetools/bdetools_input.h \
	../bdetools/byte_size_string.c ../bdetools/byte_size_string.h \
	../bdetools/info_handle.c ../bdetools/info_handle.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_tools_info_handle.c \
	bde_test_unused.h

bde_test_tools_info_handle_LDADD = \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_tools_output_SOURCES = \
	../bdetools/bdetools_output.c ../bdetools/bdetools_output.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_tools_output.c \
	bde_test_unused.h

bde_test_tools_output_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_tools_signal_SOURCES = \
	../bdetools/bdetools_signal.c ../bdetools/bdetools_signal.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_tools_signal.c \
	bde_test_unused.h

bde_test_tools_signal_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_volume_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_getopt.c bde_test_getopt.h \
	bde_test_libbde.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_libclocale.h \
	bde_test_libcnotify.h \
	bde_test_libuna.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h \
	bde_test_volume.c

bde_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

bde_test_volume_header_SOURCES = \
	bde_test_functions.c bde_test_functions.h \
	bde_test_libbfio.h \
	bde_test_libcerror.h \
	bde_test_libbde.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h \
	bde_test_volume_header.c

bde_test_volume_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

bde_test_volume_master_key_SOURCES = \
	bde_test_libbde.h \
	bde_test_libcerror.h \
	bde_test_macros.h \
	bde_test_memory.c bde_test_memory.h \
	bde_test_unused.h \
	bde_test_volume_master_key.c

bde_test_volume_master_key_LDADD = \
	../libbde/libbde.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

