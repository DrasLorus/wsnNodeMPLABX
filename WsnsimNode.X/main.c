/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
//    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    fifo bufferSIM;
    char buff;
    uint8_t test;
    
    InitFifo(&bufferSIM);
    test = ReadFifo(&bufferSIM, &buff);
    test = WriteFifo(&bufferSIM, 'A');
    while(1)
    {
        MeasureHY();
        MeasureDS();
        MeasureDHT();
        asm("SLEEP");
    }

}

