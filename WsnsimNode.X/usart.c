/*
 * File:   usart.c
 * Author: Camille
 *
 * Created on 13 août 2018, 11:53
 */


#include <xc.h>
#include "usart.h"

void ResetFifo(fifo * f){
    f->ir = 0;
    f->iw = 0;
}

uint8_t ReadFifo(fifo * f, char * c){
    if(f->iw == f->ir){
        return 0;
    }else{
        *c = f->str[f->ir++];
        if(f->ir > (FIFOSIZE - 1)){
            f->ir = 0;
        }
        return 1;
    }
}

uint8_t WriteFifo(fifo * f, char c){
    uint8_t b;
    b = (f->ir == 0) && (f->iw == FIFOSIZE - 1);
    if( b || ((f->iw + 1) == f->ir)){
        return 0;
    }else{ 
        f->str[f->iw++] = c;
        if(f->iw > FIFOSIZE - 1){
            f->iw = 0;
        }
        return 1;
    }
}

void InitUsart(uint16_t baudrate, bool hs){
    
    if(hs){
        BRGH  = 1;
        SPBRG = VALUEBRG1(baudrate);
    }else{
        BRGH  = 0;
        SPBRG = VALUEBRG0(baudrate);
    }
    
    TRISC = 0x80;
    
    SYNC  = 0;
    SPEN  = 1;
    
    TXEN  = 1;
    
    RCIE  = 1;
    
    CREN  = 1;
    
}

uint8_t SendChar(char c){
    if(TXIF){
        TXREG = c;
        return 1;
    }else{
        return 0;
    }
}

uint8_t SendString(char * s){
    uint8_t i = 0;
    while(*(s + i)){
        while(!TXIF)
            ;
        if(!SendChar(*(s + i)))
            return 0;
        i++;
    }
    return 1;
}

uint8_t ReceiveChar(char * c){
    if(RCIF){
        *c = RCREG;
        return 1;
    }else{
        return 0;
    }
}
