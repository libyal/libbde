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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#if defined( WINAPI )
#include <wincrypt.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
#include <openssl/evp.h>

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
#include <openssl/sha.h>

#endif

#include "libbde_sha.h"

#if defined( WINAPI )

#if !defined( PROV_RSA_AES )
#define PROV_RSA_AES		24
#endif

#endif /* defined( WINAPI ) */

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_SHA_H ) )

/* FIPS 180-2 compliant SHA-256 functions
 */

/* The first 32-bits of the fractional parts of the square roots of the first 8 primes 2..19
 */
uint32_t libbde_sha256_prime_square_roots[ 8 ] = {
	0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL, 0xa54ff53aUL,
	0x510e527fUL, 0x9b05688cUL, 0x1f83d9abUL, 0x5be0cd19UL
};

/* The first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311
 */
uint32_t libbde_sha256_prime_cube_roots[ 64 ] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define libbde_sha256_transform_unfolded_copy_data_to_32bit_values( data, values_32bit ) \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 0 ] ), \
		 values_32bit[ 0 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 4 ] ), \
		 values_32bit[ 1 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 8 ] ), \
		 values_32bit[ 2 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 12 ] ), \
		 values_32bit[ 3 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 16 ] ), \
		 values_32bit[ 4 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 20 ] ), \
		 values_32bit[ 5 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 24 ] ), \
		 values_32bit[ 6 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 28 ] ), \
		 values_32bit[ 7 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 32 ] ), \
		 values_32bit[ 8 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 36 ] ), \
		 values_32bit[ 9 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 40 ] ), \
		 values_32bit[ 10 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 44 ] ), \
		 values_32bit[ 11 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 48 ] ), \
		 values_32bit[ 12 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 52 ] ), \
		 values_32bit[ 13 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( ( data )[ 56 ] ), \
		 values_32bit[ 14 ] ); \
 \
		byte_stream_copy_to_uint32_little_endian( \
		 &( data[ 60 ] ), \
		 values_32bit[ 15 ] );

#define libbde_sha256_transform_extend_32bit_value( values_32bit, value_32bit_index, s0, s1 ) \
	s0 = byte_stream_bit_rotate_right( values_32bit[ value_32bit_index - 15 ], 7 ) \
	   ^ byte_stream_bit_rotate_right( values_32bit[ value_32bit_index - 15 ], 18 ) \
	   ^ byte_stream_bit_shift_right( values_32bit[ value_32bit_index - 15 ], 3 ); \
	s1 = byte_stream_bit_rotate_right( values_32bit[ value_32bit_index - 2 ], 17 ) \
	   ^ byte_stream_bit_rotate_right( values_32bit[ value_32bit_index - 2 ], 19 ) \
	   ^ byte_stream_bit_shift_right( values_32bit[ value_32bit_index - 2 ], 10 ); \
	values_32bit[ value_32bit_index ] = values_32bit[ value_32bit_index - 16 ] + s0 + values_32bit[ value_32bit_index - 7 ] + s1

#define libbde_sha256_transform_unfolded_extend_32bit_values( values_32bit, s0, s1 ) \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 16, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 17, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 18, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 19, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 20, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 21, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 22, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 23, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 24, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 25, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 26, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 27, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 28, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 29, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 30, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 31, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 32, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 33, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 34, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 35, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 36, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 37, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 38, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 39, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 40, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 41, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 42, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 43, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 44, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 45, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 46, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 47, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 48, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 49, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 50, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 51, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 52, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 53, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 54, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 55, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 56, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 57, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 58, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 59, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 60, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 61, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 62, s0, s1 ); \
	libbde_sha256_transform_extend_32bit_value( values_32bit, 63, s0, s1 );

