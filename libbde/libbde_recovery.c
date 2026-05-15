/*
 * Recovery functions
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <types.h>

#include "libbde_libcaes.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfvalue.h"
#include "libbde_libhmac.h"
#include "libbde_password.h"
#include "libbde_password_keep.h"
#include "libbde_recovery.h"

/* Calculates the SHA256 hash of an UTF-8 formatted recovery password
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_utf8_recovery_password_calculate_hash(
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     libcerror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf8_string_t *split_string = NULL;
	uint8_t *string_segment                     = NULL;
	static char *function                       = "libbde_utf8_recovery_password_calculate_hash";
	size_t string_segment_index                 = 0;
	size_t string_segment_size                  = 0;
	uint64_t value_64bit                        = 0;
	int number_of_segments                      = 0;
	int result                                  = 0;
	int segment_index                           = 0;

	if( recovery_password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: recovery password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf8_string_split(
	     utf8_string,
	     utf8_string_length + 1,
	     (uint8_t) '-',
 	     &split_string,
 	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf8_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments in split string.",
		 function );

		goto on_error;
	}
	/* The recovery password consists of 8 segments
	 */
	if( number_of_segments == 8 )
	{
		for( segment_index = 0;
		     segment_index < number_of_segments;
		     segment_index++ )
		{
			if( libfvalue_split_utf8_string_get_segment_by_index(
			     split_string,
			     segment_index,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			string_segment_index = 0;

			if( libfvalue_utf8_string_with_index_copy_to_integer(
			     string_segment,
			     string_segment_size,
			     &string_segment_index,
			     &value_64bit,
			     16,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_64bit % 11 ) != 0 )
			{
				break;
			}
			value_64bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_64bit > (uint64_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_64bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf8_string_free(
	     &split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libcnotify_print_data(
			 binary_recovery_password,
			 16,
			 0 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear binary recovery password.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf8_string_free(
		 &split_string,
		 NULL );
	}
	memory_set(
	 binary_recovery_password,
	 0,
	 16 );

	return( -1 );
}

/* Calculates the SHA256 hash of an UTF-16 formatted recovery password
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libbde_utf16_recovery_password_calculate_hash(
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint8_t *recovery_password_hash,
     size_t recovery_password_hash_size,
     libcerror_error_t **error )
{
	uint8_t binary_recovery_password[ 16 ];

	libfvalue_split_utf16_string_t *split_string = NULL;
	uint16_t *string_segment                     = NULL;
	static char *function                        = "libbde_utf16_recovery_password_calculate_hash";
	size_t string_segment_index                  = 0;
	size_t string_segment_size                   = 0;
	uint64_t value_64bit                         = 0;
	int number_of_segments                       = 0;
	int result                                   = 0;
	int segment_index                            = 0;

	if( recovery_password_hash == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password hash.",
		 function );

		return( -1 );
	}
	if( recovery_password_hash_size != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: recovery password hash size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfvalue_utf16_string_split(
	     utf16_string,
	     utf16_string_length + 1,
	     (uint16_t) '-',
 	     &split_string,
 	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to split string.",
		 function );

		goto on_error;
	}
	if( libfvalue_split_utf16_string_get_number_of_segments(
	     split_string,
	     &number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments in split string.",
		 function );

		goto on_error;
	}
	/* The recovery password consists of 8 segments
	 */
	if( number_of_segments == 8 )
	{
		for( segment_index = 0;
		     segment_index < number_of_segments;
		     segment_index++ )
		{
			if( libfvalue_split_utf16_string_get_segment_by_index(
			     split_string,
			     segment_index,
			     &string_segment,
			     &string_segment_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			string_segment_index = 0;

			if( libfvalue_utf16_string_with_index_copy_to_integer(
			     string_segment,
			     string_segment_size,
			     &string_segment_index,
			     &value_64bit,
			     16,
			     LIBFVALUE_INTEGER_FORMAT_TYPE_DECIMAL_UNSIGNED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine binary value for segment: %d from split string.",
				 function,
				 segment_index );

				goto on_error;
			}
			/* A recovery password segment should be dividable by 11
			 */
			if( ( value_64bit % 11 ) != 0 )
			{
				break;
			}
			value_64bit /= 11;

			/* A recovery password segment / 11 should be <= 65535 (0xffff)
			 */
			if( value_64bit > (uint64_t) UINT16_MAX )
			{
				break;
			}
			byte_stream_copy_from_uint16_little_endian(
			 &( binary_recovery_password[ segment_index * 2 ] ),
			 value_64bit );
		}
		result = 1;
	}
	if( libfvalue_split_utf16_string_free(
	     &split_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free split string.",
		 function );

		goto on_error;
	}
	if( result == 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: binary recovery password:\n",
			 function );
			libcnotify_print_data(
			 binary_recovery_password,
			 16,
			 0 );
		}
#endif
		if( libhmac_sha256_calculate(
		     binary_recovery_password,
		     16,
		     recovery_password_hash,
		     recovery_password_hash_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate recovery password hash.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     binary_recovery_password,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear binary recovery password.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( split_string != NULL )
	{
		libfvalue_split_utf16_string_free(
		 &split_string,
		 NULL );
	}
	memory_set(
	 binary_recovery_password,
	 0,
	 16 );

	return( -1 );
}

/* Helper: try to decrypt a sub-entry from the RP VMK stretch_key->data using
 * the given 32-byte AES key and nonce.  Tries both without and with a 16-byte
 * dummy prefix (to skip counter=0 / A0 in libcaes_crypt_ccm).
 *
 * sub_data      : pointer to the first byte of sub-entry data AFTER the 8-byte header
 *                 (i.e. at offset +8 within the sub-entry: 12-byte nonce then ciphertext)
 * ct_size       : number of ciphertext bytes (not counting the nonce header)
 * aes_key       : 32-byte AES-256 key
 * label         : short string for diagnostics (e.g. "E1" = sub-entry 1)
 * found_rp_out  : if a valid RP is found, its 16 binary bytes are written here
 * Returns 1 if a valid binary RP was found, 0 otherwise.
 */
static int rp_try_decrypt(
     libcaes_context_t *aes_context,
     const uint8_t *sub_data,
     size_t ct_size,
     const char *label,
     uint8_t *found_rp_out,
     libcerror_error_t **error )
{
	/* Max buffer: 16 dummy + up to 60 real = 76 bytes */
	uint8_t ciphertext[ 76 ];
	uint8_t plaintext[ 76 ];
	uint8_t nonce[ 12 ];
	int pass;
	int segment_index;
	int is_valid;
	size_t try_sizes[ 2 ];
	size_t rp_offsets[ 4 ];
	size_t n_offsets;
	size_t k;
	uint16_t binary_rp_word;
	uint32_t segment_value;
	int result;
	size_t prefix;
	size_t total;

	if( ct_size > 60 || ct_size < 16 )
	{
		return( 0 );
	}

	/* nonce is first 12 bytes of sub_data */
	if( memory_copy( nonce, sub_data, 12 ) == NULL ) return( 0 );

	/* ciphertext starts at sub_data+12 */
	/* pass 0: no dummy prefix — real data at counter=0 */
	/* pass 1: 16-byte dummy prefix — real data at counter=1 (A1) */
	for( pass = 0; pass <= 1; pass++ )
	{
		prefix = (size_t) pass * 16;
		total  = prefix + ct_size;

		if( total > 76 ) continue;

		if( memory_set( ciphertext, 0, total ) == NULL ) return( 0 );
		if( memory_set( plaintext,  0, total ) == NULL ) return( 0 );

		if( memory_copy( &( ciphertext[ prefix ] ), &( sub_data[ 12 ] ), ct_size ) == NULL )
			return( 0 );

		result = libcaes_crypt_ccm(
		          aes_context,
		          LIBCAES_CRYPT_MODE_DECRYPT,
		          nonce,
		          12,
		          ciphertext,
		          total,
		          plaintext,
		          total,
		          error );

		if( result != 1 )
		{
			libcerror_error_free( error );
			continue;
		}

		/* The binary RP sits at a fixed offset within the plaintext.
		 * Structure (for pass=0, ct_size=44):
		 *   bytes  0..15 : GUID / random (16 bytes)
		 *   bytes 16..27 : structure header (12 bytes)
		 *                    [0..3]  = size LE (e.g. 2C 00 00 00)
		 *                    [4..7]  = 01 00 00 00
		 *                    [8..11] = ?? 10|20 00 00
		 *   bytes 28..43 : binary RP (16 bytes)  <-- our target
		 *
		 * Try offset 28 first (definitive), then fallback to other offsets.
		 */
		n_offsets = 0;
		rp_offsets[ n_offsets++ ] = 28;            /* primary: known-good structural offset */
		rp_offsets[ n_offsets++ ] = prefix + 8;   /* fallback: header after dummy */
		rp_offsets[ n_offsets++ ] = prefix;        /* fallback: no header */

		for( k = 0; k < n_offsets; k++ )
		{
			size_t rp_offset = rp_offsets[ k ];

			if( rp_offset + 16 > total ) continue;

			/* Structural header validation: the 12 bytes immediately preceding
			 * the binary RP should match the pattern:
			 *   [offset-12]..[offset-9]  : any size LE (non-zero)
			 *   [offset-8]..[offset-5]   : 01 00 00 00
			 *   [offset-4]..[offset-1]   : ?? 10|20 00 00  (word3[2..3] == 00 00)
			 * Only apply when there are 12 bytes before rp_offset.
			 */
			if( rp_offset >= 12 )
			{
				/* bytes [offset-8]..[offset-5] must be 01 00 00 00 */
				if( plaintext[ rp_offset - 8 ] != 0x01 ||
				    plaintext[ rp_offset - 7 ] != 0x00 ||
				    plaintext[ rp_offset - 6 ] != 0x00 ||
				    plaintext[ rp_offset - 5 ] != 0x00 )
				{
					continue;
				}
			/* bytes [offset-2]..[offset-1] must be 00 00
			 * (the type/size word is ?? 10|20 00 00; only the last two bytes are reliably zero) */
			if( plaintext[ rp_offset - 2 ] != 0x00 ||
			    plaintext[ rp_offset - 1 ] != 0x00 )
			{
				continue;
			}
			}

			is_valid = 1;
			for( segment_index = 0; segment_index < 8; segment_index++ )
			{
				byte_stream_copy_to_uint16_little_endian(
				 &( plaintext[ rp_offset + (size_t) segment_index * 2 ] ),
				 binary_rp_word );

				segment_value = (uint32_t) binary_rp_word * 11;

				/* binary_rp_word = group/11, so segment_value = group.
				 * A valid 6-digit group is 000000..720720 (max uint16*11=720720).
				 * There is NO requirement that binary_rp_word itself % 11 == 0. */
				if( segment_value > 720720 )
				{
					is_valid = 0;
					break;
				}
			}

			if( is_valid )
			{
				if( memory_copy( found_rp_out, &( plaintext[ rp_offset ] ), 16 ) == NULL )
					return( 0 );
				return( 1 );
			}
		}
	}

	return( 0 );
}

/* Recovers the recovery password from metadata given the disk VMK (already
 * decrypted via the passphrase).  Attempts to decrypt the mystery sub-entries
 * (type 0x0012, 0x0013) inside the RP VMK's stretch_key->data using the disk
 * VMK directly as the 256-bit AES key, looking for the 16-byte binary RP.
 *
 * Sub-entry layout within rp_vmk->stretch_key->data:
 *   Sub-entry 1 (bytes 0..63,  size=0x40): header(8) + nonce(12) + ciphertext(44)
 *   Sub-entry 2 (bytes 64..143, size=0x50): header(8) + nonce(12) + ciphertext(60)
 *
 * Returns 1 if successful, 0 if not applicable or not found, -1 on error.
 */
int libbde_recovery_password_from_vmk(
     libbde_metadata_t *metadata,
     libbde_password_keep_t *password_keep,
     const uint8_t *volume_master_key,
     uint8_t *recovery_password,
     size_t recovery_password_size,
     libcerror_error_t **error )
{
	static char *function          = "libbde_recovery_password_from_vmk";
	libcaes_context_t *aes_context = NULL;
	libbde_stretch_key_t *sk       = NULL;
	uint8_t binary_rp[ 16 ];
	uint16_t binary_rp_word        = 0;
	uint32_t seg                   = 0;
	uint8_t *out_ptr               = NULL;
	size_t remaining               = 0;
	int segment_index              = 0;
	int found                      = 0;
	char seg_buf[ 7 ];

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid recovery password.",
		 function );

		return( -1 );
	}
	if( recovery_password_size < 56 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid recovery password value too small.",
		 function );

		return( -1 );
	}

	if( volume_master_key == NULL )
	{
		return( 0 );
	}
	if( metadata->recovery_password_volume_master_key == NULL )
	{
		return( 0 );
	}

	sk = metadata->recovery_password_volume_master_key->stretch_key;

	if( sk == NULL )
	{
		return( 0 );
	}
	/* Sub-entry 1: bytes 0..63 (need 64); sub-entry 2: bytes 64..143 (need 144) */
	if( sk->data == NULL || sk->data_size < 64 )
	{
		return( 0 );
	}

	/* Set up AES context keyed with the disk VMK (32 bytes = AES-256) */
	if( libcaes_context_initialize( &aes_context, error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_set_key(
	     aes_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     volume_master_key,
	     256,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set AES key from disk VMK.",
		 function );

		goto on_error;
	}

	if( memory_set( binary_rp, 0, 16 ) == NULL )
	{
		goto on_error;
	}

	/* --- Try sub-entry 1 (bytes 0..63: header 0..7, nonce 8..19, ct 20..63, ct_size=44) --- */
	/* Pass sub_data pointing at byte 8 (nonce start); ct_size = 44 */
	found = rp_try_decrypt( aes_context, &( sk->data[ 8 ] ), 44, "E1", binary_rp, error );

	/* --- Try sub-entry 2 (bytes 64..143: header 64..71, nonce 72..83, ct 84..143, ct_size=60) --- */
	if( found == 0 && sk->data_size >= 144 )
	{
		found = rp_try_decrypt( aes_context, &( sk->data[ 72 ] ), 60, "E2", binary_rp, error );
	}

	if( libcaes_context_free( &aes_context, error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free AES context.",
		 function );

		goto on_error;
	}
	aes_context = NULL;

	if( found == 0 )
	{
		return( 0 );
	}

	/* Format the recovery password: 8 six-digit zero-padded decimal groups joined by '-' */
	/* Total: 8*6 + 7*1 = 55 chars + NUL = 56 bytes minimum */
	out_ptr   = recovery_password;
	remaining = recovery_password_size;

	for( segment_index = 0; segment_index < 8; segment_index++ )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( binary_rp[ (size_t) segment_index * 2 ] ),
		 binary_rp_word );

		seg = (uint32_t) binary_rp_word * 11;

		/* Format as 6-digit zero-padded decimal */
		seg_buf[ 0 ] = (char)( '0' + ( seg / 100000 ) % 10 );
		seg_buf[ 1 ] = (char)( '0' + ( seg / 10000  ) % 10 );
		seg_buf[ 2 ] = (char)( '0' + ( seg / 1000   ) % 10 );
		seg_buf[ 3 ] = (char)( '0' + ( seg / 100    ) % 10 );
		seg_buf[ 4 ] = (char)( '0' + ( seg / 10     ) % 10 );
		seg_buf[ 5 ] = (char)( '0' + ( seg           ) % 10 );
		seg_buf[ 6 ] = '\0';

		if( remaining < 6 )
		{
			return( 0 );
		}
		if( memory_copy( out_ptr, seg_buf, 6 ) == NULL )
		{
			return( 0 );
		}
		out_ptr   += 6;
		remaining -= 6;

		if( segment_index < 7 )
		{
			if( remaining < 1 )
			{
				return( 0 );
			}
			*out_ptr   = (uint8_t) '-';
			out_ptr   += 1;
			remaining -= 1;
		}
	}

	/* NUL-terminate */
	if( remaining >= 1 )
	{
		*out_ptr = 0;
	}

	return( 1 );

on_error:
	if( aes_context != NULL )
	{
		libcaes_context_free( &aes_context, NULL );
	}
	return( -1 );
}
