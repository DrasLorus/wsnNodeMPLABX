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
    
    TRISB = 0x00;
            
    /* Initialize peripherals */ 
    
    /* Enable interrupts */
}

/* HY-SRF05 *******************************************************************/
void TriggerHY(){ 
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
    TMR1H = 0x00;
    TMR1L = 0x00;
    return d;           /* Return the result */
}

double CalcDistance(int time){
    return (((double)time/(double)FCY)*SOUNDSPEED/100/2);
}


void MeasureHY(){
    int i;
    TriggerHY();
    i = EchoDuration();
    distance_cm = CalcDistance(i);
}

/* DS18B20 ********************************************************************/
inline void ReleaseDS(){
    TRISB = 0x00;
}

inline void DriveLowDS(){
    TRISB = 0x04;
    OUTDS = 0;
}

inline void ResetDS(){
    DriveLowDS();
    __delay_us(480);
    ReleaseDS();
}

void InitializationSeqDS(){
    T2CON = 0x02;
    TMR2  = 0x00;
    
    DriveLowDS();
    __delay_us(480);
    ReleaseDS();
    
    TMR2ON = 1;
    if(!OUTDS){
        __delay_us(1);
    }
    while(OUTDS && !TMR2IF);
    if(OUTDS){
        SETEROI;
        TMR2ON = 0;
        TMR2IF = 0;
        exit();
    }
    while(!OUTDS && !TMR2IF);
    if(!OUTDS){
        SETEROI;
        TMR2ON = 0;
        TMR2IF = 0;
        exit();
    }
    while(!TMR2IF);
    TMR2ON = 0;
    TMR2IF = 0;
}

void Write1DS(){
    DriveLowDS();
    __delay_us(5);
    ReleaseDS();
    __delay_us(65);
}

void Write0DS(){
    DriveLowDS();
    __delay_us(65);
    ReleaseDS();
    __delay_us(5);
}

void SendInstructionDS(char c){
    char i;
    for( i = 0 ; i < 8 ; i++){
        if((c & 0x01)){
            Write1DS();
        }else{
            Write0DS();
        }
        c >>= 1;
    }
}

void SkipRom(){
    SendInstructionDS(SKIPROM);
}

void ConvertT(){
    SendInstructionDS(CONVERTT);
    while(!OUTDS);
}


void ReadTemperature(char c[]){
    SendInstructionDS(READSCRATCHPAD);

}


void ReadDS(char * c){
    
}

void MeasureDS(){
    
}
