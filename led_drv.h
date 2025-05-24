/*
 * led_drv.h
 *  Description: Header file for the led driver
 *
 *  Created on: April 02, 2025
 *  Author: Petre Radu
 */

#ifndef led_drv_H_
#define led_drv_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

/*#################################*/
/*      Global data types          */
/*#################################*/

/* Description: LED identification type
   Values:
       LED_LEFT - Left LED identifier
       LED_RIGHT - Right LED identifier
       LED_MAX_NUMBER - Maximum number of LEDs
*/
typedef enum {
	LED_LEFT=0,
	LED_RIGHT=1,
	LED_MAX_NUMBER=2,
} led_drv_id;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/

/* Function name: led_drv_init
   Description: The initialization function of the led driver
   Function parameters:
	 None
*/
void led_drv_init();

/* Function name: led_drv_turn_on
   Description: This function is used to turn on the LED at a specific pin
   Function parameters:
	 led_pin: led: The id of the LED to be turnned on
*/
void led_drv_turn_on(led_drv_id led);

/* Function name: led_drv_turn_off
   Description: This function is used to turn off the LED at a specific pin
   Function parameters:
	 led: The id of the LED to be turnned on
*/
void led_drv_turn_off(led_drv_id led);

/* Function name: led_drv_toggle
   Description: This function is used to toggle the LED at a specific pin
   Function parameters:
	 led: The id of the LED to be toggled
*/
void led_drv_toggle(led_drv_id led);

#endif /* led_drv_H_ */