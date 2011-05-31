/*
 * FIPS-197 compliant AES encryption functions
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

/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libbde_aes.h"

/* TODO replace by openssl and MS crypto API */

#define PADLOCK_ALIGN16(x) \
	(unsigned long *) (16 + ((long) x & ~15))

#include <string.h>

#ifndef GET_ULONG_LE
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}
#endif /* GET_ULONG_LE */

#ifndef PUT_ULONG_LE
#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}
#endif /* PUT_ULONG_LE */

/*
 * Forward S-box & tables
 */
static unsigned char FSb[256];
static unsigned long FT0[256]; 
static unsigned long FT1[256]; 
static unsigned long FT2[256]; 
static unsigned long FT3[256]; 

/*
 * Reverse S-box & tables
 */
static unsigned char RSb[256];
static unsigned long RT0[256];
static unsigned long RT1[256];
static unsigned long RT2[256];
static unsigned long RT3[256];

/*
 * Round constants
 */
static unsigned long RCON[10];

/*
 * Tables generation code
 */
#define ROTL8(x) ( ( x << 8 ) & 0xFFFFFFFF ) | ( x >> 24 )
#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )
#define MUL(x,y) ( ( x && y ) ? pow[(log[x]+log[y]) % 255] : 0 )

static int aes_init_done = 0;

static void aes_gen_tables( void )
{
    int i, x, y, z;
    int pow[256];
    int log[256];

    /*
     * compute pow and log tables over GF(2^8)
     */
    for( i = 0, x = 1; i < 256; i++ )
    {
        pow[i] = x;
        log[x] = i;
        x = ( x ^ XTIME( x ) ) & 0xFF;
    }

    /*
     * calculate the round constants
     */
    for( i = 0, x = 1; i < 10; i++ )
    {
        RCON[i] = (unsigned long) x;
        x = XTIME( x ) & 0xFF;
    }

    /*
     * generate the forward and reverse S-boxes
     */
    FSb[0x00] = 0x63;
    RSb[0x63] = 0x00;

    for( i = 1; i < 256; i++ )
    {
        x = pow[255 - log[i]];

        y  = x; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y ^ 0x63;

        FSb[i] = (unsigned char) x;
        RSb[x] = (unsigned char) i;
    }

    /*
     * generate the forward and reverse tables
     */
    for( i = 0; i < 256; i++ )
    {
        x = FSb[i];
        y = XTIME( x ) & 0xFF;
        z =  ( y ^ x ) & 0xFF;

        FT0[i] = ( (unsigned long) y       ) ^
                 ( (unsigned long) x <<  8 ) ^
                 ( (unsigned long) x << 16 ) ^
                 ( (unsigned long) z << 24 );

        FT1[i] = ROTL8( FT0[i] );
        FT2[i] = ROTL8( FT1[i] );
        FT3[i] = ROTL8( FT2[i] );

        x = RSb[i];

        RT0[i] = ( (unsigned long) MUL( 0x0E, x )       ) ^
                 ( (unsigned long) MUL( 0x09, x ) <<  8 ) ^
                 ( (unsigned long) MUL( 0x0D, x ) << 16 ) ^
                 ( (unsigned long) MUL( 0x0B, x ) << 24 );

        RT1[i] = ROTL8( RT0[i] );
        RT2[i] = ROTL8( RT1[i] );
        RT3[i] = ROTL8( RT2[i] );
    }
}

/*
 * AES key schedule (encryption)
 */
