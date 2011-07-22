/*
 * Metadata entry functions
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

#include "libbde_debug.h"
#include "libbde_libuna.h"
#include "libbde_metadata_entry.h"

#include "bde_metadata.h"

/* Initialize a metadata entry
 * Make sure the value metadata entry is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_entry_initialize(
     libbde_metadata_entry_t **metadata_entry,
     liberror_error_t **error )
{
	static char *function = "libbde_metadata_entry_initialize";

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
	if( *metadata_entry == NULL )
	{
		*metadata_entry = memory_allocate_structure(
		                   libbde_metadata_entry_t );

		if( *metadata_entry == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create metadata entry.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *metadata_entry,
		     0,
		     sizeof( libbde_metadata_entry_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear metadata entry.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *metadata_entry != NULL )
	{
		memory_free(
		 *metadata_entry );

		*metadata_entry = NULL;
	}
	return( -1 );
}

/* Frees a metadata entry
 * Returns 1 if successful or -1 on error
 */
int libbde_metadata_entry_free(
     libbde_metadata_entry_t **metadata_entry,
     liberror_error_t **error )
{
	static char *function = "libbde_metadata_entry_free";

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
	if( *metadata_entry != NULL )
	{
		if( ( *metadata_entry )->value_data != NULL )
		{
			memory_free(
			 ( *metadata_entry )->value_data );
		}
		memory_free(
		 *metadata_entry );

		*metadata_entry = NULL;
	}
	return( 1 );
}

/* Reads a metadata entry from the metadata data
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libbde_metadata_entry_read(
         libbde_metadata_entry_t *metadata_entry,
         const uint8_t *fve_metadata,
         size_t fve_metadata_size,
         liberror_error_t **error )
{
	static char *function = "libbde_metadata_entry_read";
	uint16_t entry_size   = 0;
	uint16_t version      = 0;

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
	if( fve_metadata == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FVE metadata.",
		 function );

		return( -1 );
	}
	if( fve_metadata_size < sizeof( bde_metadata_header_v1_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid FVE metadata size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: FVE metadata entry:\n",
		 function );
		libnotify_print_data(
		 fve_metadata,
		 sizeof( bde_metadata_entry_v1_t ) );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_v1_t *) fve_metadata )->size,
	 entry_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_v1_t *) fve_metadata )->type,
	 metadata_entry->type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_v1_t *) fve_metadata )->value_type,
	 metadata_entry->value_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (bde_metadata_entry_v1_t *) fve_metadata )->version,
	 version );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: entry size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 entry_size );\

		libnotify_printf(
		 "%s: entry type\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 metadata_entry->type );

		libnotify_printf(
		 "%s: value type\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 metadata_entry->value_type );

		libnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 version );
	}
#endif
	if( version != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported FVE metadata entry version.",
		 function );

		return( -1 );
	}
	if( ( entry_size < sizeof( bde_metadata_entry_v1_t ) )
	 || ( entry_size > fve_metadata_size ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: FVE metadata entry size value out of bounds.",
		 function );

		return( -1 );
	}
	fve_metadata += sizeof( bde_metadata_entry_v1_t );

	metadata_entry->value_data_size = entry_size - (uint16_t) sizeof( bde_metadata_entry_v1_t );

	metadata_entry->value_data = (uint8_t *) memory_allocate(
	                                          sizeof( uint8_t ) * metadata_entry->value_data_size );

	if( metadata_entry->value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     metadata_entry->value_data,
	     fve_metadata,
	     metadata_entry->value_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: FVE metadata entry data:\n",
		 function );
		libnotify_print_data(
		 metadata_entry->value_data,
		 (size_t) metadata_entry->value_data_size );
	}
#endif
	return( (ssize_t) entry_size );

on_error:
	if( metadata_entry->value_data != NULL )
	{
		memory_free(
		 metadata_entry->value_data );

		metadata_entry->value_data = NULL;
	}
	return( -1 );
}

/* Reads a string from the metadata entry
 * Returns the 1 if successful or -1 on error
 */
int libbde_metadata_entry_read_string(
     libbde_metadata_entry_t *metadata_entry,
     liberror_error_t **error )
{
	static char *function                       = "libbde_metadata_entry_read_string";

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t *value_string = NULL;
	size_t value_string_size                    = 0;
	int result                                  = 0;
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
	if( metadata_entry->value_type != 0x0002 )
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
#if defined( HAVE_DEBUG_OUTPUT )
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  metadata_entry->value_data,
		  (size_t) metadata_entry->value_data_size,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  metadata_entry->value_data,
		  (size_t) metadata_entry->value_data_size,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of name string.",
		 function );

		return( -1 );
	}
	value_string = libcstring_system_string_allocate(
			value_string_size );

	if( value_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name string.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) value_string,
		  value_string_size,
		  metadata_entry->value_data,
		  (size_t) metadata_entry->value_data_size,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) value_string,
		  value_string_size,
		  metadata_entry->value_data,
		  (size_t) metadata_entry->value_data_size,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#endif
	if( result != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set name string.",
		 function );

		memory_free(
		 value_string );

		return( -1 );
	}
	libnotify_printf(
	 "%s: string\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 function,
	 value_string );

	memory_free(
	 value_string );

	libnotify_printf(
	 "\n" );
#endif
	return( 1 );
}

