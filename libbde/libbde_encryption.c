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
#include "libbde_definitions.h"
#include "libbde_diffuser.h"
#include "libbde_encryption.h"

/* Initialize an encryption context
 * Make sure the value encryption context is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_initialize(
     libbde_encryption_context_t **context,
     uint8_t method,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_initialize";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context == NULL )
	{
		*context = memory_allocate_structure(
		            libbde_encryption_context_t );

		if( *context == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *context,
		     0,
		     sizeof( libbde_encryption_context_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear context.",
			 function );

			memory_free(
			 *context );

			*context = NULL;

			return( -1 );
		}
		if( libbde_aes_initialize(
		     &( ( *context )->volume_decryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable initialize volume decryption context.",
			 function );

			goto on_error;
		}
		if( libbde_aes_initialize(
		     &( ( *context )->volume_encryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable initialize volume encryption context.",
			 function );

			goto on_error;
		}
		( *context )->method = method;
	}
	return( 1 );

on_error:
	if( *context != NULL )
	{
		if( ( *context )->volume_decryption_context != NULL )
		{
			libbde_aes_free(
			 &( ( *context )->volume_decryption_context ),
			 NULL );
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( -1 );
}

/* Frees an encryption context
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_free(
     libbde_encryption_context_t **context,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_free";
	int result            = 1;

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( *context != NULL )
	{
		if( libbde_aes_free(
		     &( ( *context )->volume_decryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free volume decryption context.",
			 function );

			result = -1;
		}
		if( libbde_aes_free(
		     &( ( *context )->volume_encryption_context ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable free volume encryption context.",
			 function );

			result = -1;
		}
		memory_free(
		 *context );

		*context = NULL;
	}
	return( result );
}

/* Sets the volume de- and encryption key
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_set_volume_key(
     libbde_encryption_context_t *context,
     const uint8_t *key,
     size_t bit_size,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_set_volume_key";

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( libbde_aes_set_encryption_key(
	     context->volume_decryption_context,
	     (uint8_t *) key,
	     bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set volume key in decryption context.",
		 function );

		return( -1 );
	}
	if( libbde_aes_set_encryption_key(
	     context->volume_encryption_context,
	     (uint8_t *) key,
	     bit_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set volume key in encryption context.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* TODO */
#ifdef TODO

void init_keys(options_structure *options)
{

// volume keys are always 256 bit whatevr the condition
aes_setkey_enc( &options->context.VMK_E_ctx, options->VMK_key+12,256);
aes_setkey_dec( &options->context.VMK_D_ctx, options->VMK_key+12,256);



//FVEK keys depend on algorithm
if ( options->Encryption_Type == AES128 || options->Encryption_Type == AES128_diffuser )  {
 		aes_setkey_enc( &options->context.FVEK_E_ctx, options->FVEK_key+12,128);
                aes_setkey_dec( &options->context.FVEK_D_ctx, options->FVEK_key+12,128);
    }

if ( options->Encryption_Type == AES256 || options->Encryption_Type == AES256_diffuser )  {
 		aes_setkey_enc( &options->context.FVEK_E_ctx, options->FVEK_key+12,256);
                aes_setkey_dec( &options->context.FVEK_D_ctx, options->FVEK_key+12,256);
    }




// TWEAK keys also depend on algorithm
if (  options->Encryption_Type == AES128_diffuser )  {
 		aes_setkey_enc( &options->context.TWEAK_E_ctx, options->Tweak_Key+12,128);
                aes_setkey_dec( &options->context.TWEAK_D_ctx, options->Tweak_Key+12,128);
    }

if (  options->Encryption_Type == AES256_diffuser )  {
 		aes_setkey_enc( &options->context.TWEAK_E_ctx, options->Tweak_Key+12,256);
                aes_setkey_dec( &options->context.TWEAK_D_ctx, options->Tweak_Key+12,256);
    }




// all key contexts have been set

}


#endif

/* De- or encrypts a block of data using AES-CBC
 * Returns 1 if successful or -1 on error
 */
