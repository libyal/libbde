AC_PREREQ( 2.59 )

AC_INIT(
 [libbde],
 [20110627],
 [jbmetz@users.sourceforge.net])

AC_CONFIG_SRCDIR(
 [include/libbde.h.in])

AM_INIT_AUTOMAKE([gnu 1.6])

AC_CONFIG_MACRO_DIR([m4])

dnl Check for host type
AC_CANONICAL_HOST

dnl Check for large file support
AC_SYS_LARGEFILE

dnl Check for configure arguments
LIBBDE_TEST_ENABLE(
 [wide-character-type],
 [wide_character_type],
 [enable wide character type support],
 [no])
LIBBDE_TEST_ENABLE(
 [winapi],
 [winapi],
 [enable WINAPI support for cross-compilation],
 [auto-detect])
LIBBDE_TEST_ENABLE(
 [static-executables],
 [static_executables],
 [build the bdetools as static executables],
 [no])
LIBBDE_TEST_ENABLE(
 [verbose-output],
 [verbose_output],
 [enable verbose output],
 [no])
LIBBDE_TEST_ENABLE(
 [debug-output],
 [debug_output],
 [enable debug output],
 [no])

dnl Check for libtool DLL support
AC_LIBTOOL_WIN32_DLL

dnl Checks for programs
AC_PROG_CC
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Support of internationalization (i18n)
AM_GNU_GETTEXT([external])

dnl Check for compiler language support
AC_C_CONST
AC_C_VOLATILE

dnl Automatically enable winapi support for mingw
AS_IF(
 [test "x$ac_cv_libbde_enable_winapi" = xauto-detect],
 [ac_libbde_target_string="$target";

 AS_IF(
  [test "x$ac_libbde_target_string" = x],
  [ac_libbde_target_string="$build"])

 AS_CASE(
  [$ac_libbde_target_string],
  [*mingw*],[AC_MSG_NOTICE(
             [Detected MinGW enabling WINAPI support for cross-compilation])
            ac_cv_libbde_enable_winapi=yes],
  [*],[ac_cv_libbde_enable_winapi=no])
 ])

dnl Check for headers
dnl Headers included in libbde/types.h
AC_CHECK_HEADERS([sys/types.h inttypes.h stdint.h])

AS_IF(
 [test "x$ac_cv_header_sys_types_h" = xyes],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_SYS_TYPES_H],
  [0])])

AS_IF(
 [test "x$ac_cv_header_inttypes_h" = xyes],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [1])],
 [AC_SUBST(
  [HAVE_INTTYPES_H],
  [0])])

AS_IF(
 [test "x$ac_cv_header_stdint_h" = xyes],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [1])],
 [AC_SUBST(
  [HAVE_STDINT_H],
  [0])])

dnl Headers included in common/common.h
AS_IF(
 [test "x$ac_cv_libbde_enable_winapi" = xyes],
 [AC_CHECK_HEADERS([windows.h])

 AS_IF(
  [test "x$ac_cv_header_windows_h" = xno],
  [AC_MSG_FAILURE(
   [Missing header: windows.h header is required to compile with winapi support],
   [1]) ])
 ])

dnl Headers included in common/types.h
AC_CHECK_HEADERS([limits.h])

dnl Headers included in common/memory.h and libcstring/libcstring_narrow_string.h
AC_CHECK_HEADERS([stdlib.h string.h])

dnl Headers included in liberror/liberror_error.c and libnotify/libnotify_print.c
AC_CHECK_HEADERS([stdarg.h])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes],
 [AC_CHECK_HEADERS([varargs.h]) ])

AS_IF(
 [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
 [AC_MSG_FAILURE(
  [Missing headers: stdarg.h and varargs.h],
  [1]) ])

dnl Headers included in libnotify/libnotify_stream.c and libbfio/libbfio_file.c
AC_CHECK_HEADERS([errno.h])

dnl Headers included in libbfio/libbfio_file.c, libsystem/libsystem_directory_io.h and libsystem/libsystem_file_io.h
AC_CHECK_HEADERS([sys/stat.h])

dnl Headers included in libbfio/libbfio_file.c and libsystem/libsystem_file_io.h
AC_CHECK_HEADERS([fcntl.h unistd.h])

dnl Headers included in libsystem/libsystem_glob.h
AC_CHECK_HEADERS([glob.h])

AS_IF(
 [test "x$ac_cv_header_glob_h" = xno],
 [AC_CHECK_HEADERS([io.h]) ])

dnl Headers included in libsytem/libsystem_support.c
AC_CHECK_HEADERS([langinfo.h locale.h])

dnl Headers included in libsystem/libsystem_signal.h
AC_CHECK_HEADERS([signal.h sys/signal.h])

dnl Other headers

dnl Check for declarations and type definitions
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

AC_CHECK_TYPE(
 [size32_t],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [ssize32_t],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE32_T],
  [0])])

