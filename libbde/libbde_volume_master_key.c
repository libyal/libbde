/*
 * Volume Master Key (VMK) metadata entry functions
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
#include "libbde_array_type.h"
#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_io_handle.h"
#include "libbde_key.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_metadata_entry.h"
#include "libbde_stretch_key.h"
#include "libbde_volume_master_key.h"

#include "bde_metadata.h"

/* Initialize a volume master key
 * Make sure the value volume master key is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_initialize(
     libbde_volume_master_key_t **volume_master_key,
     liberror_error_t **error )
{
	static char *function = "libbde_volume_master_key_initialize";

	if( volume_master_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( *volume_master_key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume master key value already set.",
		 function );

		return( -1 );
	}
	*volume_master_key = memory_allocate_structure(
	                      libbde_volume_master_key_t );

	if( *volume_master_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume master key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_master_key,
	     0,
	     sizeof( libbde_volume_master_key_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume master key.",
		 function );

		memory_free(
		 *volume_master_key );

		*volume_master_key = NULL;

		return( -1 );
	}
	if( libbde_array_initialize(
	     &( ( *volume_master_key )->entries_array ),
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_master_key != NULL )
	{
		memory_free(
		 *volume_master_key );

		*volume_master_key = NULL;
	}
	return( -1 );
}

/* Frees a volume master key
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_free(
     libbde_volume_master_key_t **volume_master_key,
     liberror_error_t **error )
{
	static char *function = "libbde_volume_master_key_free";
	int result            = 1;

	if( volume_master_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( *volume_master_key != NULL )
	{
		if( ( *volume_master_key )->key != NULL )
		{
			if( libbde_key_free(
			     &( ( *volume_master_key )->key ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key.",
				 function );

				result = -1;
			}
		}
		if( ( *volume_master_key )->stretch_key != NULL )
		{
			if( libbde_stretch_key_free(
			     &( ( *volume_master_key )->stretch_key ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free stretch key.",
				 function );

				result = -1;
			}
		}
		if( ( *volume_master_key )->aes_ccm_encrypted_key != NULL )
		{
			if( libbde_aes_ccm_encrypted_key_free(
			     &( ( *volume_master_key )->aes_ccm_encrypted_key ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES-CCM encrypted key.",
				 function );

				result = -1;
			}
		}
		if( libbde_array_free(
		     &( ( *volume_master_key )->entries_array ),
		     (int(*)(intptr_t **, liberror_error_t **)) &libbde_metadata_entry_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *volume_master_key );

		*volume_master_key = NULL;
	}
	return( result );
}

/* Reads a volume master key from the metadata entry
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_read(
     libbde_volume_master_key_t *volume_master_key,
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error )
{
	libbde_aes_ccm_encrypted_key_t *aes_ccm_encrypted_key = NULL;
	libbde_key_t *key                                     = NULL;
	libbde_metadata_entry_t *property_metadata_entry      = NULL;
	libbde_stretch_key_t *stretch_key                     = NULL;
	uint8_t *value_data                                   = NULL;
	static char *function                                 = "libbde_volume_master_key_read";
	size_t value_data_size                                = 0;
	ssize_t read_count                                    = 0;
	int property_metadata_entry_index                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t filetime_string[ 32 ];
	libcstring_system_character_t guid_string[ LIBFGUID_IDENTIFIER_STRING_SIZE ];

	libfdatetime_filetime_t *filetime                     = NULL;
	libfguid_identifier_t *guid                           = NULL;
	uint16_t value_16bit                                  = 0;
	int result                                            = 0;
#endif

	if( volume_master_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
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
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_VOLUME_MASTER_KEY )
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

	if( value_data_size < sizeof( bde_metadata_entry_volume_master_key_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: value data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_master_key->identifier,
	     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->identifier,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->protection_type,
	 volume_master_key->protection_type );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( libfguid_identifier_initialize(
		     &guid,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create GUID.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  LIBFGUID_IDENTIFIER_STRING_SIZE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  LIBFGUID_IDENTIFIER_STRING_SIZE,
			  error );
#endif
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		libnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 function,
		 guid_string );

		if( libfguid_identifier_free(
		     &guid,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free GUID.",
			 function );

			goto on_error;
		}

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
		     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->modification_time,
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
		 "%s: modification time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
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
		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->unknown1,
		 value_16bit );
		libnotify_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: protection type\t\t\t\t: 0x%04" PRIx16 " (%s)\n",
		 function,
		 volume_master_key->protection_type,
		 libbde_debug_print_key_protection_type(
		  volume_master_key->protection_type ) );

		libnotify_printf(
		 "\n" );
	}
#endif
	value_data      += sizeof( bde_metadata_entry_volume_master_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_volume_master_key_header_t );

	while( value_data_size >= sizeof( bde_metadata_entry_v1_t ) )
	{
		if( libbde_metadata_entry_initialize(
		     &property_metadata_entry,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create property metadata entry.",
			 function );

			goto on_error;
		}
		read_count = libbde_metadata_entry_read(
			      property_metadata_entry,
			      value_data,
			      value_data_size,
			      error );

		if( read_count == -1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read property metadata entry.",
			 function );

			goto on_error;
		}
		value_data      += read_count;
		value_data_size -= read_count;

		if( property_metadata_entry->value_type == LIBBDE_VALUE_TYPE_KEY )
		{
			if( libbde_key_initialize(
			     &key,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create key.",
				 function );

				goto on_error;
			}
			if( libbde_key_read(
			     key,
			     property_metadata_entry,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read key metadata entry.",
				 function );

				goto on_error;
			}
			if( volume_master_key->key == NULL )
			{
				volume_master_key->key = key;

				key = NULL;
			}
			if( key != NULL )
			{
				if( libbde_key_free(
				     &key,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free key.",
					 function );

					goto on_error;
				}
			}
		}
		else if( property_metadata_entry->value_type == LIBBDE_VALUE_TYPE_UNICODE_STRING )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libbde_metadata_entry_read_string(
			     property_metadata_entry,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read string from property metadata entry.",
				 function );

				goto on_error;
			}
#endif
			if( volume_master_key->string_entry == NULL )
			{
				volume_master_key->string_entry = property_metadata_entry;
			}
		}
		else if( property_metadata_entry->value_type == LIBBDE_VALUE_TYPE_STRETCH_KEY )
		{
			if( libbde_stretch_key_initialize(
			     &stretch_key,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create stretch key.",
				 function );

				goto on_error;
			}
			if( libbde_stretch_key_read(
			     stretch_key,
			     property_metadata_entry,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read stretch key metadata entry.",
				 function );

				goto on_error;
			}
			if( volume_master_key->stretch_key == NULL )
			{
				volume_master_key->stretch_key = stretch_key;

				stretch_key = NULL;
			}
			if( stretch_key != NULL )
			{
				if( libbde_stretch_key_free(
				     &stretch_key,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free stretch key.",
					 function );

					goto on_error;
				}
			}
		}
		else if( property_metadata_entry->value_type == LIBBDE_VALUE_TYPE_AES_CCM_ENCRYPTED_KEY )
		{
			if( libbde_aes_ccm_encrypted_key_initialize(
			     &aes_ccm_encrypted_key,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create AES-CCM encrypted key.",
				 function );

				goto on_error;
			}
			if( libbde_aes_ccm_encrypted_key_read(
			     aes_ccm_encrypted_key,
			     property_metadata_entry,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read AES-CCM encrypted key from property metadata entry.",
				 function );

				goto on_error;
			}
			if( volume_master_key->aes_ccm_encrypted_key == NULL )
			{
				volume_master_key->aes_ccm_encrypted_key = aes_ccm_encrypted_key;

				aes_ccm_encrypted_key = NULL;
			}
			if( aes_ccm_encrypted_key != NULL )
			{
				if( libbde_aes_ccm_encrypted_key_free(
				     &aes_ccm_encrypted_key,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free AES-CCM encrypted key.",
					 function );

					goto on_error;
				}
			}
		}
		if( libbde_array_append_entry(
		     volume_master_key->entries_array,
		     &property_metadata_entry_index,
		     (intptr_t *) property_metadata_entry,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to append property metadata entry to entries array.",
			 function );

			goto on_error;
		}
		property_metadata_entry = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( value_data_size > 0 )
		{
			libnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libnotify_print_data(
			 value_data,
			 value_data_size );
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	if( aes_ccm_encrypted_key != NULL )
	{
		libbde_aes_ccm_encrypted_key_free(
		 &aes_ccm_encrypted_key,
		 NULL );
	}
	if( stretch_key != NULL )
	{
		libbde_stretch_key_free(
		 &stretch_key,
		 NULL );
	}
	if( key != NULL )
	{
		libbde_key_free(
		 &key,
		 NULL );
	}
	if( property_metadata_entry != NULL )
	{
		libbde_metadata_entry_free(
		 &property_metadata_entry,
		 NULL );
	}
	return( -1 );
}

