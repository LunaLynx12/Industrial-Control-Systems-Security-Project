/*
 * os.h
 *  Description: Operating system header file
 *
 *  Created on: March 20, 2024
 *  Author: Alex Costache
 */

#ifndef OS_H_
#define OS_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/
#define OS_10ms_RUN_TIMER  (2)
#define OS_50ms_RUN_TIMER  (10)
#define OS_100ms_RUN_TIMER (20)
#define OS_500ms_RUN_TIMER (100)

/*#################################*/
/*      Global data types          */
/*#################################*/

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/
extern volatile uint8_t  OS_u8Cnt10ms;
extern volatile uint8_t  OS_u8Cnt50ms;
extern volatile uint16_t OS_u16Cnt100ms;
extern volatile uint16_t OS_u16Cnt500ms;

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name: OS_vTaskInitialization
   Description: OS task initialization function. All components init functions
					can be called in this task during initialization phase of 
					the system.
   Function parameters:
       none
*/
extern void OS_vTaskInitialization(void);

/* Function name: OS_vTimerInit
   Description: OS internal timer initialization function.
   Function parameters:
       none
*/
extern void OS_vTimerInit(void);

/* Function name: OS_vCyclicTask5ms
   Description: OS cyclic 5 ms task.
   Function parameters:
       none
*/
extern void OS_vCyclicTask5ms(void);

/* Function name: OS_vCyclicTask10ms
   Description: OS cyclic 10 ms task.
   Function parameters:
       none
*/
extern void OS_vCyclicTask10ms(void);

/* Function name: OS_vCyclicTask50ms
   Description: OS cyclic 50 ms task.
   Function parameters:
       none
*/
extern void OS_vCyclicTask50ms(void);

/* Function name: OS_vCyclicTask100ms
   Description: OS cyclic 100 ms task.
   Function parameters:
       none
*/
extern void OS_vCyclicTask100ms(void);

/* Function name: OS_vCyclicTask500ms
   Description: OS cyclic 500 ms task.
   Function parameters:
       none
*/
extern void OS_vCyclicTask500ms(void);

/* Function name: OS_vBackgroundTask
   Description: OS background task.
   Function parameters:
       none
*/
extern void OS_vBackgroundTask(void);

/* Function name: OS_vStartFreeRunTimer
   Description: OS startup function which triggers the timer associated.
   Function parameters:
       none
*/
extern void OS_vStartFreeRunTimer(void);

#endif /* OS_H_ */
