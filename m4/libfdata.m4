dnl Functions for libfdata

dnl Function to detect if libfdata is available
AC_DEFUN([AC_CHECK_LIBFDATA],
 [AC_CHECK_HEADERS([libfdata.h])

 AS_IF(
  [test "x$ac_cv_header_libfdata_h" = xno],
  [ac_cv_libfdata=no],
  [ac_cv_libfdata=yes
  AC_CHECK_LIB(
   fdatetime,
   libfdata_get_version,
   [],
   [ac_cv_libfdata=no])
 
  dnl TODO add functions
  ])

 AS_IF(
  [test "x$ac_cv_libfdata" = xyes],
  [AC_SUBST(
   [HAVE_LIBFDATA],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBFDATA],
   [0])
  ])
 ])