#define libbde_sha256_transform_calculate_32bit_value( values_32bit, value_32bit_index, hash_values, s0, s1, t1, t2 ) \
	s0 = byte_stream_bit_rotate_right( hash_values[ 0 ], 2 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ 0 ], 13 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ 0 ], 22 ); \
	t2 = s0 \
	   + ( ( hash_values[ 0 ] & hash_values[ 1 ] ) \
	     ^ ( hash_values[ 0 ] & hash_values[ 2 ] ) \
	     ^ ( hash_values[ 1 ] & hash_values[ 2 ] ) ); \
	s1 = byte_stream_bit_rotate_right( hash_values[ 4 ], 6 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ 4 ], 11 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ 4 ], 25 ); \
	t1 = hash_values[ 7 ] \
	   + s1 \
	   + ( ( hash_values[ 4 ] & hash_values[ 5 ] ) \
	     ^ ( ~( hash_values[ 4 ] ) & hash_values[ 6 ] ) ) \
	   + libbde_sha256_prime_cube_roots[ value_32bit_index ] \
	   + values_32bit[ value_32bit_index ]; \
	hash_values[ 7 ] = hash_values[ 6 ]; \
	hash_values[ 6 ] = hash_values[ 5 ]; \
	hash_values[ 5 ] = hash_values[ 4 ]; \
	hash_values[ 4 ] = hash_values[ 3 ] + t1; \
	hash_values[ 3 ] = hash_values[ 2 ]; \
	hash_values[ 2 ] = hash_values[ 1 ]; \
	hash_values[ 1 ] = hash_values[ 0 ]; \
	hash_values[ 0 ] = t1 + t2;

#define libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, value_32bit_index, hash_values, hash_value_index0, hash_value_index1, hash_value_index2, hash_value_index3, hash_value_index4, hash_value_index5, hash_value_index6, hash_value_index7, s0, s1, t1, t2 ) \
	s0 = byte_stream_bit_rotate_right( hash_values[ hash_value_index0 ], 2 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ hash_value_index0 ], 13 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ hash_value_index0 ], 22 ); \
	t2 = s0 \
	   + ( ( hash_values[ hash_value_index0 ] & hash_values[ hash_value_index1 ] ) \
	     ^ ( hash_values[ hash_value_index0 ] & hash_values[ hash_value_index2 ] ) \
	     ^ ( hash_values[ hash_value_index1 ] & hash_values[ hash_value_index2 ] ) ); \
	s1 = byte_stream_bit_rotate_right( hash_values[ hash_value_index4 ], 6 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ hash_value_index4 ], 11 ) \
	   ^ byte_stream_bit_rotate_right( hash_values[ hash_value_index4 ], 25 ); \
	t1 = hash_values[ hash_value_index7 ] \
	   + s1 \
	   + ( ( hash_values[ hash_value_index4 ] & hash_values[ hash_value_index5 ] ) \
	     ^ ( ~( hash_values[ hash_value_index4 ] ) & hash_values[ hash_value_index6 ] ) ) \
	   + libbde_sha256_prime_cube_roots[ value_32bit_index ] \
	   + values_32bit[ value_32bit_index ]; \
	hash_values[ hash_value_index3 ] += t1; \
	hash_values[ hash_value_index7 ] = t1 + t2;

#define libbde_sha256_transform_unfolded_calculate_32bit_values( values_32bit, hash_values, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 0, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 1, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 2, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 3, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 4, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 5, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 6, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 7, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 8, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 9, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 10, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 11, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 12, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 13, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 14, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 15, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 16, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 17, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 18, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 19, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 20, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 21, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 22, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 23, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 24, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 25, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 26, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 27, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 28, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 29, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 30, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 31, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 32, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 33, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 34, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 35, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 36, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 37, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 38, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 39, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 40, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 41, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 42, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 43, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 44, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 45, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 46, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 47, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 48, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 49, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 50, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 51, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 52, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 53, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 54, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 55, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 56, hash_values, 0, 1, 2, 3, 4, 5, 6, 7, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 57, hash_values, 7, 0, 1, 2, 3, 4, 5, 6, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 58, hash_values, 6, 7, 0, 1, 2, 3, 4, 5, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 59, hash_values, 5, 6, 7, 0, 1, 2, 3, 4, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 60, hash_values, 4, 5, 6, 7, 0, 1, 2, 3, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 61, hash_values, 3, 4, 5, 6, 7, 0, 1, 2, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 62, hash_values, 2, 3, 4, 5, 6, 7, 0, 1, s0, s1, t1, t2 ) \
	libbde_sha256_transform_unfolded_calculate_32bit_value( values_32bit, 63, hash_values, 1, 2, 3, 4, 5, 6, 7, 0, s0, s1, t1, t2 )

