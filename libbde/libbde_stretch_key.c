/*
 * Stretch Key metadata entry functions
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

#include <liberror.h>
#include <libnotify.h>

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_metadata_entry.h"
#include "libbde_stretch_key.h"

#include "bde_metadata.h"

/* Initialize a stretch key
 * Make sure the value stretch key is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_stretch_key_initialize(
     libbde_stretch_key_t **stretch_key,
     liberror_error_t **error )
{
	static char *function = "libbde_stretch_key_initialize";

	if( stretch_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stretch key.",
		 function );

		return( -1 );
	}
	if( *stretch_key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid stretch key value already set.",
		 function );

		return( -1 );
	}
	*stretch_key = memory_allocate_structure(
	                libbde_stretch_key_t );

	if( *stretch_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stretch key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *stretch_key,
	     0,
	     sizeof( libbde_stretch_key_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stretch key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *stretch_key != NULL )
	{
		memory_free(
		 *stretch_key );

		*stretch_key = NULL;
	}
	return( -1 );
}

/* Frees a stretch key
 * Returns 1 if successful or -1 on error
 */
int libbde_stretch_key_free(
     libbde_stretch_key_t **stretch_key,
     liberror_error_t **error )
{
	static char *function = "libbde_stretch_key_free";

	if( stretch_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stretch key.",
		 function );

		return( -1 );
	}
	if( *stretch_key != NULL )
	{
		if( ( *stretch_key )->data != NULL )
		{
			memory_free(
			 ( *stretch_key )->data );
		}
		memory_free(
		 *stretch_key );

		*stretch_key = NULL;
	}
	return( 1 );
}

/* Reads a stretch key from the metadata entry
 * Returns 1 if successful or -1 on error
 */
int libbde_stretch_key_read(
     libbde_stretch_key_t *stretch_key,
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libbde_stretch_key_read";
	size_t value_data_size = 0;

	if( stretch_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stretch key.",
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
	if( metadata_entry->value_type != LIBBDE_VALUE_TYPE_STRETCH_KEY )
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

	if( value_data_size < sizeof( bde_metadata_entry_stretch_key_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: value data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (bde_metadata_entry_stretch_key_header_t *) value_data )->encryption_method,
	 stretch_key->encryption_method );

	if( memory_copy(
	     stretch_key->salt,
	     ( (bde_metadata_entry_stretch_key_header_t *) value_data )->salt,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to stretch key.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: encryption method\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
		 function,
		 stretch_key->encryption_method,
		 libbde_debug_print_encryption_method(
		  stretch_key->encryption_method ) );

		libnotify_printf(
		 "%s: salt:\n",
		 function );
		libnotify_print_data(
		 ( (bde_metadata_entry_stretch_key_header_t *) value_data )->salt,
		 16 );
	}
#endif
	value_data      += sizeof( bde_metadata_entry_stretch_key_header_t );
	value_data_size -= sizeof( bde_metadata_entry_stretch_key_header_t );

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
	stretch_key->data = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * value_data_size );

	if( stretch_key->data == NULL )
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
	     stretch_key->data,
	     value_data,
	     value_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data to stretch key.",
		 function );

		goto on_error;
	}
	stretch_key->data_size = value_data_size;
	
	return( 1 );

on_error:
	if( stretch_key->data != NULL )
	{
		memory_free(
		 stretch_key->data );

		stretch_key->data = NULL;
	}
	return( -1 );
}

