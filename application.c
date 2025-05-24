/*
 * application.c
 *  Description: Source file for the application
 *
 *  Created on: April 16, 2025
 *  Author: Petre Radu
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <avr/io.h>
#include <stddef.h>

#include "application.h"
#include "led_drv.h"
#include "btn_drv.h"
#include "cysec_drv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/

/*#################################*/
/*        Local data types         */
/*#################################*/

/*#################################*/
/*        Global ROM data          */
/*#################################*/


/*#################################*/
/*        Global RAM data          */
/*#################################*/


/*#################################*/
/*        Local ROM data           */
/*#################################*/

/*#################################*/
/*        Local RAM data           */
/*#################################*/
btn_drv_state left_btn_state;
btn_drv_state right_btn_state;


/*#################################*/
/*    Local function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/


/*#################################*/
/*  Global function implementation */
/*#################################*/

void application_init() {
	left_btn_state = btn_drv_read(BTN_LEFT);
	right_btn_state = btn_drv_read(BTN_RIGHT);
	
	if (cysec_drv_read() == CYSEC_INSECURE) {
		led_drv_turn_off(LED_LEFT);
		led_drv_turn_off(LED_RIGHT);
	}
}

void application_main() {
	btn_drv_state left_btn_current_state = btn_drv_read(BTN_LEFT);
	btn_drv_state right_btn_current_state = btn_drv_read(BTN_RIGHT);

	if (cysec_drv_read() == CYSEC_INSECURE) {
		led_drv_turn_off(LED_LEFT);
		led_drv_turn_off(LED_RIGHT);
	} else {
		if (left_btn_current_state != left_btn_state && left_btn_state != BTN_NOT_PRESSED) {
			led_drv_toggle(LED_RIGHT);
		}
		if (right_btn_current_state != right_btn_state && right_btn_state != BTN_NOT_PRESSED) {
			led_drv_toggle(LED_LEFT);
		}

		left_btn_state = btn_drv_read(BTN_LEFT);
		right_btn_state = btn_drv_read(BTN_RIGHT);
	}
}

/*#################################*/
/*  Local function implementation  */
/*#################################*/