AC_CHECK_TYPE(
 [size64_t],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [ssize64_t],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [1])],
 [AC_SUBST(
  [HAVE_SSIZE64_T],
  [0])])

AC_CHECK_TYPE(
 [off64_t],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [1])],
 [AC_SUBST(
  [HAVE_OFF64_T],
  [0])])

AC_CHECK_TYPE([ssize_t])

AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([size_t])

dnl Check for functions
dnl File stream functions used in common/file_stream.h
AC_CHECK_FUNCS(
 [fclose],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fclose],
  [1]) ])

AC_CHECK_FUNCS(
 [feof],
 [],
 [AC_MSG_FAILURE(
  [Missing function: feof],
  [1]) ])

AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [fopen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fopen],
  [1]) ])

AC_CHECK_FUNCS(
 [fread],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fread],
  [1]) ])

AC_CHECK_FUNCS(
 [fseeko],
 [],
 [AC_CHECK_FUNCS(
  [fseeko64],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fseeko and fseeko64],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [fwrite],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fwrite],
  [1]) ])

AC_CHECK_FUNCS(
 [vfprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vfprintf],
  [1]) ])

dnl Memory functions used in common/memory.h
AC_CHECK_FUNCS(
 [free],
 [],
 [AC_MSG_FAILURE(
  [Missing function: free],
  [1]) ])

AC_CHECK_FUNCS(
 [malloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: malloc],
  [1]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcmp],
  [1]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memcpy],
  [1]) ])

AC_CHECK_FUNCS(
 [memset],
 [],
 [AC_MSG_FAILURE(
  [Missing function: memset],
  [1]) ])

AC_CHECK_FUNCS(
 [realloc],
 [],
 [AC_MSG_FAILURE(
  [Missing function: realloc],
  [1]) ])

dnl Narrow character string functions used in libcstring/libcstring_narrow_string.h
AC_CHECK_FUNCS(
 [fgets],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fgets],
  [1]) ])

AC_CHECK_FUNCS(
 [memchr],
 [],
 [AC_CHECK_FUNCS(
  [strchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memchr and strchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memcmp],
 [],
 [AC_CHECK_FUNCS(
  [strncmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memcmp and strncmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memcpy],
 [],
 [AC_CHECK_FUNCS(
  [strncpy],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: memcpy and strncpy],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [memrchr],
 [AC_CHECK_DECLS(
  [memrchr],
  [ac_libbde_have_memrchr=yes],
  [ac_libbde_have_memrchr=no]) ],
 [ac_libbde_have_memrchr=no])

AS_IF(
 [test "x$ac_libbde_have_memrchr" = xno],
 [AC_CHECK_FUNCS(
  [strrchr],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strrchr and memrchr],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [snprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: snprintf],
  [1]) ])

AC_CHECK_FUNCS(
 [sscanf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: sscanf],
  [1]) ])

AC_CHECK_FUNCS(
 [strlen],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strlen],
  [1]) ])

AC_CHECK_FUNCS(
 [strncasecmp],
 [],
 [AC_CHECK_FUNCS(
  [strcasecmp],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strncasecmp and strcasecmp],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strstr],
 [],
 [AC_MSG_FAILURE(
  [Missing function: strstr],
  [1]) ])

AC_CHECK_FUNCS(
 [strtoll],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoll and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [strtoull],
 [],
 [AC_CHECK_FUNCS(
  [atoll],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strtoull and atoll],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [vsnprintf],
 [],
 [AC_MSG_FAILURE(
  [Missing function: vsnprintf],
  [1]) ])

dnl File input/output functions used in libbfio/libbfio_file.h
AC_CHECK_FUNCS(
 [close],
 [],
 [AC_MSG_FAILURE(
  [Missing function: close],
  [1]) ])

AC_CHECK_FUNCS(
 [fstat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: fstat],
  [1]) ])

AC_CHECK_FUNCS(
 [ftruncate],
 [],
 [AC_MSG_FAILURE(
  [Missing function: ftruncate],
  [1]) ])

AC_CHECK_FUNCS(
 [lseek],
 [],
 [AC_MSG_FAILURE(
  [Missing function: lseek],
  [1]) ])

AC_CHECK_FUNCS(
 [open],
 [],
 [AC_MSG_FAILURE(
  [Missing function: open],
  [1]) ])

AC_CHECK_FUNCS(
 [read],
 [],
 [AC_MSG_FAILURE(
  [Missing function: read],
  [1]) ])

AC_CHECK_FUNCS(
 [stat],
 [],
 [AC_MSG_FAILURE(
  [Missing function: stat],
  [1]) ])

AC_CHECK_FUNCS(
 [write],
 [],
 [AC_MSG_FAILURE(
  [Missing function: write],
  [1]) ])

dnl Check for error string functions used in libbfio/libbfio_error_string.h and libsystem/libsystem_error_string.c
AC_FUNC_STRERROR_R()

