/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/


#include <xc.h>         /* XC8 General Include File */


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    
    /* Setup analog functionality and port direction */
    
    TRISE = 0x2;            /* RE0(TRIG)/RE2 output, RE1(ECHO) input */
    RE0 = 0;
    
    /* Initialize peripherals */

    /* Enable interrupts */
}

/* HY-SRF05 *******************************************************************/

void Trigger(){
    TRIG = 1;
    __delay_us(10);
    TRIG = 0;
}

int EchoDuration(){
    int t = 0;
    while(!ECHO);
    while(ECHO){
        t++;
    }
    
}
