/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#ifndef MODE
#define MODE DEFAULT
#endif


#if     MODE==TESTPICm
 
void main(void)
{
    GIE = 0;
    TRISD = 0;
    PORTD = 0;
    
    CLROOR;
    CLREROI;
    CLRERDHT;
    CLRFF;
    CLRFE;
    CLRUER;
    
    T0CS = 0;
    PSA = 0;
    PS2 = 1;
    PS1 = 1;
    PS0 = 1;
    
    T0IF = 0;
    T0IE = 1;
    GIE = 1;
    PEIE = 1;
    
    while(1)
    {
        RD1 = 0;
        __delay_ms(1000);
        RD1 = 1;
        __delay_ms(1000);
    }

#elif   MODE==IDLEm

void main(void)
{

#elif   MODE==SLEEPm

void main(void)
{

#elif   MODE==MEASUREm

void main(void)
{

#elif   MODE==TRANSMITm
    
void main(void)
{

#elif   MODE==COMPLETEm

void main(void)
{
    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    char buff;
    uint8_t test;
 
    ResetFifo(&bufferSIM);
    test = ReadFifo(&bufferSIM, &buff);
    while(1)
    {
        MeasureHY();
        MeasureDS();
        MeasureDHT();
        asm("SLEEP");
    }
#endif
}

