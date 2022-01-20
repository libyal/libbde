/*
 * Key protector functions
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

#include "libbde_key_protector.h"
#include "libbde_libcerror.h"
#include "libbde_types.h"

/* Creates a key protector
 * Make sure the value key_protector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_key_protector_initialize(
     libbde_key_protector_t **key_protector,
     libbde_volume_master_key_t *volume_master_key,
     libcerror_error_t **error )
{
	libbde_internal_key_protector_t *internal_key_protector = NULL;
	static char *function                                   = "libbde_key_protector_initialize";

	if( key_protector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key protector.",
		 function );

		return( -1 );
	}
	if( *key_protector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key protector value already set.",
		 function );

		return( -1 );
	}
	internal_key_protector = memory_allocate_structure(
	                          libbde_internal_key_protector_t );

	if( internal_key_protector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key protector.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_key_protector,
	     0,
	     sizeof( libbde_internal_key_protector_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key protector.",
		 function );

		memory_free(
		 internal_key_protector );

		return( -1 );
	}
	internal_key_protector->volume_master_key = volume_master_key;

	*key_protector = (libbde_key_protector_t *) internal_key_protector;

	return( 1 );

on_error:
	if( internal_key_protector != NULL )
	{
		memory_free(
		 internal_key_protector );
	}
	return( -1 );
}

/* Frees a key protector
 * Returns 1 if successful or -1 on error
 */
int libbde_key_protector_free(
     libbde_key_protector_t **key_protector,
     libcerror_error_t **error )
{
	libbde_internal_key_protector_t *internal_key_protector = NULL;
	static char *function                                   = "libbde_key_protector_free";
	int result                                              = 1;

	if( key_protector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key protector.",
		 function );

		return( -1 );
	}
	if( *key_protector != NULL )
	{
		internal_key_protector = (libbde_internal_key_protector_t *) *key_protector;
		*key_protector         = NULL;

		memory_free(
		 internal_key_protector );
	}
	return( result );
}

/* Retrieves the identifier
 * The identifier is a GUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libbde_key_protector_get_identifier(
     libbde_key_protector_t *key_protector,
     uint8_t *guid_data,
     size_t guid_data_size,
     libcerror_error_t **error )
{
	libbde_internal_key_protector_t *internal_key_protector = NULL;
	static char *function                                   = "libbde_key_protector_get_identifier";

	if( key_protector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key protector.",
		 function );

		return( -1 );
	}
	internal_key_protector = (libbde_internal_key_protector_t *) key_protector;

	if( libbde_volume_master_key_get_identifier(
	     internal_key_protector->volume_master_key,
	     guid_data,
	     guid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from volume master key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libbde_key_protector_get_type(
     libbde_key_protector_t *key_protector,
     uint16_t *type,
     libcerror_error_t **error )
{
	libbde_internal_key_protector_t *internal_key_protector = NULL;
	static char *function                                   = "libbde_key_protector_get_type";

	if( key_protector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key protector.",
		 function );

		return( -1 );
	}
	internal_key_protector = (libbde_internal_key_protector_t *) key_protector;

	if( libbde_volume_master_key_get_protection_type(
	     internal_key_protector->volume_master_key,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve protection type from volume master key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

