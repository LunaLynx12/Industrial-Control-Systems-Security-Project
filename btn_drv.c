
/*#################################*/
/*       Include-Files             */
/*#################################*/
#include <avr/io.h>
#include <stddef.h>

#include "btn_drv.h"
/*#################################*/
/*       Global defines            */
/*#################################*/

#define DEBOUNCE_FACTOR 3

/*#################################*/
/*      Global data types          */
/*#################################*/

typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *pin;
	uint8_t pin_nr;
	btn_drv_id id;
	btn_drv_state prev_state;
	uint8_t debounce_counter;
	btn_drv_state last_stable_state;
} btn_drv_config;

//STORED IN RAM
btn_drv_config btn_array[BTN_MAX_NUMBER] = {
	// LEFT BTN configuration
	{
		&DDRC,
		&PINC,
		PC1,
		BTN_LEFT,
		BTN_NOT_PRESSED,
		3,
		BTN_NOT_PRESSED

	},
	// RIGHT BTN configuration
	{
		&DDRC,
		&PINC,
		PC0,
		BTN_RIGHT,
		BTN_NOT_PRESSED,
		3,
		BTN_NOT_PRESSED
	}
};






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


void btn_drv_init(){
	uint8_t btn_idx;
	btn_drv_state current_state;

	for (btn_idx = 0; btn_idx < BTN_MAX_NUMBER; btn_idx++) {
		*(volatile uint8_t *) btn_array[btn_idx].ddr &= ~(0x01 << btn_array[btn_idx].pin_nr);
		if ((*(volatile uint8_t *) btn_array[btn_idx].pin & (0x01 << btn_array[btn_idx].pin_nr)) == 0){
			current_state = BTN_NOT_PRESSED;
		} else {
			current_state = BTN_PRESSED;
		}
		btn_array[btn_idx].prev_state = current_state;
		btn_array[btn_idx].last_stable_state = current_state;
		
	}
}

/* Function name: btn_drv_read
   Description: The function that reads the state of the button
   Function parameters:
	  btn: The pin number where the button is connected
*/
btn_drv_state btn_drv_read(btn_drv_id btn){
	return btn_array[btn].last_stable_state;
}

/* Function name: btn_drv_main
   Description: The main function for the btn_drv that will be called cyclically from the OS; triggers internal proccesing inside the driver
   Function parameters:
	  None
*/
void btn_drv_main(){
	uint8_t btn_idx;
	btn_drv_state current_state;
	
	for (btn_idx = 0; btn_idx < BTN_MAX_NUMBER; btn_idx++) {
		if ((*(volatile uint8_t *) btn_array[btn_idx].pin & (0x01 << btn_array[btn_idx].pin_nr)) == 0){
			current_state = BTN_NOT_PRESSED;
		} else {
			current_state = BTN_PRESSED;
		}

		if (current_state != btn_array[btn_idx].prev_state){
			btn_array[btn_idx].debounce_counter = DEBOUNCE_FACTOR;
		} else {
			if(btn_array[btn_idx].debounce_counter > 0){
				btn_array[btn_idx].debounce_counter -= 1;
			} else {
				btn_array[btn_idx].debounce_counter = DEBOUNCE_FACTOR;
				btn_array[btn_idx].last_stable_state = current_state;
			}
		}

		btn_array[btn_idx].prev_state = current_state;
	}
}
