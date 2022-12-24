#include <stdio.h>
#include <string.h>
#include "skinny.h"


// M matrix for for mixColumns
static const unsigned char M[] ={ 1,0,1,1,
                                  1,0,0,0,
                                  0,1,1,0,
                                  1,0,1,0};



/**
* Get bit value of n in position i. Position 0 is lsb.
* Returns bit value.
**/
unsigned int checkBit(unsigned char n, unsigned char i)
{
    if (n & (1 << i))
        return 1;
    else
        return 0;
}

/**
* Swap bit value in position i to value v in input n. Position 0 is lsb.
* Returns the new outcome value.
**/
unsigned char swapBit(unsigned char n, unsigned char i, unsigned char v)
{
    int mask = 1 << i;
    return (n & ~mask) | (v << i);
}


/**
* Inserts values from p to c. Values will be picked from i -> j-1.
**/
void getInfo(unsigned char *c, const unsigned char *p, unsigned int i, unsigned int j)
{
    unsigned int index = 0;

    for(i=i; i<j; i++)
    {
        c[index]=p[i];
        index++;
    }
}

void subCells(unsigned char *c)
{
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned char hex;
    unsigned char temp;

    // a xor negation(b v c)
    unsigned int orNegXor(unsigned int a, unsigned int b, unsigned int c)
    {
        return a ^ !(b|c);
    }


    for(i=0; i<16; i++)
    {
        hex = c[i];
        for(j=0; j<4; j++)
        {
            temp = hex;

            // Negation/Or/Xor
            hex = swapBit(hex,0,orNegXor(checkBit(temp,0), checkBit(temp,3), checkBit(temp,2)));
            hex = swapBit(hex,4,orNegXor(checkBit(temp,4), checkBit(temp,7), checkBit(temp,6)));

            if(j==3)
            {
                // Bitswap
                temp = hex;
                hex = swapBit(hex,1,checkBit(temp,2));
                hex = swapBit(hex,2,checkBit(temp,1));
            }

            else
            {
                // Permutation
                temp = hex;
                hex = swapBit(hex,0,checkBit(temp,5));
                hex = swapBit(hex,1,checkBit(temp,3));
                hex = swapBit(hex,2,checkBit(temp,0));
                hex = swapBit(hex,3,checkBit(temp,4));
                hex = swapBit(hex,4,checkBit(temp,6));
                hex = swapBit(hex,5,checkBit(temp,7));
                hex = swapBit(hex,6,checkBit(temp,1));
                hex = swapBit(hex,7,checkBit(temp,2));
            }
        }
        c[i] = hex;
    }
}

void addConstants(unsigned char *c, unsigned char* c1)
{
    // I could not figure out, how the given table is read.
    // So I just calculated it manually.

    // Round constant templates that are applied to internal state.
    unsigned char temp = *c1;
    unsigned char temp2 = 0x00;

    *c1 = swapBit(*c1,0,checkBit(temp,5)^checkBit(temp,4)^1);
    *c1 = swapBit(*c1,1,checkBit(temp,0));
    *c1 = swapBit(*c1,2,checkBit(temp,1));
    *c1 = swapBit(*c1,3,checkBit(temp,2));
    *c1 = swapBit(*c1,4,checkBit(temp,3));
    *c1 = swapBit(*c1,5,checkBit(temp,4));
    temp = *c1;

    // Round constant for 4th index.
    temp2 = swapBit(temp2,0,checkBit(temp,4));
    temp2 = swapBit(temp2,1,checkBit(temp,5));

    // Round constant for 0th index.
    temp = swapBit(temp,4,0);
    temp = swapBit(temp,5,0);

    // Adding the round constants.
    c[0] = c[0]^temp;
    c[4] = c[4]^temp2;
    c[8] = c[8]^0x02;
}

