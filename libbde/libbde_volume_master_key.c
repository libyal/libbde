/*
 * Volume Master Key (VMK) metadata entry functions
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
#include "libbde_io_handle.h"
#include "libbde_key.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_metadata_entry.h"
#include "libbde_stretch_key.h"
#include "libbde_volume_master_key.h"

#include "bde_metadata.h"

/* Creates a volume master key
 * Make sure the value volume master key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_initialize(
     libbde_volume_master_key_t **volume_master_key,
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_master_key_initialize";

	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( *volume_master_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume master key value already set.",
		 function );

		return( -1 );
	}
	*volume_master_key = memory_allocate_structure(
	                      libbde_volume_master_key_t );

	if( *volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume master key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_master_key,
	     0,
	     sizeof( libbde_volume_master_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume master key.",
		 function );

		memory_free(
		 *volume_master_key );

		*volume_master_key = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *volume_master_key )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_master_key_free";
	int result            = 1;

	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free AES-CCM encrypted key.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *volume_master_key )->entries_array ),
		     (int(*)(intptr_t **, libcerror_error_t **)) &libbde_metadata_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     libcerror_error_t **error )
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
	uint16_t value_16bit                                  = 0;
#endif

	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
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
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_VOLUME_MASTER_KEY )
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

	if( value_data_size < sizeof( bde_metadata_entry_volume_master_key_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: value data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_master_key->identifier,
	     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->protection_type,
	 volume_master_key->protection_type );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libbde_debug_print_guid_value(
		     function,
		     "identifier\t\t\t\t",
		     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->identifier,
		     16,
		     LIBFGUID_ENDIAN_LITTLE,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			goto on_error;
		}
		if( libbde_debug_print_filetime_value(
		     function,
		     "modification time\t\t\t",
		     ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->modification_time,
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
		byte_stream_copy_to_uint16_little_endian(
		 ( (bde_metadata_entry_volume_master_key_header_t *) value_data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: protection type\t\t\t\t: 0x%04" PRIx16 " (%s)\n",
		 function,
		 volume_master_key->protection_type,
		 libbde_debug_print_key_protection_type(
		  volume_master_key->protection_type ) );

		libcnotify_printf(
		 "\n" );
	}
#endif
	value_data      += sizeof( bde_metadata_entry_volume_master_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_volume_master_key_header_t );

	while( value_data_size >= sizeof( bde_metadata_entry_v1_t ) )
	{
		if( memory_compare(
		     value_data,
		     libbde_metadata_entry_empty,
		     sizeof( bde_metadata_entry_v1_t ) ) == 0 )
		{
			break;
		}
		if( libbde_metadata_entry_initialize(
		     &property_metadata_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create key.",
				 function );

				goto on_error;
			}
			if( libbde_key_read(
			     key,
			     property_metadata_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create stretch key.",
				 function );

				goto on_error;
			}
			if( libbde_stretch_key_read(
			     stretch_key,
			     property_metadata_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
			     property_metadata_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free AES-CCM encrypted key.",
					 function );

					goto on_error;
				}
			}
		}
		if( libcdata_array_append_entry(
		     volume_master_key->entries_array,
		     &property_metadata_entry_index,
		     (intptr_t *) property_metadata_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to append property metadata entry to entries array.",
			 function );

			goto on_error;
		}
		property_metadata_entry = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( value_data_size > 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
	return( 1 );

on_error:
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

/* Retrieves the identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_get_identifier(
     libbde_volume_master_key_t *volume_master_key,
     uint8_t *guid_data,
     size_t guid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_master_key_get_identifier";

	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( guid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid GUID data.",
		 function );

		return( -1 );
	}
	if( guid_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid GUID data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     guid_data,
	     volume_master_key->identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the protection type
 * Returns 1 if successful or -1 on error
 */
int libbde_volume_master_key_get_protection_type(
     libbde_volume_master_key_t *volume_master_key,
     uint16_t *protection_type,
     libcerror_error_t **error )
{
	static char *function = "libbde_volume_master_key_get_protection_type";

	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( protection_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid protection type.",
		 function );

		return( -1 );
	}
	*protection_type = volume_master_key->protection_type;

	return( 1 );
}

