
/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* User funct/params, such as InitApp */

#define MODE COMPLETEm

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#ifndef MODE
#define MODE DEFAULT
#endif

volatile char * buff;
volatile char text[32];

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
        if(!ReceiveChar((char *) buff))
            SETUER;
        if(buff - text < 32){
            if((*buff != CR) && (*buff != LF)){
                buff++;
            }else if(*buff == LF){
                if((*(buff - 2) == 'O') && (*(buff - 1) == 'K')){
                    SETACK;
                    buff = text;
                }else if((*(buff - 5) == 'E') && (*(buff - 4) == 'R') && (*(buff - 3) == 'R') && (*(buff - 2) == 'O') && (*(buff - 1) == 'R')){
                    SETUER;
                    buff = text;
                }
            }
        }else{
            buff = text;
        }
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
        ConvertMeasureToStr();
        i++;
        
    }
}
#elif   MODE==TRANSMITm
    
void main(void)
{
    buff = text;
    
    /*
    TRISC = 0x80;

    while(1){
        __delay_ms(1000);   
        RC6 = 1;
        __delay_ms(1000);
        RC6 = 0;
    }
    */
    
    InitApp();

    AutobaudSIM();
    __delay_ms(1000);   
   SendCommandSIM("E0");
    __delay_ms(10000);
    SendSmsSIM("+84947323580","Hello");   
    while(1){
        
        __delay_ms(1);       
        AutobaudSIM();   
        __delay_ms(10000);
    }
}
#elif   MODE==COMPLETEm

void main(void)
{
    /* Initialize I/O and Peripherals for application */
    buff = text;
    
    InitApp();
    AutobaudSIM();
    SendCommandSIM("E0");
    Acknowledge();
    while(1)
    {
        MeasureHY();
        MeasureDS();
        MeasureDHT();
        
        ConvertMeasureToStr();
        
        SendSmsSIM("+84947323580", (char *) sms);
        //asm("SLEEP");
        __delay_ms(10000);
        while(UER)
            ;
    }
}   
#endif
