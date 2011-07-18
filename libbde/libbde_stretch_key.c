/*
 * Stretch Key metadata entry functions
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
#include <libnotify.h>

#include "libbde_aes_ccm_encrypted_key.h"
#include "libbde_definitions.h"
#include "libbde_io_handle.h"
#include "libbde_metadata_entry.h"
#include "libbde_recovery.h"
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
	if( *stretch_key == NULL )
	{
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
     libbde_io_handle_t *io_handle,
     libbde_metadata_entry_t *metadata_entry,
     uint8_t use_recovery_password,
     liberror_error_t **error )
{
	uint8_t key[ 32 ];

	libbde_metadata_entry_t *property_metadata_entry = NULL;
	uint8_t *value_data                              = NULL;
	static char *function                            = "libbde_stretch_key_read";
	size_t value_data_size                           = 0;
	ssize_t read_count                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                             = 0;
#endif

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
	if( metadata_entry->value_type != 0x0003 )
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

	if( value_data_size < 20 )
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
		byte_stream_copy_to_uint32_little_endian(
		 value_data,
		 value_32bit );
		libnotify_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
#endif
	value_data      += 4;
	value_data_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: salt:\n",
		 function );
		libnotify_print_data(
		 value_data,
		 16 );
	}
#endif
	if( memory_copy(
	     stretch_key->salt,
	     value_data,
	     16 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy salt to stretch key.",
		 function );

		return( -1 );
	}
	value_data      += 16;
	value_data_size -= 16;

	if( memory_set(
	     key,
	     0,
	     32 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key.",
		 function );

		goto on_error;
	}
	if( use_recovery_password != 0 )
	{
		if( io_handle->recovery_password_is_set == 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing recovery password.",
			 function );

			return( -1 );
		}
		if( libbde_recovery_calculate_key(
		     io_handle->recovery_password,
		     stretch_key->salt,
		     key,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine recovery key.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: recovery key:\n",
			 function );
			libnotify_print_data(
			 key,
			 32 );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
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

		if( property_metadata_entry->value_type == LIBBDE_VALUE_TYPE_AES_CMM_ENCRYPTED_KEY )
		{
/* TODO */
			if( libbde_metadata_entry_read_aes_ccm_encrypted_key(
			     property_metadata_entry,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read AES-CMM encrypted key from property metadata entry.",
				 function );

				goto on_error;
			}
		}
		if( libbde_metadata_entry_free(
		     &property_metadata_entry,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free property metadata entry.",
			 function );

			goto on_error;
		}
	}
#endif
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
	if( property_metadata_entry != NULL )
	{
		libbde_metadata_entry_free(
		 &property_metadata_entry,
		 NULL );
	}
	return( -1 );
}

