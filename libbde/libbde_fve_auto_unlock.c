/*
 * FVEAutoUnlock blob functions
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
#include <types.h>

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_definitions.h"
#include "libbde_fve_auto_unlock.h"
#include "libbde_libcaes.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_metadata_entry.h"

#include "bde_metadata.h"

/* Reads a FVEAutoUnlock registry blob and recovers the plaintext external key
 * of the secondary (auto-unlocked) volume.
 *
 * The blob layout (as documented in libbde issue #37) is approximately:
 *
 *   uint32_t length;          // total size, e.g. 0x70
 *   uint32_t unknown;         // 0x9
 *   uint8_t  vmk_identifier[16]; // GUID of the VMK slot in the secondary volume
 *   (optional) uint8_t filetime[8]; // not consistently present
 *   FVE metadata entry:       // a standard metadata entry that holds an
 *                             // AES-CCM encrypted key (value type 0x0005)
 *     uint16_t size;
 *     uint16_t type;          // 0
 *     uint16_t value_type;    // 5 (AES-CCM encrypted key)
 *     uint16_t version;       // 1
 *     uint8_t  nonce[12];
 *     uint8_t  mac[16];
 *     uint8_t  ciphertext[...];
 *
 * Because the presence of the optional FILETIME (and therefore the exact offset
 * of the inner metadata entry) is not fully pinned down, this function scans the
 * blob for the inner AES-CCM encrypted key entry instead of assuming a fixed
 * offset. The entry is identified by value_type == 0x0005, version == 1 and a
 * plausible size.
 *
 * Returns 1 if successful, 0 if the external key could not be recovered or -1 on error
 */
int libbde_fve_auto_unlock_read_blob(
     const uint8_t *blob_data,
     size_t blob_data_size,
     const uint8_t *auto_unlock_key,
     size_t auto_unlock_key_size,
     uint8_t *external_key,
     size_t external_key_size,
     size_t *external_key_data_size,
     uint8_t *vmk_identifier,
     size_t vmk_identifier_size,
     libcerror_error_t **error )
{
	libbde_aes_ccm_encrypted_key_t *aes_ccm_encrypted_key = NULL;
	libbde_metadata_entry_t *metadata_entry               = NULL;
	libcaes_context_t *aes_context                        = NULL;
	uint8_t *unencrypted_data                             = NULL;
	static char *function                                 = "libbde_fve_auto_unlock_read_blob";
	size_t entry_offset                                   = 0;
	size_t scan_offset                                    = 0;
	size_t unencrypted_data_size                          = 0;
	ssize_t read_count                                    = 0;
	uint32_t blob_length                                  = 0;
	uint32_t data_size                                    = 0;
	uint32_t version                                      = 0;
	uint16_t entry_size                                   = 0;
	uint16_t entry_value_type                             = 0;
	uint16_t entry_version                                = 0;
	int result                                            = 0;

	if( blob_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid blob data.",
		 function );

		return( -1 );
	}
	if( ( blob_data_size < ( 8 + (size_t) sizeof( bde_metadata_entry_v1_t ) ) )
	 || ( blob_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid blob data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( auto_unlock_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid auto-unlock key.",
		 function );

		return( -1 );
	}
	if( auto_unlock_key_size < 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid auto-unlock key value too small.",
		 function );

		return( -1 );
	}
	if( external_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid external key.",
		 function );

		return( -1 );
	}
	if( ( external_key_size < 32 )
	 || ( external_key_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid external key size value out of bounds.",
		 function );

		return( -1 );
	}
	if( external_key_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid external key data size.",
		 function );

		return( -1 );
	}
	if( ( vmk_identifier != NULL )
	 && ( vmk_identifier_size < 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid VMK identifier value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( blob_data[ 0 ] ),
	 blob_length );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: FVEAutoUnlock blob:\n",
		 function );
		libcnotify_print_data(
		 blob_data,
		 blob_data_size,
		 0 );

		libcnotify_printf(
		 "%s: blob length\t\t\t: %" PRIu32 "\n",
		 function,
		 blob_length );

		libcnotify_printf(
		 "\n" );
	}
#endif
	/* The VMK identifier (GUID) directly follows the two 32-bit header values.
	 */
	if( vmk_identifier != NULL )
	{
		if( memory_copy(
		     vmk_identifier,
		     &( blob_data[ 8 ] ),
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy VMK identifier.",
			 function );

			goto on_error;
		}
	}
	/* Scan for the inner AES-CCM encrypted key metadata entry. It starts right
	 * after the VMK identifier (offset 24), but an optional 8-byte FILETIME may
	 * be present, so we probe candidate offsets and validate the entry header.
	 */
	entry_offset = 0;

	for( scan_offset = 24;
	     ( scan_offset + (size_t) sizeof( bde_metadata_entry_v1_t ) ) <= blob_data_size;
	     scan_offset += 1 )
	{
		byte_stream_copy_to_uint16_little_endian(
		 &( blob_data[ scan_offset ] ),
		 entry_size );

		byte_stream_copy_to_uint16_little_endian(
		 &( blob_data[ scan_offset + 4 ] ),
		 entry_value_type );

		byte_stream_copy_to_uint16_little_endian(
		 &( blob_data[ scan_offset + 6 ] ),
		 entry_version );

		if( ( entry_value_type == LIBBDE_VALUE_TYPE_AES_CCM_ENCRYPTED_KEY )
		 && ( entry_version == 1 )
		 && ( (size_t) entry_size >= ( sizeof( bde_metadata_entry_v1_t ) + 28 ) )
		 && ( (size_t) entry_size <= ( blob_data_size - scan_offset ) ) )
		{
			entry_offset = scan_offset;

			break;
		}
	}
	if( entry_offset == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to locate AES-CCM encrypted key entry in FVEAutoUnlock blob.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: located AES-CCM encrypted key entry at blob offset: %" PRIzd ".\n",
		 function,
		 (ssize_t) entry_offset );
	}
