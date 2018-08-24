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
    CLRACK;
    
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
void Acknowledge(){
    while(!ACK)
        ;
    CLRACK;
}

void SendCommandSIM(char * command){
    SendString("AT");
    SendString(command);
    SendString("\r\n");
}

void AutobaudSIM(void){
    SendString("AT\r\n");
    Acknowledge();
}

void SendSmsSIM(char * numero, char * message){   
    CLRACK;
    SendCommandSIM("+CMGF=1");
    Acknowledge();       
    SendCommandSIM("+CSMP=17,167,2,0");
    Acknowledge();
    SendCommandSIM("+CSCS=\"GSM\"");
    Acknowledge();
    SendString("AT+CMGS=\"");
    SendString(numero);
    SendString("\"\r\n");   
    SendString(message);
    SendChar(CTRL_Z);
    Acknowledge();
} 

void Transcodemm(double d){
    if(d >= .9){                   
        sms[8] = '9';
    }else if(d >= .8){
        sms[8] = '8'; 
    }else if(d >= .7){
        sms[8] = '7'; 
    }else if(d >= .6){
        sms[8] = '6'; 
    }else if(d >= .5){
        sms[8] = '5'; 
    }else if(d >= .4){
        sms[8] = '4'; 
    }else if(d >= .3){
        sms[8] = '3'; 
    }else if(d >= .2){
        sms[8] = '2'; 
    }else if(d >= .1){
        sms[8] = '1'; 
    }else{
        sms[8] = '0'; 
   }
}

void Transcodecm(double * temp){
    if(*temp >= 9.0){
        *temp -= 9.0;
        sms[6] = '9';
        Transcodemm(*temp);           
    }else if(*temp >= 8.0){
        *temp -= 8.0;
        sms[6] = '8';
        Transcodemm(*temp);
    }else if(*temp >= 7.0){
        *temp -= 7.0;
        sms[6] = '7';
        Transcodemm(*temp); 
    }else if(*temp >= 6.0){
        *temp -= 6.0;
        sms[6] = '6';
        Transcodemm(*temp);
    }else if(*temp >= 5.0){
        *temp -= 5.0;
        sms[6] = '5';
        Transcodemm(*temp);               
    }else if(*temp >= 4.0){
        *temp -= 4.0;
        sms[6] = '4';
        Transcodemm(*temp);               
    }else if(*temp >= 3.0){
        *temp -= 3.0;
        sms[6] = '3';
        Transcodemm(*temp);               
    }else if(*temp >= 2.0){
        *temp -= 2.0;
        sms[6] = '2';
        Transcodemm(*temp);               
    }else if(*temp >= 1.0){
        *temp -= 1.0;
        sms[6] = '1';
        Transcodemm(*temp);               
    }else{
        sms[6] = '0';
        Transcodemm(*temp);               
    }
}

void Transcodedm(double * temp){
    if(*temp >= 90.0){
        *temp -= 90.0;
        sms[5] = '9';
        Transcodecm(temp);           
    }else if(*temp >= 80.0){
        *temp -= 80.0;
        sms[5] = '8';
        Transcodecm(temp);
    }else if(*temp >= 70.0){
        *temp -= 70.0;
        sms[5] = '7';
        Transcodecm(temp); 
    }else if(*temp >= 60.0){
        *temp -= 60.0;
        sms[5] = '6';
        Transcodecm(temp);
    }else if(*temp >= 50.0){
        *temp -= 50.0;
        sms[5] = '5';
        Transcodecm(temp);               
    }else if(*temp >= 40.0){
        *temp -= 40.0;
        sms[5] = '4';
        Transcodecm(temp);               
    }else if(*temp >= 30.0){
        *temp -= 30.0;
        sms[5] = '3';
        Transcodecm(temp);               
    }else if(*temp >= 20.0){
        *temp -= 20.0;
        sms[5] = '2';
        Transcodecm(temp);               
    }else if(*temp >= 10.0){
        *temp -= 10.0;
        sms[5] = '1';
        Transcodecm(temp);               
    }else{
        sms[5] = '0';
        Transcodecm(temp);               
    }
}