void aes_setkey_enc(
      aes_context *ctx,
      unsigned char *key,
      int keysize )
{
    int i;
    unsigned long *RK;

    if( aes_init_done == 0 )
    {
        aes_gen_tables();
        aes_init_done = 1;
    }
    switch( keysize )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return;
    }

    ctx->rk = RK = PADLOCK_ALIGN16( ctx->buf );

    for( i = 0; i < (keysize >> 5); i++ )
    {
        GET_ULONG_LE( RK[i], key, i << 2 );
    }

    switch( ctx->nr )
    {
        case 10:

            for( i = 0; i < 10; i++, RK += 4 )
            {
                RK[4]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[3] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[3]       ) & 0xFF ] << 24 );

                RK[5]  = RK[1] ^ RK[4];
                RK[6]  = RK[2] ^ RK[5];
                RK[7]  = RK[3] ^ RK[6];
            }
            break;

        case 12:

            for( i = 0; i < 8; i++, RK += 6 )
            {
                RK[6]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[5] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[5]       ) & 0xFF ] << 24 );

                RK[7]  = RK[1] ^ RK[6];
                RK[8]  = RK[2] ^ RK[7];
                RK[9]  = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:

            for( i = 0; i < 7; i++, RK += 8 )
            {
                RK[8]  = RK[0] ^ RCON[i] ^
                    ( FSb[ ( RK[7] >>  8 ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[7]       ) & 0xFF ] << 24 );

                RK[9]  = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                    ( FSb[ ( RK[11]       ) & 0xFF ]       ) ^
                    ( FSb[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^
                    ( FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                    ( FSb[ ( RK[11] >> 24 ) & 0xFF ] << 24 );

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;

        default:

            break;
    }
}

/*
 * AES key schedule (decryption)
 */
void aes_setkey_dec( aes_context *ctx, unsigned char *key, int keysize )
{
    int i, j;
    aes_context cty;
    unsigned long *RK;
    unsigned long *SK;

    switch( keysize )
    {
        case 128: ctx->nr = 10; break;
        case 192: ctx->nr = 12; break;
        case 256: ctx->nr = 14; break;
        default : return;
    }

    ctx->rk = RK = PADLOCK_ALIGN16( ctx->buf );

    aes_setkey_enc( &cty, key, keysize );
    SK = cty.rk + cty.nr * 4;

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    for( i = ctx->nr, SK -= 8; i > 1; i--, SK -= 8 )
    {
        for( j = 0; j < 4; j++, SK++ )
        {
            *RK++ = RT0[ FSb[ ( *SK       ) & 0xFF ] ] ^
                    RT1[ FSb[ ( *SK >>  8 ) & 0xFF ] ] ^
                    RT2[ FSb[ ( *SK >> 16 ) & 0xFF ] ] ^
                    RT3[ FSb[ ( *SK >> 24 ) & 0xFF ] ];
        }
    }

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;

    memset( &cty, 0, sizeof( aes_context ) );
}

#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ FT0[ ( Y0       ) & 0xFF ] ^   \
                 FT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                \
    X1 = *RK++ ^ FT0[ ( Y1       ) & 0xFF ] ^   \
                 FT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y0 >> 24 ) & 0xFF ];    \
                                                \
    X2 = *RK++ ^ FT0[ ( Y2       ) & 0xFF ] ^   \
                 FT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                \
    X3 = *RK++ ^ FT0[ ( Y3       ) & 0xFF ] ^   \
                 FT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                 FT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                 FT3[ ( Y2 >> 24 ) & 0xFF ];    \
}

#define AES_RROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)     \
{                                               \
    X0 = *RK++ ^ RT0[ ( Y0       ) & 0xFF ] ^   \
                 RT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                \
    X1 = *RK++ ^ RT0[ ( Y1       ) & 0xFF ] ^   \
                 RT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y2 >> 24 ) & 0xFF ];    \
                                                \
    X2 = *RK++ ^ RT0[ ( Y2       ) & 0xFF ] ^   \
                 RT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                \
    X3 = *RK++ ^ RT0[ ( Y3       ) & 0xFF ] ^   \
                 RT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                 RT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                 RT3[ ( Y0 >> 24 ) & 0xFF ];    \
}

/*
 * AES-ECB block encryption/decryption
 */
