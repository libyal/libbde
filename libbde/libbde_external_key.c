/*
 * External Key metadata entry functions
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

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_external_key.h"
#include "libbde_io_handle.h"
#include "libbde_key.h"
#include "libbde_libcdata.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"
#include "libbde_metadata_entry.h"

#include "bde_metadata.h"

/* Creates an external key
 * Make sure the value external key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_external_key_initialize(
     libbde_external_key_t **external_key,
     libcerror_error_t **error )
{
	static char *function = "libbde_external_key_initialize";

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
	if( *external_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid external key value already set.",
		 function );

		return( -1 );
	}
	*external_key = memory_allocate_structure(
	                 libbde_external_key_t );

	if( *external_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create external key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *external_key,
	     0,
	     sizeof( libbde_external_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear external key.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *external_key )->entries_array ),
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
	if( *external_key != NULL )
	{
		memory_free(
		 *external_key );

		*external_key = NULL;
	}
	return( -1 );
}

/* Frees an external key
 * Returns 1 if successful or -1 on error
 */
int libbde_external_key_free(
     libbde_external_key_t **external_key,
     libcerror_error_t **error )
{
	static char *function = "libbde_external_key_free";
	int result            = 1;

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
	if( *external_key != NULL )
	{
		if( ( *external_key )->key != NULL )
		{
			if( libbde_key_free(
			     &( ( *external_key )->key ),
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
		if( libcdata_array_free(
		     &( ( *external_key )->entries_array ),
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
		 *external_key );

		*external_key = NULL;
	}
	return( result );
}

/* Reads a external key from the metadata entry
 * Returns 1 if successful or -1 on error
 */
int libbde_external_key_read(
     libbde_external_key_t *external_key,
     libbde_metadata_entry_t *metadata_entry,
     libcerror_error_t **error )
{
	libbde_key_t *key                                = NULL;
	libbde_metadata_entry_t *property_metadata_entry = NULL;
	uint8_t *value_data                              = NULL;
	static char *function                            = "libbde_external_key_read";
	size_t value_data_size                           = 0;
	ssize_t read_count                               = 0;
	int property_metadata_entry_index                = 0;

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
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_EXTERNAL_KEY )
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

	if( value_data_size < sizeof( bde_metadata_entry_external_key_header_t ) )
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
	     external_key->identifier,
	     ( (bde_metadata_entry_external_key_header_t *) value_data )->identifier,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libbde_debug_print_guid_value(
		     function,
		     "identifier\t\t\t\t\t",
		     ( (bde_metadata_entry_external_key_header_t *) value_data )->identifier,
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
		     "modification time\t\t\t\t",
		     ( (bde_metadata_entry_external_key_header_t *) value_data )->modification_time,
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
		libcnotify_printf(
		 "\n" );
	}
#endif
	value_data      += sizeof( bde_metadata_entry_external_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_external_key_header_t );

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
			if( external_key->key == NULL )
			{
				external_key->key = key;

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
			if( external_key->string_entry == NULL )
			{
				external_key->string_entry = property_metadata_entry;
			}
		}
		if( libcdata_array_append_entry(
		     external_key->entries_array,
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