int libbde_encryption_crypt(
     libbde_encryption_context_t *context,
     int mode,
     const uint8_t *input_data,
     size_t input_data_size,
     uint8_t *output_data,
     size_t output_data_size,
     liberror_error_t **error )
{
	uint8_t initialization_vector[ 20 ];

	static char *function = "libbde_encryption_crypt";
	int result            = 0;

	if( context == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid context.",
		 function );

		return( -1 );
	}
	if( ( mode != LIBBDE_ENCYPTION_CRYPT_MODE_DECRYPT )
	 && ( mode != LIBBDE_ENCYPTION_CRYPT_MODE_ENCRYPT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported mode.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     initialization_vector,
	     0,
	     20 ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear initialization vector.",
		 function );

		return( -1 );
	}
	if( mode == LIBBDE_ENCYPTION_CRYPT_MODE_ENCRYPT )
	{
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC ) )
		{
			result = libbde_aes_cbc_crypt(
				  context->volume_encryption_context,
				  LIBBDE_AES_CRYPT_MODE_ENCRYPT,
				  initialization_vector,
				  input_data,
				  input_data_size,
				  output_data,
				  output_data_size,
				  error );
		}
		else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt output data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC )
		 || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC ) )
		{
			result = libbde_aes_cbc_crypt(
				  context->volume_decryption_context,
				  LIBBDE_AES_CRYPT_MODE_DECRYPT,
				  initialization_vector,
				  input_data,
				  input_data_size,
				  output_data,
				  output_data_size,
				  error );
		}
		else if( ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_128_CBC_DIFFUSER )
		      || ( context->method == LIBBDE_ENCRYPTION_METHOD_AES_256_CBC_DIFFUSER ) )
		{
		}
		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to encrypt output data.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

#ifdef TODO

void decrypt_diffused_sector( options_structure *options, // the usual keys
							  unsigned char *sector_data, // actual encrypted data after decrypted data is also available here
							  int32  sector_size, // size of sector which is being decoded
							  int64 sector  )  // this means if a 4 th sector sector is being decoded
{

	unsigned char IV[20]; // used to stor IV which is sector specific

	unsigned char e[20] ; // used to store sector byte offset

	unsigned char sector_key_buffer[40]; // it's actuall a 512 bit value which is xored into the plain text

	int64 temp_var_e;

	unsigned long loop_var;

	
/*aes_context fvek_e_ctxt,fvek_d_ctxt;
	aes_context tweak_e_ctxt,tweak_d_ctxt;


	// initialise contexts

	aes_setkey_dec( &fvek_d_ctxt, options->FVEK_key  + 12, 128);
	aes_setkey_enc( &fvek_e_ctxt, options->FVEK_key  + 12, 128);


	aes_setkey_dec (&tweak_d_ctxt, options->Tweak_Key + 12 , 128);
	aes_setkey_enc (&tweak_e_ctxt, options->Tweak_Key + 12 , 128); // this key is used to  get sector key

*/
	// let us compute e and other data which is necessary for decryption

	/*first e is computed
	e is nothing byt byte offset of that sector from start of volume
	*/ 
	temp_var_e = sector * options->fve_meta_data.BytesPerSector  ;

	memset(e,0,sizeof(e));

	memcpy(e, &temp_var_e , sizeof(temp_var_e)); // copy this number into a buffer in least byte first encoding

	// now let us fill in IV for this sector

	aes_crypt_ecb(&options->context.FVEK_E_ctx,AES_ENCRYPT,e , IV);



	// this block will fill the sector key
	{
	// now let us compuet sector_key_buffer
	aes_crypt_ecb(&options->context.TWEAK_E_ctx,AES_ENCRYPT,e,sector_key_buffer) ;

	//now put 128 in the 16th byte of e
	e[15] = 128; // now e represent's e'
	aes_crypt_ecb(&options->context.TWEAK_E_ctx,AES_ENCRYPT,e,sector_key_buffer+ 16) ;
	}
	

	// now decrypt the buffer usinf AESCBC using the fvek key decryption context
   // we use the same buffer as both input and output
    aes_crypt_cbc(&options->context.FVEK_D_ctx,AES_DECRYPT,options->fve_meta_data.BytesPerSector,IV,sector_data,sector_data);



// now let us call the diffuser B decryptor

	Diffuser_B_Decrypt(sector_data,options->fve_meta_data.BytesPerSector);


	// now let us call the diffuser B decryptor

	Diffuser_A_Decrypt(sector_data,options->fve_meta_data.BytesPerSector);

	// apply sector XOR  wit sector key
	for( loop_var = 0 ; loop_var < options->fve_meta_data.BytesPerSector ;loop_var++)
		sector_data[loop_var] = sector_data[loop_var] ^ sector_key_buffer[ loop_var % 32] ;


// at this stage the buffer is already decrypted succesfully, if everything went right

	return ;

}



#endif

