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
    uint8_t test_var = 0;
    
    while(1)
    {
        test_var = 1;
        __delay_ms(2000);
        test_var = 0;
        __delay_ms(2000);
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

