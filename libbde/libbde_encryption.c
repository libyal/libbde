/*
 * Encryption functions
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

#include "libbde_aes.h"
#include "libbde_encryption.h"

/* Decrypts AES encrypted data
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_aes_ccm_decrypt(
     const uint8_t *key,
     size_t key_size,
     const uint8_t *initialization_vector,
     size_t initialization_vector_size,
     const uint8_t *encrypted_data,
     size_t encrypted_data_size,
     const uint8_t *unencrypted_data,
     size_t unencrypted_data_size,
     liberror_error_t **error )
{
	uint8_t internal_initialization_vector[ 20 ];
	uint8_t block_data[ 16 ];

	static char *function    = "libbde_encryption_aes_ccm_decrypt";
	size_t data_index        = 0;
	uint8_t block_data_index = 0;

/* TODO */
	size_t local_input_length;

	if( initialization_vector == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid initialization vector.",
		 function );

		return( -1 );
	}
/* TODO size check max of 15 bytes */
	if( encrypted_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted data.",
		 function );

		return( -1 );
	}
/* TODO size check, multitude of 16 ? */
	if( unencrypted_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unencrypted data.",
		 function );

		return( -1 );
	}
/* TODO size check */

	/* The internal IV consists of:
	 * 1 byte size value formatted as: 15 - IV size - 1
	 * a maximum of 14 bytes containing IV bytes
	 * 1 byte counter
	 */
	if( memory_set(
	     internal_initialization_vector,
	     0,
	     20 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal initialization vector.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &( internal_initialization_vector[ 1 ] ),
	     initialization_vector,
	     initialization_vector_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy initialization vector.",
		 function );

		return( -1 );
	}
	internal_initialization_vector[ 0 ] = 15 - initialization_vector_size - 1;

	while( data_index < encrypted_data_size )
	{
		AES_ecb_encrypt(
		 internal_initialization_vector,
		 block_data,
		 /* key */,
		 AES_ENCRYPT );

		for( block_data_index = 0;
		     block_data_index < 16;
		     block_data_index++ )
		{
			unencrypted_data[ data_index ] = encrypted_data[ data_index ]
			                               ^ block_data[ block_data_index ];

			data_index++;

			if( data_index >= encrypted_data_size )
			{
				break;
			}
		}
		internal_initialization_vector[ 15 ] += 1;
	}
	return( 1 );
}

int libbde_encryption_decrypt(
     const uint8_t key[ 32 ],
     const uint8_t nonce[ 12 ],
     const uint8_t *encrypted_data,
     size_t encrypted_data_size,
     const uint8_t *unencrypted_data,
     size_t unencrypted_data_size,
     liberror_error_r **error )
{
	aes_context context;

	static char *function = "libbde_encryption_decrypt";

  unsigned char local_input_buffer[1024];
  unsigned char local_output_buffer[1024];
  unsigned long input_size;
  unsigned long local_output_size;
  unsigned char *output_buffer;
  HEADER *header = (HEADER *)input;

	// allocate output_buffer
	output_buffer = malloc(
	                 header->Size - 36 );

	/* 8 byte header
	 * 12 byte nounce
	 * 16 byte authenticator
	 */
	local_output_size= header->Size - 36;

	input_size = header->Size - 20 ;

	/* 8 byte header
	 * 12 byte nounce 
	 * auhtenticator is used while decrypting
	 */
	memcpy( local_input_buffer, input + 20, input_size );

	// set key which is used to decrypt

	aes_setkey_enc(
	 &context,
	 key,
	 256 );

	aes_ccm_encrypt_decrypt(
	 &context,
	 nonce,
	 12,
	 local_input_buffer,
	 input_size,
	 output );

	//local_output_buffer);

	*output_size = local_output_size;

	memmove(output, ((long)output + 16),local_output_size);
	memset ( ((long)output+local_output_size), 0 , 32);

	//memset(local_output_buffer,0,output_size+16);

	free(output_buffer);

	return( 1 );
}

