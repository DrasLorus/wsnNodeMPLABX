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
    T1CON = 0x00;
    TMR1H = 0x00;
    TMR1L = 0x00;
    T1CON = 0x00;
    /* Enable interrupts */
}

/* HY-SRF05 *******************************************************************/

void Trigger(){ 
    TRIG = 1;
    __delay_us(10);
    TRIG = 0;
}

int EchoDuration(){     
    int d = 0;
    while(!ECHO);       /* When Echo pin is driven High */
    TMR1ON = 1;         /* Activate timer 1 and let it run */
    while(ECHO);        /* Then, when the pin is driven low */
    TMR1ON = 0;         /* Desactivate timer 1 */
    d = TMR1H;          /* Extract the MSB */
    d <<= 8;            /* Put them on high position*/
    d += TMR1L;         /* Add the LSB */
    return d;           /* Return the result */
}

inline double CalcDistance(int time){
    return (((double)time/(double)FCY)*SOUNDSPEED/100/2);
}


void MeasureHY(){
    int i;
    Trigger();
    i = EchoDuration();
    distance_cm = CalcDistance(i);
    TMR1H = 0x00;
    TMR1L = 0x00;
}
