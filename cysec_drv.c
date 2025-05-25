/*
 * cysec_drv.c
 *  Description: Source file for the cysec driver
 *
 *  Created on: April 16, 2025
 *  Author: Maftei David
 */
/*#################################*/
/*         Include-Files           */
/*#################################*/

#include "cysec_drv.h"
#include <stdint.h>
#include <stddef.h>

/*#################################*/
/*         Local defines           */
/*#################################*/

#define REGION_SIZE 2048
// Rotate left
#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
// Boolean functions
#define F(x, y, z) ((x) ^ (y) ^ (z))
#define G(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define H(x, y, z) (((x) | ~(y)) ^ (z))
#define I(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define J(x, y, z) ((x) ^ ((y) | ~(z)))

// Left and right round functions
#define FUNC_LEFT(i, b, c, d) \
    ((i) < 16 ? F((b), (c), (d)) : \
     (i) < 32 ? G((b), (c), (d)) : \
     (i) < 48 ? H((b), (c), (d)) : \
     (i) < 64 ? I((b), (c), (d)) : \
                J((b), (c), (d)))

#define FUNC_RIGHT(i, b, c, d) \
    ((i) < 16 ? J((b), (c), (d)) : \
     (i) < 32 ? I((b), (c), (d)) : \
     (i) < 48 ? H((b), (c), (d)) : \
     (i) < 64 ? G((b), (c), (d)) : \
                F((b), (c), (d)))



static uint8_t CYSEC_STATIC_HASH[20] = {0x9A,0xAD,0x4D,0xB9,0x88,0x0F,0x2A,0xC6,0xE4,0x15,0xA0,0xD7,0x60,0xA0,0xDF,0x73,0xCA,0x8E,0x7E,0x1C};

/*#################################*/
/*        Local data types         */
/*#################################*/
typedef struct {
    const uint8_t *message;   // pointer to the original message bytes
    size_t         msgLen;    // original message length in bytes
    uint64_t       bitLen;    // original message length in bits
    size_t         paddedLen; // total length after padding, in bytes
    size_t         blkOffset; // how many bytes we've processed so far
    uint32_t       h0, h1, h2, h3, h4; // current chaining state
} RIPEMD160_CTX;

typedef enum {
	COMPRESSION_DONE,
	COMPRESSION_ONGOING,
} RIPE_MD_STATE;

/**#################################*/
/*        Local ROM data           */
/*#################################*/


/**#################################*/
/*        Local RAM data           */
/*#################################*/
cysec_drv_state systemState = CYSEC_INSECURE;

static const uint32_t K_left[5] = { 0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E };
static const uint32_t K_right[5] = { 0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000 };

static const uint8_t r_left[80] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    7,4,13,1,10,6,15,3,12,0,9,5,2,14,11,8,
    3,10,14,4,9,15,8,1,2,7,0,6,13,11,5,12,
    1,9,11,10,0,8,12,4,13,3,7,15,14,5,6,2,
    4,0,5,9,7,12,2,10,14,1,3,8,11,6,15,13
};
static const uint8_t r_right[80] = {
    5,14,7,0,9,2,11,4,13,6,15,8,1,10,3,12,
    6,11,3,7,0,13,5,10,14,15,8,12,4,9,1,2,
    15,5,1,3,7,14,6,9,11,8,12,2,10,0,4,13,
    8,6,4,1,3,11,15,0,5,12,2,13,9,7,10,14,
    12,15,10,4,1,5,8,7,6,2,13,14,0,3,9,11
};

static const uint8_t s_left[80] = {
    11,14,15,12,5,8,7,9,11,13,14,15,6,7,9,8,
    7,6,8,13,11,9,7,15,7,12,15,9,11,7,13,12,
    11,13,6,7,14,9,13,15,14,8,13,6,5,12,7,5,
    11,12,14,15,14,15,9,8,9,14,5,6,8,6,5,12,
    9,15,5,11,6,8,13,12,5,12,13,14,11,8,5,6
};
static const uint8_t s_right[80] = {
    8,9,9,11,13,15,15,5,7,7,8,11,14,14,12,6,
    9,13,15,7,12,8,9,11,7,7,12,7,6,15,13,11,
    9,7,15,11,8,6,6,14,12,13,5,14,13,13,7,5,
    15,5,8,11,14,14,6,14,6,9,12,9,12,5,15,8,
    8,5,12,9,12,5,14,6,8,13,6,5,15,13,11,11
};

/*#################################*/
/*  Local function implementation  */
/*#################################*/

/* Function name: RIPEMD160_Init
   Description: Initializes the context for a new has process
   Function parameters:
	 RIPEMD160_CTX *C 		   pointer to the structore on which the data will be saved
	 const uint8_t *message    the message to be hashed
	 size_t msgLen             the lenght of the message to be hashed
*/
void RIPEMD160_Init(RIPEMD160_CTX *C, const uint8_t *message, size_t msgLen) {
    C->message   = message;
    C->msgLen    = msgLen;
    C->bitLen    = (uint64_t)msgLen * 8;
    {
        size_t totalLen = msgLen + 1 + 8;              // data + 0x80 + 64-bit length
        C->paddedLen   = ((totalLen + 63) / 64) * 64;  // round up to next 64-byte block
    }
    C->blkOffset = 0;
    C->h0 = 0x67452301;
    C->h1 = 0xEFCDAB89;
    C->h2 = 0x98BADCFE;
    C->h3 = 0x10325476;
    C->h4 = 0xC3D2E1F0;
}
static RIPEMD160_CTX ctx;


