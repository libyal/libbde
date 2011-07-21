/*
 * Elephant diffuser encryption functions
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

#include "libbde_diffuser.h"

/* TODO */

unsigned long Diffuser_B_Decrypt(unsigned char *input, int32 input_size);
unsigned long Diffuser_A_Decrypt(unsigned char *input, int32 input_size);


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


// TODO write an ecryption function just convertting first plus into minus will make an encryption function
//this applies to both Diffuser A and Diffuser B , thus we can have write support

// this will apply an in place diffuser B decryption function
int32 Diffuser_B_Decrypt(unsigned char *input, unsigned long input_size)
{
	int32 temp_array[512];
	int32 loop_var;
	int32 max_loop;

	int32 total_loop; // no . of times diffuser is applied to whole block

	//init  array with supplied data
	memcpy(temp_array,input,  input_size);
 

	max_loop = input_size / 4;

	total_loop = 3;  // the diffuser function is applied a total of 3 times

while ( total_loop) {
	// the below loop should be executed 
	for ( loop_var = 0 ; loop_var < (max_loop-1) ;)  {

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var +2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var + 5) % max_loop]),0));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var +2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var + 5) % max_loop]),10));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var +2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var + 5) % max_loop]),0));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var +2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var + 5) % max_loop]),25));
		loop_var++;

	}

	total_loop-- ;
} // end total_loop


// now copy the output onto to the input
memcpy(input, temp_array, input_size);

return 0;
}

// this will apply an in place diffuser A decryption function
int32 Diffuser_A_Decrypt(unsigned char *input, int32 input_size)
{

	unsigned long temp_array[512];
	unsigned long loop_var;
	unsigned long max_loop;

	unsigned long total_loop; // no . of times diffuser is applied to whole block

	//init  array with supplied data
	memcpy(temp_array,input,  input_size);
 

	max_loop = input_size / 4;

	total_loop = 5;  // the diffuser function is applied a total of 3 times

while ( total_loop) {
	// the below loop should be executed 
	for ( loop_var = 0 ; loop_var < (max_loop-1) ;)  {

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var -2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var - 5) % max_loop]),9));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var -2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var - 5) % max_loop]),0));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var -2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var - 5) % max_loop]),13));
		loop_var++;

		temp_array[loop_var] = temp_array[loop_var] + ( temp_array [ (loop_var -2 ) % max_loop] ^  ROTATE( (temp_array [ ( loop_var - 5) % max_loop]),0));
		loop_var++;

	}

	total_loop-- ;
} // end total_loop



// now copy the output onto to the input
memcpy(input, temp_array, input_size);

return 0;

}