void addRoundTweakey(unsigned char *c, unsigned char *tk1, unsigned char *tk2, unsigned char *tk3)
{
    unsigned int i = 0;

    for(i=0; i<8; i++)
    {
        // Tweakey bitwise XOR
        c[i] = c[i]^tk1[i]^tk2[i]^tk3[i];
    }

    // Tweakey permutations
    void tweakeyPermutation(unsigned char* tk, unsigned int tk_num)
    {
        unsigned int i = 0;
        unsigned char temp[16];
        unsigned char temp_hex;
        unsigned char hex;
        memcpy(temp,tk,16);

        // Permutations
        tk[0] =  temp[9];  tk[1] = temp[15]; tk[2] = temp[8];  tk[3] = temp[13];
        tk[4] =  temp[10]; tk[5] = temp[14]; tk[6] = temp[12]; tk[7] = temp[11];
        tk[8] =  temp[0];  tk[9] = temp[1];  tk[10] = temp[2]; tk[11] = temp[3];
        tk[12] = temp[4];  tk[13] = temp[5]; tk[14] = temp[6]; tk[15] = temp[7];

        // Adding lsfr to TK2
        if(tk_num == 2)
        {
            for(i=0; i<8; i++)
            {
                hex = tk[i];
                temp_hex = hex;

                hex = swapBit(hex,0,checkBit(temp_hex,7)^checkBit(temp_hex,5));
                hex = swapBit(hex,1,checkBit(temp_hex,0));
                hex = swapBit(hex,2,checkBit(temp_hex,1));
                hex = swapBit(hex,3,checkBit(temp_hex,2));
                hex = swapBit(hex,4,checkBit(temp_hex,3));
                hex = swapBit(hex,5,checkBit(temp_hex,4));
                hex = swapBit(hex,6,checkBit(temp_hex,5));
                hex = swapBit(hex,7,checkBit(temp_hex,6));

                tk[i] = hex;
            }
        }
        // Adding lsfr to TK3
        else if(tk_num == 3)
        {
            for(i=0; i<8; i++)
            {
                hex = tk[i];
                temp_hex = hex;

                hex = swapBit(hex,0,checkBit(temp_hex,1));
                hex = swapBit(hex,1,checkBit(temp_hex,2));
                hex = swapBit(hex,2,checkBit(temp_hex,3));
                hex = swapBit(hex,3,checkBit(temp_hex,4));
                hex = swapBit(hex,4,checkBit(temp_hex,5));
                hex = swapBit(hex,5,checkBit(temp_hex,6));
                hex = swapBit(hex,6,checkBit(temp_hex,7));
                hex = swapBit(hex,7,checkBit(temp_hex,0)^checkBit(temp_hex,6));

                tk[i] = hex;
            }
        }
    }

    // Adding permutations and lsfr.
    tweakeyPermutation(tk1,1);
    tweakeyPermutation(tk2,2);
    tweakeyPermutation(tk3,3);
}

void shiftRows(unsigned char *c)
{
    unsigned char temp[16];
    memcpy(temp,c,16);

    // Permutations

    //c[0] = temp[0];     c[1] = temp[1];     c[2] = temp[2];     c[3] = temp[3];
    c[4] = temp[7];     c[5] = temp[4];     c[6] = temp[5];     c[7] = temp[6];
    c[8] = temp[10];    c[9] = temp[11];    c[10] = temp[8];    c[11] = temp[9];
    c[12] = temp[13];   c[13] = temp[14];   c[14] = temp[15];   c[15] = temp[12];
}

void mixColumns(unsigned char *c)
{
    unsigned int i = 0;
    unsigned char hex0; unsigned char hex1; unsigned char hex2; unsigned char hex3;


    // Each internal state column being multiplied with M-matrix.
    for(i=0; i<4; i++)
    {
        hex0 = c[0+i];  hex1 = c[4+i];  hex2 = c[8+i];  hex3 = c[12+i];

        c[0+i]  =    hex0*M[0] ^  hex1*M[1] ^  hex2*M[2] ^  hex3*M[3];
        c[4+i]  =    hex0*M[4] ^  hex1*M[5] ^  hex2*M[6] ^  hex3*M[7];
        c[8+i]  =    hex0*M[8] ^  hex1*M[9] ^  hex2*M[10] ^ hex3*M[11];
        c[12+i] =    hex0*M[12] ^ hex1*M[13] ^ hex2*M[14] ^ hex3*M[15];
    }
}

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k)
{
    // Cipher internal state == c.

    // Round number
    unsigned int round = 0;
    // Round constant
    unsigned char c1 = 0x00;


    // Keys
    unsigned char tk1[16];
    unsigned char tk2[16];
    unsigned char tk3[16];

    // Extraction of initial value to internal state
    getInfo(c,p,0,16);
    // Extraction of tweakey keys
    getInfo(tk1,k,0,16);
    getInfo(tk2,k,16,32);
    getInfo(tk3,k,32,48);


    // Skinny phases
    for(round=0; round<56; round++)
    {
        subCells(c);
        addConstants(c,&c1);
        addRoundTweakey(c,tk1,tk2,tk3);
        shiftRows(c);
        mixColumns(c);
    }

}
