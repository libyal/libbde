dnl Functions for libfguid

dnl Function to detect if libfguid is available
AC_DEFUN([AC_CHECK_LIBFGUID],
 [AC_CHECK_HEADERS([libfguid.h])

 AS_IF(
  [test "x$ac_cv_header_libfguid_h" = xno],
  [ac_cv_libfguid=no],
  [ac_cv_libfguid=yes
  AC_CHECK_LIB(
   fdatetime,
   libfguid_get_version,
   [],
   [ac_cv_libfguid=no])
 
  dnl TODO add functions
  ])

 AS_IF(
  [test "x$ac_cv_libfguid" = xyes],
  [AC_SUBST(
   [HAVE_LIBFGUID],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFGUID],
   [0])
  ])
 ])