/* Transforms (calculates) data into a SHA-256 hash
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_transform(
     libbde_sha256_context_t *context,
     const uint8_t *data,
     size_t number_of_blocks,
     liberror_error_t **error )
{
	uint32_t hash_values[ 8 ];
	uint32_t values_32bit[ 64 ];

	uint32_t s0               = 0;
	uint32_t s1               = 0;
	uint32_t t1               = 0;
	uint32_t t2               = 0;
	size_t data_offset        = 0;
	size_t block_index        = 0;

#if !defined( LIBBDE_SHA_UNFOLLED_LOOPS )
	uint8_t value_32bit_index = 0;
#endif

	static char *function = "libbde_sha256_transform";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	for( block_index = 0;
	     block_index < number_of_blocks;
	     block_index++ )
	{
		data_offset = block_index * LIBBDE_SHA256_BLOCK_SIZE;

		if( memory_copy(
		     hash_values,
		     context->hash_values,
		     sizeof( uint32_t ) * 8 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash values.",
			 function );

			return( -1 );
		}
		/* Break the data into 16 x 32-bit values
		 */
#if defined( LIBBDE_SHA_UNFOLLED_LOOPS )
		libbde_sha256_transform_unfolded_copy_data_to_32bit_values(
		 &( data[ data_offset ] ),
		 values_32bit );
#else
		for( value_32bit_index = 0;
		     value_32bit_index < 16;
		     value_32bit_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( data[ data_offset ] ),
			 values_32bit[ value_32bit_index ] );

			data_offset += sizeof( uint32_t );
		}
#endif /* defined( LIBBDE_SHA_UNFOLLED_LOOPS ) */

		/* Extend to 64 x 32-bit values
		 */
#if defined( LIBBDE_SHA_UNFOLLED_LOOPS )
		libbde_sha256_transform_unfolded_extend_32bit_values(
		 values_32bit,
		 s0,
		 s1 );
#else
		for( value_32bit_index = 16;
		     value_32bit_index < 64;
		     value_32bit_index++ )
		{
			libbde_sha256_transform_extend_32bit_value(
			 values_32bit,
			 value_32bit_index,
			 s0,
			 s1 );
		}
#endif /* defined( LIBBDE_SHA_UNFOLLED_LOOPS ) */

		/* Calculate the hash values for the 32-bit values
		 */
#if defined( LIBBDE_SHA_UNFOLLED_LOOPS )
		libbde_sha256_transform_unfolded_calculate_32bit_values(
		 values_32bit,
		 hash_values,
		 s0,
		 s1,
		 t1,
		 t2 );
#else
		for( value_32bit_index = 0;
		     value_32bit_index < 64;
		     value_32bit_index++ )
		{
			libbde_sha256_transform_calculate_32bit_value(
			 values_32bit,
			 value_32bit_index,
			 hash_values,
			 s0,
			 s1,
			 t1,
			 t2 );
		}
