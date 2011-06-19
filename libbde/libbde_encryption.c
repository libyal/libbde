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

int libbde_encryption_decrypt(
     const uint8_t key[ 32 ],
     const uint8_t nonce[ 12 ],
     const uint8_t *encrypted_data,
     size_t encrypted_data_size,
     const uint8_t *unencrypted_data,
     size_t unencrypted_data_size,
     liberror_error_r **error )
{
	libbde_aes_context context;

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

	libbde_aes_setkey_enc(
	 &context,
	 key,
	 256 );

	libbde_aes_ccm_encrypt_decrypt(
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

