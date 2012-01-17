/*
 * AES-CCM encrypted key metadata entry functions
 *
 * Copyright (C) 2011-2012, Google Inc.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_definitions.h"
#include "libbde_libfdatetime.h"
#include "libbde_metadata_entry.h"

#include "bde_metadata.h"

/* Initialize a AES-CCM encrypted key
 * Make sure the value AES-CCM encrypted key is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_encrypted_key_initialize(
     libbde_aes_ccm_encrypted_key_t **aes_ccm_encrypted_key,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_ccm_encrypted_key_initialize";

	if( aes_ccm_encrypted_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	if( *aes_ccm_encrypted_key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid AES-CCM encrypted key value already set.",
		 function );

		return( -1 );
	}
	*aes_ccm_encrypted_key = memory_allocate_structure(
	                          libbde_aes_ccm_encrypted_key_t );

	if( *aes_ccm_encrypted_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *aes_ccm_encrypted_key,
	     0,
	     sizeof( libbde_aes_ccm_encrypted_key_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
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

/* Frees a AES-CCM encrypted key
 * Returns 1 if successful or -1 on error
 */
int libbde_aes_ccm_encrypted_key_free(
     libbde_aes_ccm_encrypted_key_t **aes_ccm_encrypted_key,
     liberror_error_t **error )
{
	static char *function = "libbde_aes_ccm_encrypted_key_free";

	if( aes_ccm_encrypted_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     liberror_error_t **error )
{
	uint8_t *value_data               = NULL;
	static char *function             = "libbde_aes_ccm_encrypted_key_read";
	size_t value_data_size            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	uint32_t value_32bit              = 0;
	int result                        = 0;
#endif

	if( aes_ccm_encrypted_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	if( metadata_entry == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata entry.",
		 function );

		return( -1 );
	}
	if( metadata_entry->value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid metadata entry - missing value data.",
		 function );

		return( -1 );
	}
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_AES_CCM_ENCRYPTED_KEY )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid metadata entry - unsupported value type: 0x%04" PRIx16 ".",
		 function,
		 metadata_entry->value_type );

		return( -1 );
	}
	value_data      = metadata_entry->value_data;
	value_data_size = metadata_entry->value_data_size;

	if( value_data_size < sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: value data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create filetime.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (bde_metadata_entry_aes_ccm_encrypted_key_header_t *) value_data )->nonce_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MICRO_SECONDS,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MICRO_SECONDS,
			  LIBFDATETIME_DATE_TIME_FORMAT_CTIME,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy filetime to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: nonce time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free filetime.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (bde_metadata_entry_aes_ccm_encrypted_key_header_t *) value_data )->nonce_counter,
		 value_32bit );
		libnotify_printf(
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy nonce to AES-CCM encrypted key.",
		 function );

		return( -1 );
	}
	value_data      += sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_aes_ccm_encrypted_key_header_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: encrypted data:\n",
		 function );
		libnotify_print_data(
		 value_data,
		 value_data_size );
	}
#endif
	aes_ccm_encrypted_key->data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * value_data_size );

	if( aes_ccm_encrypted_key->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     aes_ccm_encrypted_key->data,
	     value_data,
	     value_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data to AES-CCM encrypted key.",
		 function );

		goto on_error;
	}
	aes_ccm_encrypted_key->data_size = value_data_size;
	
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	if( aes_ccm_encrypted_key->data != NULL )
	{
		memory_free(
		 aes_ccm_encrypted_key->data );

		aes_ccm_encrypted_key->data = NULL;
	}
	return( -1 );
}