void TranscodeOthersUnit(signed char data, uint8_t i){
    if(data >= 9){                   
        sms[i] = '9';
    }else if(data >= 8){
        sms[i] = '8'; 
    }else if(data >= 7){
        sms[i] = '7'; 
    }else if(data >= 6){
        sms[i] = '6'; 
    }else if(data >= 5){
        sms[i] = '5'; 
    }else if(data >= 4){
        sms[i] = '4'; 
    }else if(data >= 3){
        sms[i] = '3'; 
    }else if(data >= 2){
        sms[i] = '2'; 
    }else if(data >= 1){
        sms[i] = '1'; 
    }else{
        sms[8] = '0'; 
   }
}

void TranscodeOthersTenth(signed char * data, uint8_t i){
    if(*data >= 90){
        *data -= 90;
        sms[i] = '9';
        TranscodeOthersUnit(*data, i + 1);
    }else if(*data >= 80){
        *data -= 80;
        sms[i] = '8';
        TranscodeOthersUnit(*data, i + 1);
    }else if(*data >= 70){
        *data -= 70;
        sms[i] = '7';
        TranscodeOthersUnit(*data, i + 1);
    }else if(*data >= 60){
        *data -= 60;
        sms[i] = '6';
        TranscodeOthersUnit(*data, i + 1);
    }else if(*data >= 50){
        *data -= 50;
        sms[i] = '5';
        TranscodeOthersUnit(*data, i + 1);               
    }else if(*data >= 40){
        *data -= 40;
        sms[i] = '4';
        TranscodeOthersUnit(*data, i + 1);            
    }else if(*data >= 30){
        *data -= 30;
        sms[i] = '3';
        TranscodeOthersUnit(*data, i + 1);             
    }else if(*data >= 20){
        *data -= 20;
        sms[i] = '2';
        TranscodeOthersUnit(*data, i + 1);           
    }else if(*data >= 10){
        *data -= 10;
        sms[i] = '1';
        TranscodeOthersUnit(*data, i + 1);              
    }else{
        sms[i] = '0';
        TranscodeOthersUnit(*data, i + 1);               
    }
}

void ConvertMeasureToStr(){

    //HYSRF05
    double d;
    d = distance_cm;
    if(d >= 400.0){           // 400cm is considered as the maximum range.
        sms[4] = '4';
        sms[5] = '0';
        sms[6] = '0';
        sms[8] = '0';
    }else if(d >= 300.0){
        d -= 300.0;
        sms[4] = '3';
        Transcodedm(&d);
    }else if(d >= 200.0){
        d -= 200.0;
        sms[4] = '2';
        Transcodedm(&d);
    }else if(d >= 100.0){
        d -= 100.0;
        sms[4] = '1';
        Transcodedm(&d);
    }else{
        sms[4] = ' ';
        Transcodedm(&d);
    }
    
    //DS    
    signed char tds = 0;
    tds = ((temperatureDS[1] & 0x0F) << 4) + ((temperatureDS[0] & 0xF0) >> 4);
    if(tds < 0){
        tds = ~tds + 1;
        sms[DS_H]= '-';
        TranscodeOthersTenth(&tds, DS_T);
    }else if(tds > 100){
        tds -= 100;
        sms[DS_H]= '1';
        TranscodeOthersTenth(&tds, DS_T);
    }else{
        sms[DS_H]= ' ';
        TranscodeOthersTenth(&tds, DS_T);
    }
    
    //DHT11_T
    tds = DatasDHT[2];
    if(tds < 0){
        tds = ~tds + 1;
        sms[DHT_T_H]= '-';
        TranscodeOthersTenth(&tds, DHT_T_T);
    }else if(tds > 100){
        tds -= 100;
        sms[DHT_T_H]= '1';
        TranscodeOthersTenth(&tds, DHT_T_T);
    }else{
        sms[DHT_T_H]= ' ';
        TranscodeOthersTenth(&tds, DHT_T_T);
    }
    
    //DHT11_RH
    tds = DatasDHT[4];
    TranscodeOthersTenth(&tds, DHT_RH_T);
    //TODO
}