AS_IF(
 [test "x$ac_cv_have_decl_strerror_r" = xno],
 [AC_CHECK_FUNCS(
  [strerror],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: strerror_r and strerror],
   [1]) ]) ])

dnl Date and time functions used in libfdata/libfdata_date_time.h and libsystem/libsystem_date_time.h
AC_CHECK_FUNCS(
 [time],
 [],
 [AC_MSG_FAILURE(
  [Missing function: time],
  [1]) ])

dnl Date and time functions used in libsystem/libsystem_date_time.h
LIBBDE_CHECK_FUNC_CTIME

AC_CHECK_FUNCS(
 [gmtime_r],
 [],
 [AC_CHECK_FUNCS(
  [gmtime],
  [],
  [AC_MSG_FAILURE(
   [Missing functions: gmtime_r and gmtime],
   [1]) ]) ])

dnl Directory functions used in libsystem/libsystem_directory_io.h
AC_CHECK_FUNCS(
 [chdir],
 [],
 [AC_MSG_FAILURE(
  [Missing functions: chdir],
  [1]) ])

LIBBDE_CHECK_FUNC_MKDIR

dnl Commandline argument/option parsing functions in libsystem/libsystem_getopt.h
AC_CHECK_FUNCS([getopt])

dnl Check for locale functions in libcstring/libcstring_locale.c
AS_IF(
 [test "x$ac_cv_libbde_enable_winapi" = xno],
 [AC_CHECK_FUNCS(
  [localeconv],
  [],
  [AC_MSG_FAILURE(
   [Missing function: localeconv],
   [1]) ]) ])

AC_CHECK_FUNCS(
 [setlocale],
 [],
 [AC_MSG_FAILURE(
  [Missing function: setlocale],
  [1]) ])

LIBBDE_CHECK_FUNC_LANGINFO_CODESET

dnl Other functions

dnl Check for printf conversion specifier support
LIBBDE_CHECK_FUNC_PRINTF_JD
LIBBDE_CHECK_FUNC_PRINTF_ZD

dnl Checks for libraries and library functions
dnl ac_libbde_dummy is used to prevent AC_CHECK_LIB adding unnecessary -l<library> arguments

dnl Check for enabling libuna for Unicode string support
LIBBDE_CHECK_LIBUNA

AS_IF(
 [test "x$ac_libbde_have_libuna" = xyes],
 [dnl libuna is available
 AC_SUBST(
  [libbde_pc_libs_private_libuna],
  [-luna])
 AC_SUBST(
  [libbde_spec_requires_libuna],
  [libuna])
 AC_SUBST(
  [libbde_spec_build_requires_libuna],
  [libuna-devel])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBUNA_LIBADD],
  []) ],
 [dnl libuna is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBUNA],
  [1],
  [Define to 1 if the local version of libuna is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBUNA],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libuna],
  [])
 AC_SUBST(
  [libbde_spec_requires_libuna],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libuna],
  [])
 AC_SUBST(
  [LIBUNA_CPPFLAGS],
  [-I../libuna])
 AC_SUBST(
  [LIBUNA_LIBADD],
  [../libuna/libuna.la])
 ac_libbde_have_libuna=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBUNA],
 [test "x$ac_libbde_have_libuna" = xlocal])

dnl Check for enabling libbfio for basic file IO support
LIBBDE_CHECK_LIBBFIO

AS_IF(
 [test "x$ac_libbde_have_libbfio" = xyes],
 [dnl libbfio is available
 AC_SUBST(
  [libbde_pc_libs_private_libbfio],
  [-lbfio])
 AC_SUBST(
  [libbde_spec_requires_libbfio],
  [libbfio])
 AC_SUBST(
  [libbde_spec_build_requires_libbfio],
  [libbfio-devel])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [1]) ],
 [dnl libbfio is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBBFIO],
  [1],
  [Define to 1 if the local version of libbfio is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBBFIO],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libbfio],
  [])
 AC_SUBST(
  [libbde_spec_requires_libbfio],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libbfio],
  [])
 AC_SUBST(
  [LIBBFIO_CPPFLAGS],
  [-I../libbfio])
 AC_SUBST(
  [LIBBFIO_LIBADD],
  [../libbfio/libbfio.la])
 AC_SUBST(
  [HAVE_LIBBFIO],
  [0])
 ac_libbde_have_libbfio=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBBFIO],
 [test "x$ac_libbde_have_libbfio" = xlocal])

dnl Check for enabling libfdata for file and memory data block support
AC_CHECK_HEADERS(
 [libfdata.h],
 [],
 [ac_libbde_have_libfdata=no])

AS_IF(
	[test "x$ac_libbde_have_libfdata" != xno],
	[ac_libbde_have_libfdata=yes
	AC_CHECK_LIB(
	 fdata,
	 libfdata_get_version,
	 [],
	 [ac_libbde_have_libfdata=no])

	])

