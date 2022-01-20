/*
 * Debug functions
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
#include <memory.h>
#include <types.h>

#include "libbde_debug.h"
#include "libbde_definitions.h"
#include "libbde_libbfio.h"
#include "libbde_libcerror.h"
#include "libbde_libcnotify.h"
#include "libbde_libfdatetime.h"
#include "libbde_libfguid.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the encryption method
 */
const char *libbde_debug_print_encryption_method(
             uint16_t encryption_method )
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

		case LIBBDE_ENCRYPTION_METHOD_AES_128_XTS:
			return( "AES-XTS 128-bit encryption" );

		case LIBBDE_ENCRYPTION_METHOD_AES_256_XTS:
			return( "AES-XTS 256-bit encryption" );

		case LIBBDE_ENCRYPTION_METHOD_NONE:
			return( "None" );
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

		case LIBBDE_KEY_PROTECTION_TYPE_TPM_AND_PIN:
			return( "TPM and PIN protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_RECOVERY_PASSWORD:
			return( "recovery password protected" );

		case LIBBDE_KEY_PROTECTION_TYPE_PASSWORD:
			return( "password protected" );
	}
	return( "_UNKNOWN_" );
}

/* Prints a FILETIME value
 * Returns 1 if successful or -1 on error
 */
int libbde_debug_print_filetime_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	static char *function             = "libbde_debug_print_filetime_value";

	if( libfdatetime_filetime_initialize(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create filetime.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_from_byte_stream(
	     filetime,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to filetime.",
		 function );

		goto on_error;
	}
	if( libfdatetime_filetime_copy_to_utf8_string(
	     filetime,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy filetime to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
	 value_name,
	 date_time_string );

	if( libfdatetime_filetime_free(
	     &filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free filetime.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
	return( -1 );
}

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libbde_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid = NULL;
	static char *function       = "libbde_debug_print_guid_value";

	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libbde_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libbde_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

