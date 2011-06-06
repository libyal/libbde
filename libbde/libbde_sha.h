/*
 * SHA-256 functions
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

#if !defined( _LIBBDE_SHA_H )
#define _LIBBDE_SHA_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LIBBDE_SHA256_HASH_SIZE		32

#if defined( WINAPI )
typedef struct libbde_sha256_context libbde_sha256_context_t;

struct libbde_sha256_context
{
        HCRYPTPROV crypt_provider;
        HCRYPTHASH hash;
};

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
typedef EVP_MD_CTX libbde_sha256_context_t;

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
typedef SHA256_CTX libbde_sha256_context_t;

#else
#define LIBBDE_SHA256_BLOCK_SIZE	64

typedef struct libbde_sha256_context libbde_sha256_context_t;

struct libbde_sha256_context
{
/* TODO code clean up for fallback */

	/* The block index
	 */
	size_t block_index;

	size_t size;
	size_t total_size;
	uint8_t block[ 128 ];

	/* The last calculated h-values
	 */
	uint32_t h[ 8 ];
};

#endif

int libbde_sha256_initialize(
     libbde_sha256_context_t *context,
     liberror_error_t **error );

int libbde_sha256_update(
     libbde_sha256_context_t *context,
     const uint8_t *buffer,
     size_t size,
     liberror_error_t **error );

int libbde_sha256_finalize(
     libbde_sha256_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error );

int libbde_sha256_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error );

#ifdef __cplusplus
}
#endif

#endif