AS_IF(
[test "x$ac_libbde_have_libfdata" = xyes],
 [dnl libfdata is available
 AC_SUBST(
  [libbde_pc_libs_private_libfdata],
  [-lfdata])
 AC_SUBST(
  [libbde_spec_requires_libfdata],
  [libfdata])
 AC_SUBST(
  [libbde_spec_build_requires_libfdata],
  [libfdata-devel])
 AC_SUBST(
  [LIBFDATA_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBFDATA_LIBADD],
  []) ],
 [dnl libfdata is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBFDATA],
  [1],
  [Define to 1 if the local version of libfdata is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFDATA],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libfdata],
  [])
 AC_SUBST(
  [libbde_spec_requires_libfdata],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libfdata],
  [])
 AC_SUBST(
  [LIBFDATA_CPPFLAGS],
  [-I../libfdata])
 AC_SUBST(
  [LIBFDATA_LIBADD],
  [../libfdata/libfdata.la])
 ac_libbde_have_libfdata=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFDATA],
 [test "x$ac_libbde_have_libfdata" = xlocal])

dnl Check for enabling libfdatetime for file date and time support
LIBBDE_CHECK_LIBFDATETIME

AS_IF(
 [test "x$ac_libbde_have_libfdatetime" = xyes],
 [dnl libfdatetime is available
 AC_SUBST(
  [libbde_pc_libs_private_libfdatetime],
  [-lfdatetime])
 AC_SUBST(
  [libbde_spec_requires_libfdatetime],
  [libfdatetime])
 AC_SUBST(
  [libbde_spec_build_requires_libfdatetime],
  [libfdatetime-devel])
 AC_SUBST(
  [LIBFDATETIME_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBFDATETIME_LIBADD],
  []) ],
 [dnl libfdatetime is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBFDATETIME],
  [1],
  [Define to 1 if the local version of libfdatetime is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFDATETIME],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libfdatetime],
  [])
 AC_SUBST(
  [libbde_spec_requires_libfdatetime],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libfdatetime],
  [])
 AC_SUBST(
  [LIBFDATETIME_CPPFLAGS],
  [-I../libfdatetime])
 AC_SUBST(
  [LIBFDATETIME_LIBADD],
  [../libfdatetime/libfdatetime.la])
 ac_libbde_have_libfdatetime=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFDATETIME],
 [test "x$ac_libbde_have_libfdatetime" = xlocal])

dnl Check for enabling libfguid for file GUID/UUID support
AC_CHECK_HEADERS(
 [libfguid.h],
 [],
 [ac_libbde_have_libfguid=no])

AS_IF(
	[test "x$ac_libbde_have_libfguid" != xno],
	[ac_libbde_have_libfguid=yes
	AC_CHECK_LIB(
	 fguid,
	 libfguid_get_version,
	 [],
	 [ac_libbde_have_libfguid=no])

	])

AS_IF(
[test "x$ac_libbde_have_libfguid" = xyes],
 [dnl libfguid is available
 AC_SUBST(
  [libbde_pc_libs_private_libfguid],
  [-lfguid])
 AC_SUBST(
  [libbde_spec_requires_libfguid],
  [libfguid])
 AC_SUBST(
  [libbde_spec_build_requires_libfguid],
  [libfguid-devel])
 AC_SUBST(
  [LIBFGUID_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBFGUID_LIBADD],
  []) ],
 [dnl libfguid is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBFGUID],
  [1],
  [Define to 1 if the local version of libfguid is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFGUID],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libfguid],
  [])
 AC_SUBST(
  [libbde_spec_requires_libfguid],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libfguid],
  [])
 AC_SUBST(
  [LIBFGUID_CPPFLAGS],
  [-I../libfguid])
 AC_SUBST(
  [LIBFGUID_LIBADD],
  [../libfguid/libfguid.la])
 ac_libbde_have_libfguid=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFGUID],
 [test "x$ac_libbde_have_libfguid" = xlocal])

dnl Check for enabling libfvalue for file value support
LIBBDE_CHECK_LIBFVALUE

AS_IF(
[test "x$ac_libbde_have_libfvalue" = xyes],
 [dnl libfvalue is available
 AC_SUBST(
  [libbde_pc_libs_private_libfvalue],
  [-lfvalue])
 AC_SUBST(
  [libbde_spec_requires_libfvalue],
  [libfvalue])
 AC_SUBST(
  [libbde_spec_build_requires_libfvalue],
  [libfvalue-devel])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  []) ],
 [dnl libfvalue is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBFVALUE],
  [1],
  [Define to 1 if the local version of libfvalue is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBFVALUE],
  [1])
 AC_SUBST(
  [libbde_pc_libs_private_libfvalue],
  [])
 AC_SUBST(
  [libbde_spec_requires_libfvalue],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libfvalue],
  [])
 AC_SUBST(
  [LIBFVALUE_CPPFLAGS],
  [-I../libfvalue])
 AC_SUBST(
  [LIBFVALUE_LIBADD],
  [../libfvalue/libfvalue.la])
 ac_libbde_have_libfvalue=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBFVALUE],
 [test "x$ac_libbde_have_libfvalue" = xlocal])

