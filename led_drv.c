/*
 * led_drv.c
 *  Description: Source file for the led driver
 *
 *  Created on: April 02, 2025
 *  Author: Petre Radu
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <avr/io.h>
#include <stddef.h>

#include "led_drv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/


/*#################################*/
/*        Local data types         */
/*#################################*/

typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *port;
	uint8_t pin;
	led_drv_id id;
} led_drv_config;

/*#################################*/
/*        Global ROM data          */
/*#################################*/


/*#################################*/
/*        Global RAM data          */
/*#################################*/


/*#################################*/
/*        Local ROM data           */
/*#################################*/

const led_drv_config led_array[LED_MAX_NUMBER] = {
	// LEFT LED configuration
	{
		&DDRA,
		&PORTA,
		PA0,
		LED_LEFT,
	},
	// RIGHT LED configuration
	{
		&DDRA,
		&PORTA,
		PA1,
		LED_RIGHT,
	}
};

/*#################################*/
/*        Local RAM data           */
/*#################################*/


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

void led_drv_init() {
	uint8_t led_idx;
	for (led_idx = 0; led_idx < LED_MAX_NUMBER; led_idx++) {
		*(volatile uint8_t *) led_array[led_idx].ddr |= (0x01 << led_array[led_idx].pin);
		*(volatile uint8_t *) led_array[led_idx].port &= ~(0x01 << led_array[led_idx].pin);
	}
}

void led_drv_turn_on(led_drv_id led) {
	*(volatile uint8_t *) led_array[led].port |= (0x01 << led_array[led].pin);
}

void led_drv_turn_off(led_drv_id led) {
	*(volatile uint8_t *) led_array[led].port &= ~(0x01 << led_array[led].pin);
}

void led_drv_toggle(led_drv_id led) {
	*(volatile uint8_t *) led_array[led].port ^= (0x01 << led_array[led].pin);
}

/*#################################*/
/*  Local function implementation  */
/*#################################*/