if HAVE_LOCAL_LIBCTHREADS
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@

noinst_LTLIBRARIES = libcthreads.la

libcthreads_la_SOURCES = \
	libcthreads_condition.c libcthreads_condition.h \
	libcthreads_definitions.h \
	libcthreads_error.c libcthreads_error.h \
	libcthreads_extern.h \
	libcthreads_libcerror.h \
	libcthreads_libcstring.h \
	libcthreads_lock.c libcthreads_lock.h \
	libcthreads_mutex.c libcthreads_mutex.h \
	libcthreads_queue.c libcthreads_queue.h \
	libcthreads_read_write_lock.c libcthreads_read_write_lock.h \
	libcthreads_repeating_thread.c libcthreads_repeating_thread.h \
	libcthreads_support.c libcthreads_support.h \
	libcthreads_thread.c libcthreads_thread.h \
	libcthreads_thread_attributes.c libcthreads_thread_attributes.h \
	libcthreads_thread_pool.c libcthreads_thread_pool.h \
	libcthreads_types.h \
	libcthreads_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcthreads ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcthreads_la_SOURCES)