dnl Check for message digest hash and crypto support
ac_libbde_have_cipher_support=no
ac_libbde_have_hmac_support=no

dnl Check for enabling libhmac for message digest hash function support
LIBBDE_CHECK_LIBHMAC

AS_IF(
 [test "x$ac_libbde_have_libhmac" = xyes],
 [dnl libhmac is available
 AC_SUBST(
  [libbde_spec_requires_libhmac],
  [libhmac])
 AC_SUBST(
  [libbde_spec_build_requires_libhmac],
  [libhmac-devel])
 AC_SUBST(
  [LIBHMAC_CPPFLAGS],
  [])
 AC_SUBST(
  [LIBHMAC_LIBADD],
  [])
 AC_SUBST(
  [HAVE_LIBHMAC],
  [1]) ],
 [dnl libhmac is not available, using the (local) version that came with the source package
 AC_DEFINE(
  [HAVE_LOCAL_LIBHMAC],
  [1],
  [Define to 1 if the local version of libhmac is used.])
 AC_SUBST(
  [HAVE_LOCAL_LIBHMAC],
  [1])
 AC_SUBST(
  [libbde_spec_requires_libhmac],
  [])
 AC_SUBST(
  [libbde_spec_build_requires_libhmac],
  [])
 AC_SUBST(
  [LIBHMAC_CPPFLAGS],
  [-I../libhmac])
 AC_SUBST(
  [LIBHMAC_LIBADD],
  [../libhmac/libhmac.la])
 AC_SUBST(
  [HAVE_LIBHMAC],
  [0])
 ac_libbde_have_libhmac=local])

AM_CONDITIONAL(
 [HAVE_LOCAL_LIBHMAC],
 [test "x$ac_libbde_have_libhmac" = xlocal])

AS_IF(
 [test "x$ac_libbde_have_libhmac" = xlocal],
 [ac_libbde_have_hmac_support=no],
 [ac_libbde_have_hmac_support=libhmac])

AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xlibhmac],
 [dnl TODO check if all LIBHMAC functions are available])

AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xlibhmac],
 [ac_libbde_have_hmac_md5_support=libhmac
 ac_libbde_have_hmac_sha1_support=libhmac
 ac_libbde_have_hmac_sha256_support=libhmac])

dnl Check for Windows crypto API support
AS_IF(
 [test "x$ac_libbde_have_cipher_support" = xno || test "x$ac_libbde_have_hmac_support" = xno],
 [AS_IF(
  [test "x$ac_cv_libbde_enable_winapi" = xyes],
  [AC_SUBST(
   [LIBCRYPTO_LIBADD],
   [-ladvapi32])
   ac_libbde_have_cipher_support=libadvapi32
   ac_libbde_have_hmac_support=libadvapi32])
 ])

AS_IF(
 [test "x$ac_libbde_have_cipher_support" = xlibadvapi32],
 [ac_libbde_have_cipher_aes_support=libadvapi32])

AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xlibadvapi32],
 [ac_libbde_have_hmac_md5_support=libadvapi32
 ac_libbde_have_hmac_sha1_support=libadvapi32
 ac_libbde_have_hmac_sha256_support=libadvapi32])

AS_IF(
 [test "x$ac_libbde_have_hmac_md5_support" = xlibadvapi32],
 [LIBBDE_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_MD5])

 AS_IF(
  [test "x$ac_cv_libbde_winapi_define_CALG_MD5" = xyes],
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_md5_support=local])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha1_support" = xlibadvapi32],
 [LIBBDE_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA1])

 AS_IF(
  [test "x$ac_cv_libbde_winapi_define_CALG_SHA1" = xyes],
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_sha1_support=local])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha256_support" = xlibadvapi32],
 [LIBBDE_CHECK_WINAPI_DEFINE(
  [wincrypt.h],
  [CALG_SHA_256])

 AS_IF(
  [test "x$ac_cv_libbde_winapi_define_CALG_SHA_256" = xyes],
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_sha256_support=local])
 ])

dnl Check for libcrypto (openssl) EVP support
AS_IF(
 [test "x$ac_libbde_have_cipher_support" = xno || test "x$ac_libbde_have_hmac_support" = xno],
 [AC_CHECK_HEADERS([openssl/evp.h openssl/opensslv.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xyes],
  [LIBBDE_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE

  AS_IF(
   [test "x$ac_cv_libbde_openssl_evp_zlib_compatible" = xyes],
   [ac_libbde_have_cipher_support=libcrypto_evp
   ac_libbde_have_hmac_support=libcrypto_evp],
   [ac_libbde_have_cipher_support=no
   ac_libbde_have_hmac_support=no]) ],
  [ac_libbde_have_cipher_support=no
  ac_libbde_have_hmac_support=no])
 ])

