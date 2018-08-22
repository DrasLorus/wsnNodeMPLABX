/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/


#include <xc.h>         /* XC8 General Include File */

#include "user.h"

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    
    GIE = 0;
    
    /* Setup analog functionality and port direction */
    
    ADCON1 = 0x82;          /* Set all RE pins as Digital I/O b'10000010' */
    TRISE = 0x2;            /* RE0(TRIG)/RE2 output, RE1(ECHO) input */
    RE0 = 0;
    
    TRISB = 0x05;
            
    /* Initialize peripherals */ 
    InitUsart(USART_BAUD_9600);
    RCIE = 1;
    /* Enable interrupts */
    GIE = 1;
    PEIE = 1;
    
    /* Clear all flags */
    CLROOR;
    CLREROI;
    CLRERDHT;
    CLRUER;
    
    ResetDS();
    
}

/******************************************************************************/
/* HY-SRF05 *******************************************************************/
/******************************************************************************/
void TriggerHY(){   
    T1CON = 0x00;

    TMR1H = 0x00;
    TMR1L = 0x00;
    
    TRIG = 1;
    __delay_us(10);
    TRIG = 0;
}

uint16_t EchoDuration(){     
    uint16_t d = 0;
    while(!ECHO);       /* When Echo pin is driven High */
    TMR1ON = 1;         /* Activate timer 1 and let it run */
    while(ECHO);        /* Then, when the pin is driven low */
    TMR1ON = 0;         /* Desactivate timer 1 */
    d = (TMR1L | (TMR1H<<8));
    return d;           /* Return the result */
}

double CalcDistance(uint16_t time){
    return (((double)time)/((double)FCY)*100.0*((double)SOUNDSPEED)/2.0);
}


void MeasureHY(){
    int i;
    TriggerHY();
    i = EchoDuration();
    distance_cm = CalcDistance(i);
}
/******************************************************************************/
/* DS18B20 ********************************************************************/
/******************************************************************************/
inline void ReleaseDS(){
    TRISB |= 0x04;  // xxxxxxxx | 00000100 = xxxxxx1xx
}

inline void DriveLowDS(){
    TRISB &= 0xFB;  // xxxxxxxx & 11111011 = xxxxxx0xx
    OUTDS = 0;
}

inline void WaitTMR2IFDS(){
    TMR2 = 0;
    TMR2ON = 1;
    while(!TMR2IF);
    TMR2ON = 0;
    TMR2IF = 0;
}

inline void TMR2Config480us(){
    T2CON = 0x02;       // '00000010'
    PR2   = 150;        // 150 x 16 x (1/5)E-6 = 480E-6 
}

inline void TMR2Config10us(){
    T2CON = 0x00;       // '00000000'
    PR2   = 50;         // 50 x (1/5)E-6 = 10E-6 
}

void ResetDS(){
    TMR2Config480us(); 
    
    DriveLowDS();
    WaitTMR2IFDS();
    
    ReleaseDS();   
    WaitTMR2IFDS();
}

void InitializationSeqDS(){
    TMR2Config480us();       
    
    DriveLowDS();
    __delay_us(480);
    ReleaseDS();
    
    TMR2 = 0;
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
        return;
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

void SendInstructionDS(uint8_t c){
    uint8_t i;
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
    __delay_us(1);
}

void ConvertT(){
    SendInstructionDS(CONVERTT);
    while(!OUTDS);
    __delay_us(1);
}


void ReadTemperatureDS(){
    SendInstructionDS(READSCRATCHPAD);
    uint8_t i;  
    for ( i = 0 ; i < 16 ; i++){            
        temperatureDS[(i>>3)] >>= 1; 
        temperatureDS[(i>>3)] += (ReadDS() << 7);
    }
    ResetDS();
}


uint8_t ReadDS(){
    TMR2Config10us();
    uint8_t c = 0;
    
    DriveLowDS();
    __delay_us(1);
    ReleaseDS();
    TMR2ON = 1;
    while(!TMR2IF){
        c = OUTDS;
    }
    TMR2ON = 0;
    TMR2IF = 0;
    __delay_us(50);
    return c;
}

void MeasureDS(){
    InitializationSeqDS();
    SkipRom();
    ConvertT();
    
    InitializationSeqDS();
    SkipRom();
    ReadTemperatureDS();  
}

/******************************************************************************/
/* DHT11 **********************************************************************/
/******************************************************************************/
inline void DriveLowDHT(){
    TRISB  &= 0xFE;     // xxxxxxxx & 11111110 = xxxxxxxx0
    OUTDHT = 0;
}

inline void ReleaseDHT(){
    TRISB  |= 0x01;     // xxxxxxxx | 00000001 = xxxxxxxx1
}

inline void TMR2Config40us(void){
    T2CON = 0x00;       // '00000000'
    PR2   = 200;        // 200 x (1/5)E-6 = 40E-6 
}

inline void TMR1Config18ms(void){
    T1CON = 0x30;       // '00110000'
}

inline void StartSeqDHT(){
    TMR1Config18ms();
    TMR2Config40us();
    DriveLowDHT();
    __delay_ms(18);
    ReleaseDHT();
    while(OUTDHT)
        ;
    while(!OUTDHT)
        ;
    while(OUTDHT)
        ;
}

inline void ReadBitDHT(uint8_t * c){
    TMR2 = 0;
    TMR2IF = 0;
    while(!OUTDHT)
        ;
    TMR2ON = 1;
    while(OUTDHT)
        ;
    TMR2ON = 0;
    *c = TMR2IF;
    TMR2IF = 0;
}

void MeasureDHT(void){
    uint8_t buff;
    uint8_t i = 40;
    StartSeqDHT();
    while(i){
        ReadBitDHT(&buff);
        DatasDHT[(i-1)>>3] <<= 1;
        DatasDHT[(i-1)>>3] += buff;
        i--;
    }
    
    // Checksum
      uint8_t check = DatasDHT[4] + DatasDHT[3] + DatasDHT[2] + DatasDHT[1];
      
      if(check != DatasDHT[0]){
          SETERDHT;
      }else{
          CLRERDHT;
      }
     
}

/******************************************************************************/
/* SIM800L ********************************************************************/
/******************************************************************************/
void SendCommandSIM(char * command){
    SendString("AT");
    SendString(command);
    SendString("\r\n");
}

void AutobaudSIM(void){
    SendString("AT\r\n");  
}

void SendSmsSIM(char * numero, char * message){       
    SendCommandSIM("+CMFG=1");
    __delay_ms(1000);
    SendCommandSIM("+CSMP=17,167,2,0");
    __delay_ms(1000);
    SendCommandSIM("+CSCS=\"GSM\"");
    __delay_ms(1000);
    SendString("AT+CMGS=\"");
    SendString(numero);
    SendString("\"\r\n");
    __delay_ms(1000);
    SendString(message);
    SendChar(CTRL_Z);
    //SendString("\r\n");
} 
