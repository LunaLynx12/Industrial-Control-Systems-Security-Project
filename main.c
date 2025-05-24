/*
 * main.c
 *  Description: main module which handles OS cyclic tasks
 *
 *  Created on: March 20, 2024
 *  Author: Alex Costache
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <stdio.h>
#include "os.h"

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


/*#################################*/
/*    Local function declaration   */
/*#################################*/


/*#################################*/
/*  Global function implementation */
/*#################################*/
int main()
 { 
   OS_vTimerInit();

   OS_vTaskInitialization();
    
   OS_vStartFreeRunTimer();
   
   while (1)
    {
        if (OS_u8Cnt10ms >= OS_10ms_RUN_TIMER)
        {
            OS_vCyclicTask10ms();
            OS_u8Cnt10ms = 0;
        }

        if (OS_u8Cnt50ms >= OS_50ms_RUN_TIMER)
        {
            OS_vCyclicTask50ms();
            OS_u8Cnt50ms = 0;
        }

        if (OS_u16Cnt100ms >= OS_100ms_RUN_TIMER)
        {
            OS_vCyclicTask100ms();
            OS_u16Cnt100ms = 0;
        }

        if (OS_u16Cnt500ms >= OS_500ms_RUN_TIMER)
        {
            OS_vCyclicTask500ms();
            OS_u16Cnt500ms = 0;
        }

        OS_vBackgroundTask();
    } 
    
   return 0;
 }
 
/*#################################*/
/*  Local function implementation  */
/*#################################*/