dnl Check if all required libcrypto (openssl) EVP CIPHER functions are available
AS_IF(
 [test "x$ac_libbde_have_cipher_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_CIPHER_CTX_init,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_CIPHER_CTX_cleanup,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DecryptInit,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DecryptUpdate,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DecryptFinal,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_EncryptInit,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_EncryptUpdate,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_EncryptFinal,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_support=no])
 ])

AS_IF(
 [test "x$ac_libbde_have_cipher_support" = xlibcrypto_evp],
 [ac_libbde_have_openssl_evp=yes
 ac_libbde_have_libcrypto=yes
 ac_libbde_have_cipher_aes_support=libcrypto_evp])

AS_IF(
 [test "x$ac_libbde_have_cipher_aes_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_aes_128_ecb,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_aes_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_aes_192_ecb,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_aes_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_aes_256_ecb,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_cipher_aes_support=no])
 ])

dnl Check for libcrypto (openssl) AES support
AS_IF(
 [test "x$ac_libbde_have_cipher_aes_support" = xno],
 [AC_CHECK_HEADERS([openssl/aes.h])
 dnl TODO check for AES functions
 ac_libbde_have_cipher_aes_support=libcrypto
 ])

dnl Check if all required libcrypto (openssl) EVP MD functions are available
AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_init,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_MD_CTX_cleanup,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestInit_ex,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestUpdate,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_support=no])
 AC_CHECK_LIB(
  crypto,
  EVP_DigestFinal_ex,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_support=no])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xlibcrypto_evp],
 [ac_libbde_have_openssl_evp=yes
 ac_libbde_have_libcrypto=yes
 ac_libbde_have_hmac_md5_support=libcrypto_evp
 ac_libbde_have_hmac_sha1_support=libcrypto_evp
 ac_libbde_have_hmac_sha256_support=libcrypto_evp])

AS_IF(
 [test "x$ac_libbde_have_hmac_md5_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_md5,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_md5_support=no])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha1_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_sha1,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_sha1_support=no])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha256_support" = xlibcrypto_evp],
 [AC_CHECK_LIB(
  crypto,
  EVP_sha256,
  [ac_libbde_dummy=yes],
  [ac_libbde_have_hmac_sha256_support=no])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_support" = xno],
 [ac_libbde_have_hmac_md5_support=no
 ac_libbde_have_hmac_sha1_support=no
 ac_libbde_have_hmac_sha256_support=no])

dnl Check for libcrypto (openssl) MD5 support
AS_IF(
 [test "x$ac_libbde_have_hmac_md5_support" = xno],
 [AC_CHECK_HEADERS([openssl/md5.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_md5_h" = xyes],
  [ac_libbde_have_hmac_md5_support=libcrypto
  AC_CHECK_LIB(
   crypto,
   MD5_Init,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_md5_support=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Update,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_md5_support=no])
  AC_CHECK_LIB(
   crypto,
   MD5_Final,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_md5_support=no])
  ])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_md5_support" != xno],
 [ac_libbde_have_libcrypto=yes],
 [ac_libbde_have_hmac_md5_support=local])

dnl Check for libcrypto (openssl) SHA1 support
AS_IF(
 [test "x$ac_libbde_have_hmac_sha1_support" = xno],
 [AC_CHECK_HEADERS([openssl/sha.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_sha1_h" = xyes],
  [ac_libbde_have_hmac_sha1_support=libcrypto
  AC_CHECK_LIB(
   crypto,
   SHA1_Init,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha1_support=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Update,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha1_support=no])
  AC_CHECK_LIB(
   crypto,
   SHA1_Final,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha1_support=no])
  ])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha1_support" != xno],
 [ac_libbde_have_libcrypto=yes],
 [ac_libbde_have_hmac_sha1_support=local])

dnl Check for libcrypto (openssl) SHA256 support
AS_IF(
 [test "x$ac_libbde_have_hmac_sha256_support" = xno],
 [AC_CHECK_HEADERS([openssl/sha.h])

 AS_IF(
  [test "x$ac_cv_header_openssl_sha256_h" = xyes],
  [ac_libbde_have_hmac_sha256_support=libcrypto
  AC_CHECK_LIB(
   crypto,
   SHA256_Init,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha256_support=no])
  AC_CHECK_LIB(
   crypto,
   SHA256_Update,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha256_support=no])
  AC_CHECK_LIB(
   crypto,
   SHA256_Final,
   [ac_libbde_dummy=yes],
   [ac_libbde_have_hmac_sha256_support=no])
  ])
 ])

AS_IF(
 [test "x$ac_libbde_have_hmac_sha256_support" != xno],
 [ac_libbde_have_libcrypto=yes],
 [ac_libbde_have_hmac_sha256_support=local])

