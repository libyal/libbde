/*
 * FIPS-197 compliant AES encryption functions
 *
 * Copyright (C) 2011, Joachim Metz <jbmetz@users.sourceforge.net>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBBDE_AES_H )
#define _LIBBDE_AES_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define AES_ENCRYPT	0
#define AES_DECRYPT	1

typedef struct
{
    int nr;
    unsigned long *rk;
    unsigned long buf[68];
}
aes_context;

void aes_setkey_enc(
      aes_context *ctx,
      unsigned char *key,
      int keysize );

void aes_setkey_dec(
      aes_context *ctx,
      unsigned char *key,
      int keysize );

void aes_crypt_ecb(
      aes_context *ctx,
      int mode,
      unsigned char input[16],
      unsigned char output[16] );

void aes_crypt_cbc(
      aes_context *ctx,
      int mode,
      int length,
      unsigned char iv[16],
      unsigned char *input,
      unsigned char *output );

void aes_crypt_cfb(
      aes_context *ctx,
      int mode,
      int length,
      int *iv_off,
      unsigned char iv[16],
      unsigned char *input,
      unsigned char *output );

int aes_self_test(
     int verbose );

#ifdef __cplusplus
}
#endif

#endif

