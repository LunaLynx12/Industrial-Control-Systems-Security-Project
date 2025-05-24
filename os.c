/*
 * os.c
 *  Description: Operating system c source file
 *
 *  Created on: March 20, 2024
 *  Author: Alex Costache
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

/*Include project header files*/
#include "os.h"
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
volatile uint8_t  OS_u8Cnt10ms;
volatile uint8_t  OS_u8Cnt50ms;
volatile uint16_t OS_u16Cnt100ms;
volatile uint16_t OS_u16Cnt500ms;


/*#################################*/
/*        Local ROM data           */
/*#################################*/

/*#################################*/
/*        Local RAM data           */
/*#################################*/
uint16_t startTimer = 0;
uint16_t applicationTimer = 0;
uint8_t  systemInitialized = 0;

/*#################################*/
/*    Local function declaration   */
/*#################################*/

/*#################################*/
/*  Global function implementation */
/*#################################*/

void OS_vTaskInitialization(void)
{
	cysec_drv_init();

	led_drv_init();
	btn_drv_init();
	application_init();
	systemInitialized = 1;
	
}

void OS_vTimerInit(void)
{
   /*Set 8 bit Timer0 on Clear Timer on Compare Match (CTC) Mode with clkIO/256 clock source
    * TCCR0 register: WGM01=1 WGM00=0       -> CTC Mode, 8-bit with OCR1C top
    *                 CS02=1 CS01=0 CS00=0  -> clkIO/256 from prescaler
    * Max value OCR1C = 0x9C
    * Timer set to generate interrupt at 5ms*/
   TCCR0 = (1<<WGM01) | (1<<CS02);
   OCR0 = 0x9C;
}

void OS_vCyclicTask5ms(void)
{
	btn_drv_main();
	
}

void OS_vCyclicTask10ms(void)
{
		 
}

void OS_vCyclicTask50ms(void)
{
	applicationTimer++;
	if(systemInitialized && applicationTimer > 5)
	{
		cysec_drv_main();
        applicationTimer = 0;
	}
}

void OS_vCyclicTask100ms(void)
{
	application_main();
}

void OS_vCyclicTask500ms(void)
{
	
}

void OS_vBackgroundTask(void)
{

}

void OS_vStartFreeRunTimer(void)
{
   /*Set Timer/Counter0 Compare Match interrupt enable*/
   TIMSK |= (1<<OCIE0);
   sei();
}

/*Interrupt is generated every 5ms*/
ISR(TIMER0_COMP_vect)
{
   OS_vCyclicTask5ms();

   /*Increase counters used for other OS cyclic functions*/
   OS_u8Cnt10ms++;
   OS_u8Cnt50ms++;
   OS_u16Cnt100ms++;
   OS_u16Cnt500ms++;
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/