void aes_crypt_ecb( aes_context *ctx,
                    int mode,
                    unsigned char input[16],
                    unsigned char output[16] )
{
    int i;
    unsigned long *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3;

    RK = ctx->rk;

    GET_ULONG_LE( X0, input,  0 ); X0 ^= *RK++;
    GET_ULONG_LE( X1, input,  4 ); X1 ^= *RK++;
    GET_ULONG_LE( X2, input,  8 ); X2 ^= *RK++;
    GET_ULONG_LE( X3, input, 12 ); X3 ^= *RK++;

    if( mode == AES_ENCRYPT )
    {
        for( i = (ctx->nr >> 1); i > 1; i-- )
        {
            AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_FROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( FSb[ ( Y0       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

        X1 = *RK++ ^ ( FSb[ ( Y1       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );

        X2 = *RK++ ^ ( FSb[ ( Y2       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

        X3 = *RK++ ^ ( FSb[ ( Y3       ) & 0xFF ]       ) ^
                     ( FSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( FSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
                     ( FSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );
    }
    else /* AES_DECRYPT */
    {
        for( i = (ctx->nr >> 1); i > 1; i-- )
        {
            AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
            AES_RROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

        AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

        X0 = *RK++ ^ ( RSb[ ( Y0       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

        X1 = *RK++ ^ ( RSb[ ( Y1       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );

        X2 = *RK++ ^ ( RSb[ ( Y2       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

        X3 = *RK++ ^ ( RSb[ ( Y3       ) & 0xFF ]       ) ^
                     ( RSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
                     ( RSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
                     ( RSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );
    }

    PUT_ULONG_LE( X0, output,  0 );
    PUT_ULONG_LE( X1, output,  4 );
    PUT_ULONG_LE( X2, output,  8 );
    PUT_ULONG_LE( X3, output, 12 );
}

/*
 * AES-CBC buffer encryption/decryption
 */
void aes_crypt_cbc( aes_context *ctx,
                    int mode,
                    int length,
                    unsigned char iv[16],
                    unsigned char *input,
                    unsigned char *output )
{
    int i;
    unsigned char temp[16];

    if( mode == AES_ENCRYPT )
    {
        while( length > 0 )
        {
            for( i = 0; i < 16; i++ )
                output[i] = (unsigned char)( input[i] ^ iv[i] );

            aes_crypt_ecb( ctx, mode, output, output );
            memcpy( iv, output, 16 );

            input  += 16;
            output += 16;
            length -= 16;
        }
    }
    else
    {
        while( length > 0 )
        {
            memcpy( temp, input, 16 );
            aes_crypt_ecb( ctx, mode, input, output );

            for( i = 0; i < 16; i++ )
                output[i] = (unsigned char)( output[i] ^ iv[i] );

            memcpy( iv, temp, 16 );

            input  += 16;
            output += 16;
            length -= 16;
        }
    }
}

/*
 * AES-CFB buffer encryption/decryption
 */
void aes_crypt_cfb( aes_context *ctx,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[16],
                    unsigned char *input,
                    unsigned char *output )
{
    int c, n = *iv_off;

    if( mode == AES_ENCRYPT )
    {
        while( length-- )
        {
            if( n == 0 )
                aes_crypt_ecb( ctx, mode, iv, iv );

            iv[n] = *output++ = (unsigned char)( iv[n] ^ *input++ );

            n = (n + 1) & 0x0F;
        }
    }
    else
    {
        while( length-- )
        {
            if( n == 0 )
                aes_crypt_ecb( ctx, mode, iv, iv );

            c = *input++;
            *output++ = (unsigned char)( c ^ iv[n] );
            iv[n] = (unsigned char) c;

            n = (n + 1) & 0x0F;
        }
    }

    *iv_off = n;
}

void aes_ccm_decrypt(
      aes_context *ctxt,
      int mode,
      unsigned char *iv,
      unsigned long iv_length,
      unsigned char *input,
      unsigned long input_length,
      unsigned char *output);

char datum_data[] = {
/*0x5C,0x00,0x00,0x00,0x03,0x00,0x01,0x00,0x00,0x10,0x00,0x00,0x94,0xE9,0xA7,0x51
,0xC0,0xA8,0xBF,0xFD,0x7E,0xB4,0xEC,0xF3,0xB9,0x70,0x4C,0xA9, */
0x38,0x00,0x00,0x00
,0x05,0x00,0x01,0x00,0x40,0x78,0x88,0x8D,0xD0,0x23,0xC8,0x01,0x09,0x00,0x00,0x00
,0x07,0x41,0xF6,0xC3,0x2C,0xB0,0x65,0x6E,0x18,0xAA,0x33,0xD1,0x80,0xA2,0x82,0xAE
,0x98,0x94,0x2E,0x0A,0xF3,0xE8,0x46,0xDD,0x3D,0x26,0x26,0xC0,0xCE,0x19,0x7B,0x8D
,0xF3,0xFA,0x18,0xF3//,0x4C,0x09,0xDF,0x74,0xF7,0x69,0xDE,0x1D 
};

uint8_t datum_encrypted[] = {
0x5c,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
0x40,0x78,0x88,0x8d,0xd0,0x23,0xc8,0x01,
0x09,0x00,0x00,0x00,0x85,0x68,0x02,0x2c,0xc2,0x47,0x1b,0x9e,0xdf,0x5e,0x4b,0x1d,
0xca,0x8e,0x2a,0xce,0x14,0x58,0x39,0xd8,0x38,0xbc,0xdd,0xe5,0x9f,0x0e,0x82,0x89,
0x01,0xbf,0xac,0x5d,0x61,0xfa,0xd0,0x4b,0x2c,0x1e,0x46,0xb9,0xa0,0x35,0x5e,0x6f,
0x32,0xf3,0x3a,0xa3,0xb7,0xa5,0x48,0x51,0xab,0x22,0xc3,0xc8,0x66,0x4f,0x14,0x6b,
0x88,0x7d,0x17,0x3d,0xe1,0x5f,0xc5,0x40,0xec,0xfc,0xd3,0x6d 
,0,0,0,0,0,0,0,0
// alignment
};

uint8_t datum_password[] = {
	0x2C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0x06, 0x00, 0x4d, 0xf7,
	0xa8, 0x28, 0xfa, 0xcc, 0xcb, 0xb6, 0xc5, 0x0f,
	0xf2, 0x52, 0xe1, 0x9a, 0x32, 0x39, 0xce, 0x6a,
	0x3c, 0xf9, 0xfb, 0xb9, 0x2d, 0x7f, 0x8e, 0xd9,
	0x54, 0x5d, 0x8a, 0xc0
};

uint8_t datum_password_testing[] = {
	0x2C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0xA8, 0x18, 0x56, 0x08,
	0x9C, 0x83, 0x82, 0x35, 0x15, 0x75, 0x62, 0x87,
	0xD5, 0xF7, 0xD3, 0xCC, 0x6C, 0xC9, 0x90, 0x83,
	0x1B, 0xB5, 0x7E, 0x81, 0x64, 0xF7, 0x89, 0xFC,
	0xA3, 0x24, 0x07, 0xFB
};

uint8_t datum_encrypted_testing[] = {
	0x5C, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00,
	0x02, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
	0x05, 0x00, 0x01, 0x00, 0x70, 0x5D, 0x8B, 0x29,
	0xDA, 0x1F, 0xC8, 0x01, 0x03, 0x00, 0x00, 0x00,
	0x1F, 0x63, 0x25, 0x85, 0xC5, 0xDF, 0xCE, 0x83,
	0x28, 0x42, 0x7F, 0x21, 0x3C, 0x8B, 0x7C, 0x4A,
	0xCA, 0x65, 0xA3, 0xFD, 0xA9, 0x16, 0xB6, 0x73,
	0x8B, 0x6A, 0x8B, 0x10, 0xE2, 0xA3, 0xBC, 0x9C,
	0x77, 0x02, 0xA8, 0x08, 0x55, 0xCC, 0xF3, 0xB7,
	0x91, 0x2D, 0xC6, 0x35, 0xA0, 0x90, 0x23, 0xFA,
	0x67, 0xE8, 0xD4, 0x9A, 0x19, 0xF4, 0x11, 0xC6,
	0x00, 0x18, 0xA8, 0xBB
};

uint8_t datum_encrypted_testing2[] = {
	0x50, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01, 0x00,
	0x70, 0x5D, 0x8B, 0x29, 0xDA, 0x1F, 0xC8, 0x01,
	0x04, 0x00, 0x00, 0x00, 0x9F, 0x79, 0x5A, 0xB8,
	0x98, 0x38, 0x4C, 0x71, 0xA7, 0xCA, 0x62, 0x3A,
	0xDA, 0x49, 0x31, 0x42, 0x5D, 0x6C, 0xB0, 0xC8,
	0xE5, 0x56, 0x53, 0xC4, 0x6B, 0x7A, 0xBD, 0xED,
	0xC9, 0xBF, 0xD4, 0x5D, 0xFE, 0xD9, 0x01, 0xE4,
	0x1C, 0x36, 0x31, 0xE3, 0x42, 0xAA, 0xBD, 0xDA,
	0x7D, 0x10, 0xD7, 0x9F, 0x5F, 0x52, 0xA1, 0xB3,
	0x20, 0xFE, 0xBE, 0x2C, 0xC8, 0x41, 0xE5, 0x00 
};

