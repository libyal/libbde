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
int libbde_encryption_context_initialize(
     libbde_encryption_context_t **context,
     uint8_t encryption_method,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_context_initialize";

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
		     sizeof( libbde_context_t ) ) == NULL )
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
		     ( *context )->volume_decryption_context,
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
		     ( *context )->volume_encryption_context,
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
	}
	return( 1 );

on_error:
	if( *context != NULL )
	{
		if( ( *context )->volume_decryption_context != NULL )
		{
			libbde_aes_context_free(
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
int libbde_encryption_context_free(
     libbde_encryption_context_t **context,
     liberror_error_t **error )
{
	static char *function = "libbde_encryption_context_free";
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
		if( libbde_aes_context_free(
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
		if( libbde_aes_context_free(
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

/* Sets the volume decryption and encryption key
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




void decrypt_diffused_sector( options_structure *options, // the usual keys
							  int8 *sector_data, // actual encrypted data after decrypted data is also available here
							  int32  sector_size, // size of sector which is being decoded
							  int64 sector  );

// this means a sector encrypted with pure AES 128 or AES 256
unsigned long decrypt_normal_sector( options_structure *options, // the usual keys
							  int8 *sector_data, // actual encrypted data after decrypted data is also available here
							  int32  sector_size, // size of sector which is being decoded
							  int64 sector  )  // this means if a 4 th sector sector is being decoded
{

aes_context fvek_d_ctxt;
 int8 IV[20] ;  // initilization vector



/*
// if algorithm is not AES 128 or AES 256 return -1
if ( options->Encryption_Type == AES128_diffuser ||
	 options->Encryption_Type == AES256_diffuser )
	 return -1;

// setup key schedule  according to the algorithm

if ( options->Encryption_Type == AES128 ) 
			aes_setkey_dec( &fvek_d_ctxt, options->FVEK_key  + 12, 128); // 128 bit encryption
else if (options->Encryption_Type == AES256)
           aes_setkey_dec( &fvek_d_ctxt, options->FVEK_key  + 12, 256); // select 256 bit encryption
else
  return -1;

*/
 
		memset(IV,0,sizeof(IV));  // init initialization vector
		aes_crypt_cbc(&options->context.FVEK_D_ctx,AES_DECRYPT,sector_size,IV,sector_data,sector_data);



return 0;
}



// this will select a decryptor based on the algorithm selected in the FVEK
// and return result
void decrypt_sector(options_structure *options,
					int8 *sector_data,
					 int64 sector )
{

	// select a function based on algorithm 

	if ( options->Encryption_Type == AES128 || options->Encryption_Type == AES256 )
		  return decrypt_normal_sector(options,sector_data,options->fve_meta_data.BytesPerSector,sector);


	if ( options->Encryption_Type == AES128_diffuser || options->Encryption_Type == AES256_diffuser )
		  return decrypt_diffused_sector(options,sector_data,options->fve_meta_data.BytesPerSector,sector);


	// we are here means unknown algorithm

	return -1;



}



// this function does the actual decryption of data
void decrypt_data(Interface* input_interface, options_structure *options) {
	unsigned char encrypted_buffer[ 8192];
	unsigned char decrypted_buffer[8192];

	FILE *stream;


	unsigned long loop_var;
	


	if( (stream = fopen( "decrypted", "w+b" )) == NULL )    {

		printf("Output file could not be opned");
    return ;
	}


	// process first sector and write it

	Read_Sector( input_interface,0  ,1, encrypted_buffer);
	fix_sector(options,encrypted_buffer);
	fwrite(encrypted_buffer,1,512,stream);

	loop_var =0; // start decrypting from the 9 sector


#define DECRYPTED_SECTORS 10
	for ( loop_var =1 ;loop_var < DECRYPTED_SECTORS;loop_var++)
	{
				Read_Sector( input_interface,loop_var  ,1, encrypted_buffer);
			     fwrite(encrypted_buffer,1,512,stream);
	}

loop_var = DECRYPTED_SECTORS  ;

	//for(loop_var = 0; loop_var < 100000 ;loop_var++) 
	while( 1)//!feof(input_interface->stream ))
	{
		
loop_var++;
	// read one sector at a time and decrypt it
	//if (! feof( input_interface->stream))	{
		Read_Sector( input_interface,loop_var  ,1, encrypted_buffer);
	//}

		// if eof break;

		if ( feof(input_interface->stream ))
			break;
	


		// sector has been read now decrypt it
	decrypt_sector(options,encrypted_buffer,loop_var);

	

     fwrite(encrypted_buffer,1,512,stream);

	}


	// we are here means whole file has been decrypte including the last sector
	// so fix it up
	// it is done by seeking back 512 bytes
	// reading  0 sector , fixing it up
	// and then writing it again

	custom_fseek(stream, (int64)0 - (int64)input_interface->SectorSize ,SEEK_CUR);
	Read_Sector( input_interface,0  ,1, encrypted_buffer);
	fix_sector(options,encrypted_buffer);
	 fwrite(encrypted_buffer,1,512,stream);





fclose(stream);

}

#endif

