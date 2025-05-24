/*
 * cysec_drv.h
 *  Description: Header file for the cysec driver
 *
 *  Created on: April 16, 2025
 *  Author: Maftei David
 */
#pragma once
#ifndef cysec_drv_H_
#define cysec_drv_H_
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/
#ifndef MAX_MESSAGE_LEN
#define MAX_MESSAGE_LEN 2048
#endif

// Static buffer for padded message (max padded length)
#define PADDED_LEN (((MAX_MESSAGE_LEN + 8) / 64 + 1) * 64)

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

// Constants
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
/*      Global data types          */
/*#################################*/

typedef enum {
	CYSEC_SECURE=0,
	CYSEC_INSECURE=1,
} cysec_drv_state;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name: cysec_drv_init
   Description: The initialization function of the cysec driver
   Function parameters:
	 None
*/
void cysec_drv_init();

/* Function name: cysec_drv_read
   Description: The function that reads the state of the to system
   Function parameters:
	  None
*/
cysec_drv_state cysec_drv_read();

/* Function name: cysec_drv_main
   Description: The main function for the cysec_drv that will be called cyclically from the OS; triggers internal proccesing inside the driver
   Function parameters:
	  None
*/



void cysec_drv_main();

#endif /* cysec_drv_H_ */