#endif /* defined( LIBBDE_SHA_UNFOLLED_LOOPS ) */

		if( memory_copy(
		     context->hash_values,
		     hash_values,
		     sizeof( uint32_t ) * 8 ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy hash values.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

#endif /* !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_SHA_H ) ) */

/* Initializes the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_initialize(
     libbde_sha256_context_t *context,
     liberror_error_t **error )
{
	static char *function = "libbde_sha256_initialize";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     context,
	     0,
	     sizeof( libbde_sha256_context_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear context.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	/* Request the AES crypt provider, fail back to the RSA crypt provider
	*/
 	if( CryptAcquireContext(
	     &( context->crypt_provider ),
	     NULL,
	     NULL,
	     PROV_RSA_AES,
	     CRYPT_VERIFYCONTEXT ) == 0 )
	{
		if( CryptAcquireContext(
		     &( context->crypt_provider ),
		     NULL,
		     NULL,
		     PROV_RSA_FULL,
		     CRYPT_VERIFYCONTEXT ) == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create AES or RSA crypt provider.",
			 function );

			return( -1 );
		}
	}
	if( context->crypt_provider == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to create crypt provider.",
		 function );

		return( -1 );
	}
	if( CryptCreateHash(
	     context->crypt_provider,
	     CALG_SHA256,
	     0,
	     0,
	     &( context->hash ) ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hash object.",
		 function );

		CryptReleaseContext(
		 context->crypt_provider,
		 0 );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestInit_ex(
	     context,
	     EVP_sha256(),
	     NULL ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize context.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	SHA256_Init( context );
#else
	if( memory_copy(
	     context->hash_values,
	     libbde_sha256_prime_square_roots,
	     sizeof( uint32_t ) * 8 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy initial hash values.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Updates the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_update(
     libbde_sha256_context_t *context,
     const uint8_t *buffer,
     size_t size,
     liberror_error_t **error )
{
	static char *function       = "libbde_sha256_update";

#if !defined( WINAPI ) && !defined( HAVE_LIBCRYPTO ) && !( defined( HAVE_OPENSSL_EVP_H ) || defined( HAVE_OPENSSL_SHA_H ) )
	size_t available_block_size = 0;
	size_t buffer_offset        = 0;
	size_t number_of_blocks     = 0;
	size_t remaining_size       = 0;
#endif

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
	if( CryptHashData(
	     context->hash,
	     (BYTE *) buffer,
	     (DWORD) size,
	     0 ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update hash.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( EVP_DigestUpdate(
	     context,
	     (const void *) buffer,
	     (unsigned long) size ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to update context.",
		 function );

		return( -1 );
	}

#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	SHA256_Update( context, buffer, size );

#else
	available_block_size = LIBBDE_SHA256_BLOCK_SIZE - context->block_offset;

	if( available_block_size > size )
	{
		available_block_size = size;
	}
	if( memory_copy(
	     &( context->block[ context->block_offset ] ),
	     buffer,
	     available_block_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data to context block.",
		 function );

		return( -1 );
	}
	if( ( context->block_offset + size ) < LIBBDE_SHA256_BLOCK_SIZE )
	{
		context->block_offset += size;
	}
	else
	{
		size -= available_block_size;

		number_of_blocks = size / LIBBDE_SHA256_BLOCK_SIZE;

		if( libbde_sha256_transform(
		     context,
		     context->block,
		     1,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform context block.",
			 function );

			return( -1 );
		}
		if( number_of_blocks > 0 )
		{
			buffer_offset = available_block_size;

			if( libbde_sha256_transform(
			     context,
			     &( buffer[ buffer_offset ] ),
			     number_of_blocks,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to transform buffer.",
				 function );

				return( -1 );
			}
			buffer_offset += number_of_blocks * LIBBDE_SHA256_BLOCK_SIZE;
		}
		remaining_size = size % LIBBDE_SHA256_BLOCK_SIZE;

		if( remaining_size > 0 )
		{
			if( memory_copy(
			     context->block,
			     &( buffer[ buffer_offset ] ),
			     remaining_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy remaining data to context block.",
				 function );

				return( -1 );
			}
		}
		context->block_offset = remaining_size;

		context->hash_count += ( number_of_blocks + 1 ) * LIBBDE_SHA256_BLOCK_SIZE;
	}
#endif
	return( 1 );
}

/* Finalizes the SHA256 context
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_finalize(
     libbde_sha256_context_t *context,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error )
{
	static char *function       = "libbde_sha256_finalize";

#if defined( WINAPI )
	DWORD safe_hash_size        = 0;
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	unsigned int safe_hash_size = 0;
#else
	size_t bit_size             = 0;
	size_t block_size           = 0;
	size_t hash_index           = 0;
	size_t number_of_blocks     = 0;

#if !defined( LIBBDE_SHA_UNFOLLED_LOOPS )
	int hash_values_index       = 0;
#endif
#endif

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	if( hash_size > (size_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid hash size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( context->hash == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid context - missing hash.",
		 function );

		return( -1 );
	}
	if( hash != NULL )
	{
		safe_hash_size = (DWORD) hash_size;

		if( CryptGetHashParam(
		     context->hash,
		     HP_HASHVAL,
		     (BYTE *) hash,
		     &safe_hash_size,
		     0 ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize hash.",
			 function );

			return( -1 );
		}
	}
	if( context->crypt_provider != 0 )
	{
		CryptReleaseContext(
		 context->crypt_provider,
		 0 );
	}
	if( context->hash != 0 )
	{
		CryptDestroyHash(
		 context->hash );
	}
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_EVP_H )
	if( hash != NULL )
	{
		if( hash_size > (size_t) UINT_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid hash size value exceeds maximum.",
			 function );

			return( -1 );
		}
		safe_hash_size = (unsigned int) hash_size;

		if( EVP_DigestFinal_ex(
		     context,
		     (unsigned char *) hash,
		     &safe_hash_size ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to finalize context.",
			 function );

			return( -1 );
		}
	}
	if( EVP_MD_CTX_cleanup(
	     context ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clean up context.",
		 function );
	}
#elif defined( HAVE_LIBCRYPTO ) && defined( HAVE_OPENSSL_SHA_H )
	if( hash != NULL )
	{
		if( hash_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid hash size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( hash_size < (size_t) LIBBDE_SHA256_HASH_SIZE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid hash size value too small.",
			 function );

			return( -1 );
		}
		SHA256_Final( hash, context );
	}
#else
	if( hash != NULL )
	{
		if( hash_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid hash size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( hash_size < (size_t) LIBBDE_SHA256_HASH_SIZE )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: invalid hash size value too small.",
			 function );

			return( -1 );
		}
		number_of_blocks = 1;

		if( ( context->block_offset % LIBBDE_SHA256_BLOCK_SIZE ) >= ( LIBBDE_SHA256_BLOCK_SIZE - 9 ) )
		{
			number_of_blocks += 1;
		}
		block_size = number_of_blocks * LIBBDE_SHA256_BLOCK_SIZE;

		if( memory_set(
		     &( context->block[ context->block_offset ] ),
		     0,
		     block_size - context->block_offset ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear context block.",
			 function );

			return( -1 );
		}
		bit_size = ( context->hash_count + context->block_offset ) * 8;

		byte_stream_copy_to_uint32_little_endian(
		 &( context->block[ block_size - 4 ] ),
		 bit_size );

		context->block[ context->block_offset ] = 0x80;

		if( libbde_sha256_transform(
		     context,
		     context->block,
		     number_of_blocks,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to transform context block.",
			 function );

			return( -1 );
		}
#if !defined( LIBBDE_SHA_UNFOLLED_LOOPS )
		for( hash_values_index = 0;
		     hash_values_index < 8;
		     hash_values_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( hash[ hash_index ] ),
			 context->hash_values[ hash_values_index ] );

			hash_index += sizeof( uint32_t );
		}
#else
		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 0 ] ),
		 context->hash_values[ 0 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 4 ] ),
		 context->hash_values[ 1 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 8 ] ),
		 context->hash_values[ 2 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 12 ] ),
		 context->hash_values[ 3 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 16 ] ),
		 context->hash_values[ 4 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 20 ] ),
		 context->hash_values[ 5 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 24 ] ),
		 context->hash_values[ 6 ] );

		byte_stream_copy_to_uint32_little_endian(
		 &( hash[ 28 ] ),
		 context->hash_values[ 7 ] );
	}
#endif /* !defined( LIBBDE_SHA_UNFOLLED_LOOPS ) */
}
#endif
	return( 1 );
}

/* Calculates the SHA256 digest hash of the data
 * Returns 1 if successful or -1 on error
 */
int libbde_sha256_calculate(
     const uint8_t *buffer,
     size_t size,
     uint8_t *hash,
     size_t hash_size,
     liberror_error_t **error )
{
	libbde_sha256_context_t context;

	static char *function = "libbde_sha256_initialize";

	if( libbde_sha256_initialize(
	     &context,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize context.",
		 function );

		return( -1 );
	}
	if( libbde_sha256_update(
	     &context,
	     buffer,
	     size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable update context.",
		 function );

		libbde_sha256_finalize(
		 &context,
		 NULL,
		 0,
		 NULL );

		return( -1 );
	}
	if( libbde_sha256_finalize(
	     &context,
	     hash,
	     hash_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable finalize context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