/* Function name: RIPEMD160_Step
   Description: Process one 64-byte block; return 1 if more blocks remain, 0 when done
   Function parameters:
	 RIPEMD160_CTX *C 		   pointer to the structore on which the data will be saved
*/

RIPE_MD_STATE RIPEMD160_Step(RIPEMD160_CTX *C) {
    if (C->blkOffset >= C->paddedLen)
        return COMPRESSION_DONE;  // all blocks processed

    uint8_t block[64];
    // Build the single padded block:
	int l =0;
    for (l; l < 64; l++) {
        size_t idx = C->blkOffset + l;
        uint8_t v;
        if (idx < C->msgLen) {
            v = pgm_read_byte(C->message + idx);
        } else if (idx == C->msgLen) {
            v = 0x80;
        } else if (idx < C->paddedLen - 8) {
            v = 0x00;
        } else {
            // little-endian length field
            v = (uint8_t)((C->bitLen >> (8 * (idx - (C->paddedLen - 8)))) & 0xFF);
        }
        block[l] = v;
    }

    // Expand block[] into sixteen 32-bit words X[0..15]
    uint32_t X[16];
    int i =0;
    for (i; i < 16; i++) {
        X[i] =  (uint32_t)block[i*4 + 0]
              | ((uint32_t)block[i*4 + 1] <<  8)
              | ((uint32_t)block[i*4 + 2] << 16)
              | ((uint32_t)block[i*4 + 3] << 24);
    }

    // Compression: two parallel tracks
    uint32_t A1 = C->h0, B1 = C->h1, C1 = C->h2, D1 = C->h3, E1 = C->h4;
    uint32_t A2 = C->h0, B2 = C->h1, C2 = C->h2, D2 = C->h3, E2 = C->h4;
		int j = 0;
    for (j; j < 80; j++) {
        uint32_t T1 = ROTL(
            A1 + FUNC_LEFT(j, B1, C1, D1)
               + X[r_left[j]]
               + K_left[j/16],
            s_left[j]
        ) + E1;
        A1 = E1;  E1 = D1;  D1 = ROTL(C1, 10);  C1 = B1;  B1 = T1;

        uint32_t T2 = ROTL(
            A2 + FUNC_RIGHT(j, B2, C2, D2)
               + X[r_right[j]]
               + K_right[j/16],
            s_right[j]
        ) + E2;
        A2 = E2;  E2 = D2;  D2 = ROTL(C2, 10);  C2 = B2;  B2 = T2;
    }

    // Combine results back into state
    uint32_t T = C->h1 + C1 + D2;
    C->h1 = C->h2 + D1 + E2;
    C->h2 = C->h3 + E1 + A2;
    C->h3 = C->h4 + A1 + B2;
    C->h4 = C->h0 + B1 + C2;
    C->h0 = T;

    C->blkOffset += 64;
    return COMPRESSION_ONGOING;
}

/* Function name: RIPEMD160_Verify
   Description: Interprets the raw hash from a finalized compression
   Function parameters:
	 RIPEMD160_CTX *C 		   pointer to the structore on which the data will be interpretated
*/

void RIPEMD160_Verify(RIPEMD160_CTX *C) {
    // Grab the five 32-bit state words
    uint32_t H[5] = { C->h0, C->h1, C->h2, C->h3, C->h4 };

    // For each word, peel off its 4 bytes (LSB first) and compare
		int i = 0;
    for (i; i < 5; i++) {
        uint32_t v = H[i];
        int b = 0;
        for (b ; b < 4; b++, v >>= 8) {
            uint8_t byte = (uint8_t)(v & 0xFF);
            if (byte != CYSEC_STATIC_HASH[i*4 + b]) {
                systemState = CYSEC_INSECURE;   // mismatch ? hash is bad
				return;
            }
        }
    }

    // all 20 bytes matched
    systemState = CYSEC_SECURE;
}

/**#################################*/
/*  Global function implementation */
/*#################################*/

cysec_drv_state cysec_drv_read()
{
	return systemState;
}
void cysec_drv_main()
{
	if(RIPEMD160_Step(&ctx) == COMPRESSION_DONE)
	{
      RIPEMD160_Verify(&ctx);
	  RIPEMD160_Init(&ctx, (uint8_t*)0x2000, REGION_SIZE); //prepare new hash
	}
}
void cysec_drv_init()
{
		RIPEMD160_Init(&ctx, (uint8_t*)0x2000, REGION_SIZE);
		while (RIPEMD160_Step(&ctx));
		RIPEMD160_Verify(&ctx);   
	
}