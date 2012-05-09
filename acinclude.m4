dnl Function to detect if libbde dependencies are available
AC_DEFUN([AX_LIBBDE_CHECK_LOCAL],
 [dnl Check for internationalization functions in libbde/libbde_i18n.c 
 AC_CHECK_FUNCS([bindtextdomain])
 ])

