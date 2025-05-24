/*
 * btn_drv.h
 *  Description: Header file for the btn driver
 *
 *  Created on: April 16, 2025
 *  Author: Stefan-Cristian Jarcau
 */

#ifndef btn_drv_H_
#define btn_drv_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

/*#################################*/
/*      Global data types          */
/*#################################*/

typedef enum {
	BTN_LEFT=0,
	BTN_RIGHT=1,
	BTN_MAX_NUMBER=2,
} btn_drv_id;

typedef enum {
	BTN_PRESSED=0,
	BTN_NOT_PRESSED=1,
} btn_drv_state;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name: btn_drv_init
   Description: The initialization function of the btn driver
   Function parameters:
	 None
*/
void btn_drv_init() __attribute__ ((section(".btn")) );

/* Function name: btn_drv_read
   Description: The function that reads the state of the button
   Function parameters:
	  btn: The pin number where the button is connected
*/
btn_drv_state btn_drv_read(btn_drv_id btn) __attribute__ ((section(".btn")) );

/* Function name: btn_drv_main
   Description: The main function for the btn_drv that will be called cyclically from the OS; triggers internal proccesing inside the driver
   Function parameters:
	  None
*/
void btn_drv_main() __attribute__ ((section(".btn")) );

#endif /* btn_drv_H_ */