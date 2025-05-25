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



/*#################################*/
/*      Global data types          */
/*#################################*/

/*
* cysec_drv_state
*  Description: The state of the system, either secure or insecure
*/
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