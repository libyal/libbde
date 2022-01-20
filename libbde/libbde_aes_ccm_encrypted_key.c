/*
 * AES-CCM encrypted key metadata entry functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <system_string.h>
#include <types.h>

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_metadata_entry.h"

#include "bde_metadata.h"

/* Creates an AES-CCM encrypted key
 * Make sure the value AES-CCM encrypted key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_encrypted_key_initialize(
     libbde_aes_ccm_encrypted_key_t **aes_ccm_encrypted_key,
     libcerror_error_t **error )
{
	static char *function = "libbde_aes_ccm_encrypted_key_initialize";

	if( aes_ccm_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	if( *aes_ccm_encrypted_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid AES-CCM encrypted key value already set.",
		 function );

		return( -1 );
	}
	*aes_ccm_encrypted_key = memory_allocate_structure(
	                          libbde_aes_ccm_encrypted_key_t );

	if( *aes_ccm_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *aes_ccm_encrypted_key,
	     0,
	     sizeof( libbde_aes_ccm_encrypted_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *aes_ccm_encrypted_key != NULL )
	{
		memory_free(
		 *aes_ccm_encrypted_key );

		*aes_ccm_encrypted_key = NULL;
	}
	return( -1 );
}

/* Frees an AES-CCM encrypted key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_encrypted_key_free(
     libbde_aes_ccm_encrypted_key_t **aes_ccm_encrypted_key,
     libcerror_error_t **error )
{
	static char *function = "libbde_aes_ccm_encrypted_key_free";

	if( aes_ccm_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	if( *aes_ccm_encrypted_key != NULL )
	{
		if( ( *aes_ccm_encrypted_key )->data != NULL )
		{
			memory_free(
			 ( *aes_ccm_encrypted_key )->data );
		}
		memory_free(
		 *aes_ccm_encrypted_key );

		*aes_ccm_encrypted_key = NULL;
	}
	return( 1 );
}

/* Reads a AES-CCM encrypted key from the metadata entry
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_encrypted_key_read(
     libbde_aes_ccm_encrypted_key_t *aes_ccm_encrypted_key,
     libbde_metadata_entry_t *metadata_entry,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libbde_aes_ccm_encrypted_key_read";
	size_t value_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit   = 0;
#endif

	if( aes_ccm_encrypted_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	if( metadata_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata entry.",
		 function );

		return( -1 );
	}
	if( metadata_entry->value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid metadata entry - missing value data.",
		 function );

		return( -1 );
	}
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_AES_CCM_ENCRYPTED_KEY )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid metadata entry - unsupported value type: 0x%04" PRIx16 ".",
		 function,
		 metadata_entry->value_type );

		return( -1 );
	}
	value_data      = metadata_entry->value_data;
	value_data_size = metadata_entry->value_data_size;

	if( ( value_data_size < sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t ) )
	 || ( value_data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid metadata entry - value data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libbde_debug_print_filetime_value(
		     function,
		     "nonce time\t\t\t\t",
		     ( (bde_metadata_entry_aes_ccm_encrypted_key_header_t *) value_data )->nonce_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_entry_aes_ccm_encrypted_key_header_t *) value_data )->nonce_counter,
		 value_32bit );
		libcnotify_printf(
		 "%s: nonce counter\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
#endif
	if( memory_copy(
	     aes_ccm_encrypted_key->nonce,
	     value_data,
	     12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy nonce to AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	value_data      += sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: encrypted data:\n",
		 function );
		libcnotify_print_data(
		 value_data,
		 value_data_size,
		 0 );
	}
#endif
	aes_ccm_encrypted_key->data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * value_data_size );

	if( aes_ccm_encrypted_key->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     aes_ccm_encrypted_key->data,
	     value_data,
	     value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data to AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	aes_ccm_encrypted_key->data_size = value_data_size;
	
	return( 1 );

on_error:
	if( aes_ccm_encrypted_key->data != NULL )
	{
		memory_free(
		 aes_ccm_encrypted_key->data );

		aes_ccm_encrypted_key->data = NULL;
	}
	return( -1 );
}