dnl Setup libcrypto (openssl) parameters
AS_IF(
 [test "x$ac_libbde_have_openssl_evp" = xyes],
 [AC_DEFINE(
  [HAVE_OPENSSL_EVP_H],
  [1],
  [Define to 1 if you have the <openssl/evp.h> header file.])
 AC_SUBST(
  [HAVE_OPENSSL_EVP_H],
  [1]) ],
 [AC_SUBST(
  [HAVE_OPENSSL_EVP_H],
  [0])
 ])

AS_IF(
 [test "x$ac_libbde_have_libcrypto" = xyes],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  ["-lcrypto"])
 AC_DEFINE(
  [HAVE_LIBCRYPTO],
  [1],
  [Define to 1 if you have the 'crypto' library (-lcrypto).])
 dnl Enforce the dynamic loader library to be included if available
 AC_CHECK_LIB(
  dl,
  dlopen,
  [AC_SUBST(
   [LIBDL_LIBADD],
   ["-ldl"])],
  [])
 ])

dnl Check if libbde should be build with wide character type support
AS_IF(
 [test "x$ac_cv_libbde_enable_winapi" = xyes],
 [ac_cv_libbde_enable_wide_character_type=yes])

AS_IF(
 [test "x$ac_cv_libbde_enable_wide_character_type" != xno ],
 [dnl Check for the wide character header
 AC_CHECK_HEADERS(
  [wchar.h],
  [AC_DEFINE(
   [HAVE_WCHAR_H],
   [1],
   [Define to 1 if you have the <wchar.h> header file.])
  AC_SUBST(
   [HAVE_WCHAR_H],
   [1]) ],
  [AC_MSG_FAILURE(
   [Missing header: wchar.h],
   [1]) ])

 dnl Determine wide character byte size
 AC_CHECK_SIZEOF([wchar_t])

 dnl Check for wide character string functions

 dnl Wide character string functions used in common/file_stream.h
 AC_CHECK_FUNCS(
  [fgetws],
  [],
  [AC_MSG_FAILURE(
   [Missing function: fgetws],
   [1]) ])

 dnl Wide character string functions used in libcstring/libcstring_wide_string.h
 AC_CHECK_FUNCS(
  [swprintf],
  [],
  [AC_MSG_FAILURE(
   [Missing function: swprintf],
   [1]) ])

 AC_CHECK_FUNCS(
  [wmemchr],
  [],
  [AC_CHECK_FUNCS(
   [wcschr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemchr and wcschr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcmp],
  [],
  [AC_CHECK_FUNCS(
   [wcsncmp],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcmp and wcsncmp],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemcpy],
  [],
  [AC_CHECK_FUNCS(
   [wcsncpy],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemcpy and wcsncpy],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wmemrchr],
  [],
  [AC_CHECK_FUNCS(
   [wcsrchr],
   [],
   [AC_MSG_FAILURE(
    [Missing functions: wmemrchr and wcsrchr],
    [1]) ]) ])

 AC_CHECK_FUNCS(
  [wcslen],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcslen],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcsncasecmp],
  [],
  [AC_CHECK_FUNCS(
   [wcscasecmp],
   [],
   [AC_CHECK_FUNCS(
    [towlower],
    [],
    [AC_MSG_FAILURE(
     [Missing functions: wcsncasecmp, wcscasecmp and towlower],
     [1]) ]) ]) ])

 AC_CHECK_FUNCS(
  [wcsstr],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcsstr],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcstoll],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoll],
   [1]) ])

 AC_CHECK_FUNCS(
  [wcstoull],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstoull],
   [1]) ])

 dnl Wide character string functions used in liberror/liberror_error.h
 AC_CHECK_FUNCS(
  [wcstombs],
  [],
  [AC_MSG_FAILURE(
   [Missing function: wcstombs],
   [1]) ])

 dnl Make the necessary definitions
 AC_DEFINE(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1],
  [Define to 1 if wide character type should be used.])
 AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [1])
 ac_cv_libbde_enable_wide_character_type=yes],

 [AC_SUBST(
  [HAVE_WIDE_CHARACTER_TYPE],
  [0])
 AC_SUBST(
  [HAVE_WCHAR_H],
  [0]) ])

AC_SUBST(
 [HAVE_WIDE_CHARACTER_SUPPORT_FUNCTIONS],
 [0])

dnl Check if bdetools should be build as static executables
AS_IF(
 [test "x$ac_cv_libbde_enable_static_executables" != xno],
 [STATIC_LDFLAGS="-all-static";
 AC_SUBST(
  [STATIC_LDFLAGS])
 AC_DEFINE(
  [HAVE_STATIC_EXECUTABLES],
  [1],
  [Define if executables should be static compiled.])
 ac_cv_libbde_enable_static_executables=yes])

