/*
 * Debug functions
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
		case LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER:
			return( "AES-CBC 128-bit encryption with diffuser" );

		case LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER:
			return( "AES-CBC 256-bit encryption with diffuser" );

		case LIBBDE_ENCRYPTION_METHOD_AES_128_CBC:
			return( "AES-CBC 128-bit encryption" );

		case LIBBDE_ENCRYPTION_METHOD_AES_256_CBC:
			return( "AES-CBC 256-bit encryption" );
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

		case LIBBDE_ENTRY_TYPE_STARTUP_KEY:
			return( "Startup key" );

		case LIBBDE_ENTRY_TYPE_DESCRIPTION:
			return( "Description" );

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

/* Prints the key protection type
 */
const char *libbde_debug_print_key_protection_type(
             uint16_t key_protection_type )
{
	switch( key_protection_type )
	{
		case LIBBDE_KEY_PROTECTION_TYPE_CLEAR_KEY:
			return( "clear key protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_TPM:
			return( "TPM protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_STARTUP_KEY:
			return( "startup key protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_RECOVERY_PASSWORD:
			return( "recovery password protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_PASSWORD:
			return( "password protected" );
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

