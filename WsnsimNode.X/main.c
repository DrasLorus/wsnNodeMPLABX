
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* User funct/params, such as InitApp */

#define MODE TRANSMITm

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#ifndef MODE
#define MODE DEFAULT
#endif

char buff;
char s[32];
uint8_t ind;

void __interrupt() isr(void)
{
#if MODE == TESTPICm
    if(T0IF){
        uint8_t b = RD2;
        RD2 = !b;
        T0IF = 0;
    }
    else
    {
        /* Unhandled interrupts */
    }
#else
        /* Determine which flag generated the interrupt */
    if(TMR1IF)
    {   
        TMR1ON = 0;
        SETOOR;
        TMR1IF=0; /* Clear Interrupt Flag */
    }
    else if(RCIF)
    {
        if(!ReceiveChar(&buff))
            SETUER;
        if(!WriteFifo(&bufferSIM, buff))
            SETFF;
        CLRFE;
    }
    else
    {
        SETUER;
    }
#endif
}

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
    
    TRISC = 0x80;
    RC6   = 0;
    
    while(1)
    {
        RD1 = 0;
        RC6 = 0;
        __delay_ms(1000);
        RD1 = 1;
        RC6 = 1;
        __delay_ms(1000);
    }      
}
#elif   MODE==IDLEm

void main(void)
{
    InitApp();
    
    while(1){
        
    }
}
#elif   MODE==SLEEPm

void main(void)
{
    InitApp();
    
    while(1){
        asm("SLEEP");
    }
}
#elif   MODE==MEASUREm

void main(void)
{
    InitApp();
    char i = 0;
    
    while(1){
        MeasureDHT();
        MeasureDS();
        MeasureHY();
        __delay_ms(5000);
        
        i++;
        
    }
}
#elif   MODE==TRANSMITm
    
void main(void)
{
    InitApp();
    char temp[64];
    temp[63] = 0;
    uint8_t ind = 0;
    AutobaudSIM();
    while(1){
        if(OERR){
            CREN = 0;
            ResetFifo(&bufferSIM);
            CLRFF;
            SETFE;
            
        }

        __delay_ms(6000);       
        //SendSmsSIM("0947323580","HELLO");
        //__delay_ms(10000);
        if(!FE){
            if(!ReadFifo(&bufferSIM, &(temp[ind]))){
                SETFE;
            }else{
                CLRFF;
                ind++;
            }
            if(ind > 62)
                ind = 0;
        }
    }
}
#elif   MODE==COMPLETEm

void main(void)
{
    /* Initialize I/O and Peripherals for application */
    InitApp();
    SyncPicSIM();
    
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
}   
#endif