dnl Check if libbde should be build with verbose output
AS_IF(
 [test "x$ac_cv_libbde_enable_verbose_output" != xno ],
 [AC_DEFINE(
  [HAVE_VERBOSE_OUTPUT],
  [1],
  [Define to 1 if verbose output should be used.])
 ac_cv_libbde_enable_verbose_output=yes])

dnl Check if libbde should be build with debug output
AS_IF(
 [test "x$ac_cv_libbde_enable_debug_output" != xno ],
 [AC_DEFINE(
  [HAVE_DEBUG_OUTPUT],
  [1],
  [Define to 1 if debug output should be used.])
 ac_cv_libbde_enable_debug_output=yes])

dnl Check if requires and build requires should be set in spec file
AS_IF(
 [test "x$ac_libbde_have_libuna" = xyes || test "x$ac_libbde_have_libbfio" = xyes || test "x$ac_libbde_have_libfdata" = xyes || test "x$ac_libbde_have_libfdatetime" = xyes || test "x$ac_libbde_have_libfguid" = xyes || test "x$ac_libbde_have_libfvalue" = xyes],
 [AC_SUBST(
  [libbde_spec_requires],
  [Requires:])
 AC_SUBST(
  [libbde_spec_build_requires],
  [BuildRequires:]) ],
 [AC_SUBST(
  [libbde_spec_requires],
  [])
 AC_SUBST(
  [libbde_spec_build_requires],
  [])
 ac_libbde_dummy=yes])

dnl Set memwatch support
ac_libbde_have_memwatch=no

AS_IF(
 [test "x$ac_libbde_have_memwatch" != xno ],
 [AC_SUBST(
  [MEMWATCH_CPPFLAGS],
  ["-DMEMWATCH -DMEMWATCH_STDIO"])
 AC_SUBST(
  [MEMWATCH_LIBADD],
  [../common/libmemwatch.la]) ])

AM_CONDITIONAL(
 [HAVE_MEMWATCH],
 [test "x$ac_libbde_have_memwatch" != xno])

dnl Set additional compiler flags
CFLAGS="$CFLAGS -Wall";

dnl Check if DLL support is needed in bdetools
AS_IF(
 [test x"$enable_shared" = xyes],
 [AS_CASE(
  [$host],
  [*cygwin* | *mingw*],
  [AC_SUBST(
   [LIBBDE_DLL_IMPORT],
   ["-DLIBBDE_DLL_IMPORT"])
  ])
 ])

dnl Have configure make the Makefiles
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([common/Makefile])
AC_CONFIG_FILES([libcstring/Makefile])
AC_CONFIG_FILES([liberror/Makefile])
AC_CONFIG_FILES([libnotify/Makefile])
AC_CONFIG_FILES([libuna/Makefile])
AC_CONFIG_FILES([libbfio/Makefile])
AC_CONFIG_FILES([libfdata/Makefile])
AC_CONFIG_FILES([libfdatetime/Makefile])
AC_CONFIG_FILES([libfguid/Makefile])
AC_CONFIG_FILES([libfvalue/Makefile])
AC_CONFIG_FILES([libhmac/Makefile])
AC_CONFIG_FILES([libbde/Makefile])
AC_CONFIG_FILES([libsystem/Makefile])
AC_CONFIG_FILES([bdetools/Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_FILES([po/Makevars])
AC_CONFIG_FILES([manuals/Makefile])
dnl Have configure make the include files
AC_CONFIG_FILES([include/libbde.h])
AC_CONFIG_FILES([include/libbde/definitions.h])
AC_CONFIG_FILES([include/libbde/features.h])
AC_CONFIG_FILES([include/libbde/types.h])
AC_CONFIG_FILES([libbde/libbde_definitions.h])
dnl Have configure make distribution specific files
AC_CONFIG_FILES([common/types.h])
AC_CONFIG_FILES([libbde/libbde.rc])
AC_CONFIG_FILES([libbde.pc])
AC_CONFIG_FILES([libbde.spec])
dnl Have configure write its configuration
AC_CONFIG_HEADERS([common/config.h])

AC_OUTPUT

dnl Print a summary
AC_MSG_NOTICE([
Building:
   libuna support:                           $ac_libbde_have_libuna
   libbfio support:                          $ac_libbde_have_libbfio
   libfdata support:                         $ac_libbde_have_libfdata
   libfdatetime support:                     $ac_libbde_have_libfdatetime
   libfguid support:                         $ac_libbde_have_libfguid
   libfvalue support:                        $ac_libbde_have_libfvalue
   libhmac support:                          $ac_libbde_have_libhmac
   SHA256 support:                           $ac_libbde_have_hmac_sha256_support
   AES support:                              $ac_libbde_have_cipher_aes_support

Features:
   Wide character type support:              $ac_cv_libbde_enable_wide_character_type
   bdetools are build as static executables: $ac_cv_libbde_enable_static_executables
   Verbose output:                           $ac_cv_libbde_enable_verbose_output
   Debug output:                             $ac_cv_libbde_enable_debug_output
]);


