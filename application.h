/*
 * application.h
 *  Description: Header file for the application
 *
 *  Created on: April 16, 2025
 *  Author: Petre Radu
 */

#ifndef application_drv_H_
#define application_drv_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

/*#################################*/
/*      Global data types          */
/*#################################*/

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name: application_init
   Description: The initialization function of the application
   Function parameters:
	 None
*/
void application_init();

/* Function name: application_main
   Description: The main function for the application that will be called cyclically from the OS; triggers internal proccesing inside the driver
   Function parameters:
	  None
*/
void application_main();

#endif /* application_drv_H_ */