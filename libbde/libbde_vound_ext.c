/*
 * Volume functions
 *
 * Copyright (C) 2011-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libbde_vound_ext.h"
#include "libbde_definitions.h"
#include "libbde_volume.h"
#include "libbde_libcerror.h"
#include "libbde_volume_master_key.h"
#include "libbde_password.h"

char libbde_vound_is_clear_key_present(libbde_volume_t* volume,
	libcerror_error_t** error) {

	libbde_internal_volume_t* int_vol = NULL;
	static char* function = "is_clear_key_present";
	
	if (volume == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			"%s: volume must not be NUL",
			function);

		return(FALSE);
	}

	int_vol = (libbde_internal_volume_t *) volume;


	if (int_vol->primary_metadata == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			"%s: Primary metadata must not be null, is volume open ?",
			function);

		return(FALSE);
	}

	if (int_vol->primary_metadata->clear_key_volume_master_key == NULL) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}


int libbde_metadata_validate_recovery_master_key(
	libbde_volume_t* internal,
	libbde_key_t* key,
	libcerror_error_t** error)
{

	libbde_password_keep_t* password_keep = NULL;
	libbde_volume_master_key_t* volume_master_key;
	uint8_t aes_ccm_key[32];

	libcaes_context_t* aes_context = NULL;
	uint8_t* unencrypted_data = NULL;
	static char* function = "libbde_metadata_validate_recovery_master_key";
	size_t unencrypted_data_size = 0;
	uint32_t data_size = 0;
	uint32_t version = 0;
	int result = 0;

	if (internal == NULL) {

		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			"%s: internal volume pointer must not be null. Provide valid volume id ",
			function);
		return(-1);
	}

	password_keep = ((libbde_internal_volume_t*)internal)->password_keep;
	volume_master_key = (libbde_volume_master_key_t*) key;

	if (volume_master_key == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			"%s: invalid metadata - missing recovery password volume master key.",
			function);

		return(-1);
	}
	if (volume_master_key->stretch_key == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			"%s: invalid metadata - invalid recovery password volume master key - missing stretch key.",
			function);

		return(-1);
	}
	if (volume_master_key->aes_ccm_encrypted_key == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			"%s: invalid metadata - invalid recovery password volume master key - missing AES-CCM encrypted key.",
			function);

		return(-1);
	}

	if (memory_set(aes_ccm_key, 0, 32) == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_SET_FAILED,
			"%s: unable to clear AES-CCM key.",
			function);

		goto on_error;
	}

	if (libbde_password_calculate_key(
		password_keep->recovery_password_hash,
		32,
		volume_master_key->stretch_key->salt,
		16,
		aes_ccm_key,
		32,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			"%s: unable to determine AES-CCM key.",
			function);

		goto on_error;
	}

	if (volume_master_key->aes_ccm_encrypted_key->data_size < 28)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			"%s: recovery password volume master key - AES-CCM encrypted key data size value out of bounds.",
			function);

		goto on_error;
	}

	unencrypted_data_size = volume_master_key->aes_ccm_encrypted_key->data_size;
	unencrypted_data = (uint8_t*)memory_allocate(unencrypted_data_size);

	if (unencrypted_data == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			"%s: unable to create unencrypted data.",
			function);

		goto on_error;
	}
	if (memory_set(
		unencrypted_data,
		0,
		unencrypted_data_size) == NULL)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_MEMORY,
			LIBCERROR_MEMORY_ERROR_SET_FAILED,
			"%s: unable to clear unencrypted data.",
			function);

		goto on_error;
	}
	if (libcaes_context_initialize(
		&aes_context,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			"%s: unable initialize AES context.",
			function);

		goto on_error;
	}
	if (libcaes_context_set_key(
		aes_context,
		LIBCAES_CRYPT_MODE_ENCRYPT,
		aes_ccm_key,
		256,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_RUNTIME,
			LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			"%s: unable to set encryption key in AES context.",
			function);

		goto on_error;
	}
	if (libcaes_crypt_ccm(
		aes_context,
		LIBCAES_CRYPT_MODE_DECRYPT,
		volume_master_key->aes_ccm_encrypted_key->nonce,
		12,
		volume_master_key->aes_ccm_encrypted_key->data,
		volume_master_key->aes_ccm_encrypted_key->data_size,
		unencrypted_data,
		unencrypted_data_size,
		error) != 1)
	{
		libcerror_error_set(
			error,
			LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
			"%s: unable to decrypt data.",
			function);

		goto on_error;
	}
	/* TODO improve this check */
	byte_stream_copy_to_uint16_little_endian(
		&(unencrypted_data[16]),
		data_size);

	byte_stream_copy_to_uint16_little_endian(
		&(unencrypted_data[20]),
		version);

	if (version == 1 && data_size == 0x2c) {
		{
			result = 1;

		}
		if (libcaes_context_free(
			&aes_context,
			error) != 1)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_RUNTIME,
				LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				"%s: unable free context.",
				function);

			goto on_error;
		}

		if (memory_set(
			unencrypted_data,
			0,
			unencrypted_data_size) == NULL)
		{
			libcerror_error_set(
				error,
				LIBCERROR_ERROR_DOMAIN_MEMORY,
				LIBCERROR_MEMORY_ERROR_SET_FAILED,
				"%s: unable to clear unencrypted data.",
				function);

			goto on_error;
		}
		memory_free(unencrypted_data);

		unencrypted_data = NULL;

	}

	return(result);

on_error:
	if (unencrypted_data != NULL)
	{
		memory_set(
			unencrypted_data,
			0,
			unencrypted_data_size);
		memory_free(
			unencrypted_data);
	}
	if (aes_context != NULL)
	{
		libcaes_context_free(
			&aes_context,
			NULL);
	}
	return(-1);
}




