dnl Functions for libcaes

dnl Function to detect if libcaes is available
AC_DEFUN([AC_CHECK_LIBCAES],
 [dnl Check if parameters were provided
 AS_IF(
  [test x"$ac_cv_with_libcaes" != x && test "x$ac_cv_with_libcaes" != xno && test "x$ac_cv_with_libcaes" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_libcaes"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_libcaes}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_libcaes}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_libcaes])
   ])
  ])

 AS_IF(
  [test x"$ac_cv_with_libcaes" != xno],
  [dnl Check for headers
  AC_CHECK_HEADERS([libcaes.h])

  AS_IF(
   [test "x$ac_cv_header_libcaes_h" = xno],
   [ac_cv_libcaes=no],
   [ac_cv_libcaes=yes
   AC_CHECK_LIB(
    caes,
    libcaes_get_version,
    [ac_cv_libcaes_dummy=yes],
    [ac_cv_libcaes=no])

    dnl TODO check if all LIBCAES functions are available
   ])
  ])

 AS_IF(
  [test "x$ac_cv_libcaes" = xyes],
  [AC_DEFINE(
   [HAVE_LIBCAES],
   [1],
   [Define to 1 if you have the `caes' library (-lcaes).])
  LIBS="-lcaes $LIBS"
  ])

 AS_IF(
  [test "x$ac_cv_libcaes" = xyes],
  [ac_cv_libcaes_aes=libcaes],
  [ac_cv_libcaes_aes=no])

 AS_IF(
  [test "x$ac_cv_libcaes" = xyes],
  [AC_SUBST(
   [HAVE_LIBCAES],
   [1]) ],
  [AC_SUBST(
   [HAVE_LIBCAES],
   [0])
  ])
 ])

dnl Function to detect if wincrypt AES functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBCAES_WINCRYPT],
 [AC_SUBST(
  [LIBCRYPTO_LIBADD],
  [-ladvapi32])

 ac_cv_libcaes_aes=libadvapi32 
 ])
 
dnl Function to detect if openssl EVP AES functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBCAES_OPENSSL_EVP],
 [AC_CHECK_HEADERS([openssl/evp.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_evp_h" = xno],
  [ac_cv_libcrypto=no],
  [AC_CHECK_OPENSSL_EVP_ZLIB_COMPATIBILE
 
  AS_IF(
   [test "x$ac_cv_openssl_evp_zlib_compatible" = xyes],
   [ac_cv_libcrypto=evp],
   [ac_cv_libcrypto=no])
 ])

 dnl Check if all required libcrypto (openssl) EVP MD functions are available
 AS_IF(
  [test "x$ac_cv_libcrypto" = xevp],
  [ac_cv_libcaes_aes=libcrypto_evp
  AC_CHECK_LIB(
   crypto,
   EVP_CIPHER_CTX_init,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_CIPHER_CTX_cleanup,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_CIPHER_CTX_set_padding,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])

  AC_CHECK_LIB(
   crypto,
   EVP_CipherInit_ex,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_CipherUpdate,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_CipherFinal_ex,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  ])
 ])

dnl Function to detect if openssl AES functions are available
AC_DEFUN([AC_CHECK_LOCAL_LIBCAES_OPENSSL_AES],
 [AC_CHECK_HEADERS([openssl/aes.h])
 
 AS_IF(
  [test "x$ac_cv_header_openssl_aes_h" = xno],
  [ac_cv_libcaes_aes=no],
  [ac_cv_libcaes_aes=libcrypto
  AC_CHECK_LIB(
   crypto,
   EVP_aes_128_ecb,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_aes_192_ecb,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  AC_CHECK_LIB(
   crypto,
   EVP_aes_256_ecb,
   [ac_cv_libcaes_dummy=yes],
   [ac_cv_libcaes_aes=no])
  ])
 ])
 
dnl Function to detect if libcaes dependencies are available
AC_DEFUN([AC_CHECK_LOCAL_LIBCAES],
 [dnl Check if parameters were provided 
 AS_IF(
  [test x"$ac_cv_with_openssl" != x && test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_with_openssl" != xauto-detect],
  [AS_IF(
   [test -d "$ac_cv_with_openssl"],
   [CFLAGS="$CFLAGS -I${ac_cv_with_openssl}/include"
   LDFLAGS="$LDFLAGS -L${ac_cv_with_openssl}/lib"],
   [AC_MSG_WARN([no such directory: $ac_cv_with_openssl])
   ])
  ])

 ac_cv_libcrypto=no 
 ac_cv_libcaes_aes=no

 dnl Check for Windows crypto API support
 AS_IF(
  [test "x$ac_cv_enable_winapi" = xyes],
  [AC_CHECK_LOCAL_LIBCAES_WINCRYPT])

 dnl Check for libcrypto (openssl) support
 AS_IF(
  [test "x$ac_cv_with_openssl" != xno],
  [dnl Check for libcrypto (openssl) EVP support
  AS_IF(
   [test "x$ac_cv_libcaes_aes" = xno],
   [AC_CHECK_LOCAL_LIBCAES_OPENSSL_EVP])

  dnl Check for libcrypto (openssl) AES support
  AS_IF(
   [test "x$ac_cv_with_openssl" != xno && test "x$ac_cv_libcaes_aes" = xno],
   [AC_CHECK_LOCAL_LIBCAES_OPENSSL_AES])
  ])
  
 dnl Fallback to local versions if necessary 
 AS_IF(
  [test "x$ac_cv_libcaes_aes" = xno],
  [ac_cv_libcaes_aes=local])
 

 AS_IF(
  [test "x$ac_cv_libcrypto" = xno],
  [AS_IF(
   [test "x$ac_cv_libcaes_aes" = xlibcrypto],
   [ac_cv_libcrypto=yes])
  ])

 dnl Setup libcrypto (openssl) parameters
 AS_IF(
  [test "x$ac_cv_libcrypto" = xevp],
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
  [test "x$ac_cv_libcrypto" != xno],
  [AC_CHECK_HEADERS([openssl/opensslv.h])

  AC_SUBST(
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
 ])