#endif
	if( libbde_metadata_entry_initialize(
	     &metadata_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata entry.",
		 function );

		goto on_error;
	}
	read_count = libbde_metadata_entry_read(
	              metadata_entry,
	              &( blob_data[ entry_offset ] ),
	              blob_data_size - entry_offset,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read AES-CCM encrypted key metadata entry.",
		 function );

		goto on_error;
	}
	if( libbde_aes_ccm_encrypted_key_initialize(
	     &aes_ccm_encrypted_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( libbde_aes_ccm_encrypted_key_read(
	     aes_ccm_encrypted_key,
	     metadata_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( ( aes_ccm_encrypted_key->data_size < 28 )
	 || ( aes_ccm_encrypted_key->data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid AES-CCM encrypted key data size value out of bounds.",
		 function );

		goto on_error;
	}
	unencrypted_data_size = aes_ccm_encrypted_key->data_size;

	unencrypted_data = (uint8_t *) memory_allocate(
	                                unencrypted_data_size );

	if( unencrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create unencrypted data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     unencrypted_data,
	     0,
	     unencrypted_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear unencrypted data.",
		 function );

		goto on_error;
	}
	if( libcaes_context_initialize(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable initialize AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_context_set_key(
	     aes_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     auto_unlock_key,
	     256,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set encryption key in AES context.",
		 function );

		goto on_error;
	}
	if( libcaes_crypt_ccm(
	     aes_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     aes_ccm_encrypted_key->nonce,
	     12,
	     aes_ccm_encrypted_key->data,
	     aes_ccm_encrypted_key->data_size,
	     unencrypted_data,
	     unencrypted_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
		 "%s: unable to decrypt data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unencrypted data:\n",
		 function );
		libcnotify_print_data(
		 unencrypted_data,
		 unencrypted_data_size,
		 0 );
	}
#endif
	/* The first 16 bytes are the (unvalidated, see libbde issue #36) AES-CCM MAC
	 * region; the key container starts at offset 16.
	 */
	byte_stream_copy_to_uint16_little_endian(
	 &( unencrypted_data[ 16 ] ),
	 data_size );

	byte_stream_copy_to_uint16_little_endian(
	 &( unencrypted_data[ 20 ] ),
	 version );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key container data size\t\t: %" PRIu32 "\n",
		 function,
		 data_size );

		libcnotify_printf(
		 "%s: key container version\t\t: %" PRIu32 "\n",
		 function,
		 version );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( version == 1 )
	{
		/* The external key is stored as a 256-bit key. The key container size
		 * field (0x2c) covers the 0x2c bytes that follow the size field itself.
		 */
		if( ( data_size == 0x2c )
		 && ( unencrypted_data_size >= ( 28 + 32 ) ) )
		{
			if( memory_copy(
			     external_key,
			     &( unencrypted_data[ 28 ] ),
			     32 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy unencrypted external key.",
				 function );

				goto on_error;
			}
			*external_key_data_size = 32;

			result = 1;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( ( result == 0 )
	 && ( libcnotify_verbose != 0 ) )
	{
		libcnotify_printf(
		 "%s: external key could not be decrypted "
		 "(unexpected key container version: %" PRIu32 " or data size: %" PRIu32 "); "
		 "wrong auto-unlock key or blob?\n",
		 function,
		 version,
		 data_size );
	}
#endif
	if( libcaes_context_free(
	     &aes_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable free context.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     unencrypted_data,
	     0,
	     unencrypted_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear unencrypted data.",
		 function );

		goto on_error;
	}
	memory_free(
	 unencrypted_data );

	unencrypted_data = NULL;

	if( libbde_aes_ccm_encrypted_key_free(
	     &aes_ccm_encrypted_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( libbde_metadata_entry_free(
	     &metadata_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata entry.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( unencrypted_data != NULL )
	{
		memory_set(
		 unencrypted_data,
		 0,
		 unencrypted_data_size );
		memory_free(
		 unencrypted_data );
	}
	if( aes_context != NULL )
	{
		libcaes_context_free(
		 &aes_context,
		 NULL );
	}
	if( aes_ccm_encrypted_key != NULL )
	{
		libbde_aes_ccm_encrypted_key_free(
		 &aes_ccm_encrypted_key,
		 NULL );
	}
	if( metadata_entry != NULL )
	{
		libbde_metadata_entry_free(
		 &metadata_entry,
		 NULL );
	}
	return( -1 );
}
