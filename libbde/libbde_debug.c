/*
 * Debug functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_libbfio.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the encryption method
 */
const char *libbde_debug_print_encryption_method(
             uint32_t encryption_method )
{
	switch( encryption_method )
	{
		case LIBBDE_ENCRYPTION_METHOD_AES_128_CBC:
			return( "AES-CBC 128-bit encryption" );

		case LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER:
			return( "AES-CBC 128-bit encryption with diffuser" );

		case LIBBDE_ENCRYPTION_METHOD_AES_256_CBC:
			return( "AES-CBC 256-bit encryption" );

		case LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER:
			return( "AES-CBC 256-bit encryption with diffuser" );
	}
	return( "_UNKNOWN_" );
}

/* Prints the entry type
 */
const char *libbde_debug_print_entry_type(
             uint16_t entry_type )
{
	switch( entry_type )
	{
		case LIBBDE_ENTRY_TYPE_PROPERTY:
			return( "Property" );

		case LIBBDE_ENTRY_TYPE_VOLUME_MASTER_KEY:
			return( "Volume master key (VMK)" );

		case LIBBDE_ENTRY_TYPE_FULL_VOLUME_ENCRYPTION_KEY:
			return( "Full volume encryption key (FVEK)" );

		case LIBBDE_ENTRY_TYPE_VALIDATION:
			return( "Validation" );

		case LIBBDE_ENTRY_TYPE_VOLUME_NAME:
			return( "Volume name" );

		case LIBBDE_ENTRY_TYPE_VOLUME_HEADER_BLOCK:
			return( "Volume header block" );
	}
	return( "_UNKNOWN_" );
}

/* Prints the value type
 */
const char *libbde_debug_print_value_type(
             uint16_t value_type )
{
	switch( value_type )
	{
		case LIBBDE_VALUE_TYPE_ERASED:
			return( "Erased" );

		case LIBBDE_VALUE_TYPE_KEY:
			return( "Key" );

		case LIBBDE_VALUE_TYPE_UNICODE_STRING:
			return( "Unicode string (UTF-16 little-endian)" );

		case LIBBDE_VALUE_TYPE_STRETCH_KEY:
			return( "Stretch key" );

		case LIBBDE_VALUE_TYPE_USE_KEY:
			return( "Use key" );

		case LIBBDE_VALUE_TYPE_AES_CCM_ENCRYPTED_KEY:
			return( "AES-CCM encrypted key" );

		case LIBBDE_VALUE_TYPE_TPM_ENCODED_KEY:
			return( "TPM encoded key" );

		case LIBBDE_VALUE_TYPE_VALIDATION:
			return( "Validation" );

		case LIBBDE_VALUE_TYPE_VOLUME_MASTER_KEY:
			return( "Volume master key" );

		case LIBBDE_VALUE_TYPE_EXTERNAL_KEY:
			return( "External key" );

		case LIBBDE_VALUE_TYPE_UPDATE:
			return( "Update" );

		case LIBBDE_VALUE_TYPE_ERROR:
			return( "Error" );

		case LIBBDE_VALUE_TYPE_OFFSET_AND_SIZE:
			return( "Offset and size" );
	}
	return( "_UNKNOWN_" );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libbde_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error )
{
	static char *function = "libbde_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

