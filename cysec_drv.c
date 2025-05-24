/*
 * cysec_drv.h
 *  Description: Header file for the cysec driver
 *
 *  Created on: April 16, 2025
 *  Author: Maftei David
 */

#include "cysec_drv.h"

static volatile uint8_t val;
#include <stdint.h>
#include <stddef.h>
static const HASH_READY = 0;
cysec_drv_state systemState = CYSEC_INSECURE;

static uint8_t GOOD_HASH[20] = {0x9A,0xAD,0x4D,0xB9,0x88,0x0F,0x2A,0xC6,0xE4,0x15,0xA0,0xD7,0x60,0xA0,0xDF,0x73,0xCA,0x8E,0x7E,0x1C};
typedef struct {
    const uint8_t *message;   // pointer to the original message bytes
    size_t         msgLen;    // original message length in bytes
    uint64_t       bitLen;    // original message length in bits
    size_t         paddedLen; // total length after padding, in bytes
    size_t         blkOffset; // how many bytes we've processed so far
    uint32_t       h0, h1, h2, h3, h4; // current chaining state
} RIPEMD160_CTX;

//  Initialize context 
void RIPEMD160_Init(RIPEMD160_CTX *C, const uint8_t *message, size_t msgLen) {
    if (msgLen > MAX_MESSAGE_LEN) {
        return;
    }
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
// Process one 64-byte block; return 1 if more blocks remain, 0 when done
int RIPEMD160_Step(RIPEMD160_CTX *C) {
    if (C->blkOffset >= C->paddedLen)
        return 0;  // all blocks processed

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
    return 1;
}

/* --- 3) Finalize: write the 20-byte digest (little-endian h0..h4) --- */
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
            if (byte != GOOD_HASH[i*4 + b]) {
                systemState = CYSEC_INSECURE;   // mismatch ? hash is bad
				return;
            }
        }
    }

    // all 20 bytes matched
    systemState = CYSEC_SECURE;
	RIPEMD160_Init(&ctx, (uint8_t*)0x2000, 2048); //prepare new hash
}
cysec_drv_state cysec_drv_read()
{
	return systemState;
}

void cysec_drv_main()
{
	
	if(RIPEMD160_Step(&ctx) == HASH_READY)
	{
      RIPEMD160_Verify(&ctx);
	}
}

void cysec_drv_init()
{
    RIPEMD160_Init(&ctx, (uint8_t*)0x2000, 2048);
    while (RIPEMD160_Step(&ctx));
	RIPEMD160_Verify(&ctx);
}