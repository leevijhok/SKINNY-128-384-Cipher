
// Helper functions


/**
* Get bit value of n in position i. Position 0 is lsb.
* Returns bit value.
**/
unsigned int checkBit(unsigned char n, unsigned char i);

/**
* Swap bit value in position i to value v in input n. Position 0 is lsb.
* Returns the new outcome value.
**/
unsigned char swapBit(unsigned char n, unsigned char i, unsigned char v);

/**
* Inserts values from p to c. Values will be picked from i -> j-1.
**/
void getInfo(unsigned char *c, const unsigned char *p, unsigned int i, unsigned int j);



// Skinny operation functions



void subCells(unsigned char *c);
void addConstants(unsigned char *c, unsigned char* c1);
void addRoundTweakey(unsigned char *c, unsigned char *tk1, unsigned char *tk2, unsigned char *tk3);
void shiftRows(unsigned char *c);
void mixColumns(unsigned char *c);



// Skinny function



/**
 * Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/**
 * SKINNY-128-384 block cipher encryption.
 * Under 48-byte tweakey at k, encrypt 16-byte plaintext at p and store the 16-byte output at c.
 */
void skinny(unsigned char *c, const unsigned char *p, const unsigned char *k);

