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
    f->elts = 0;
}

uint8_t ReadFifo(fifo * f, char * c){
    if(f->elts == 0){
        return 0;
    }else{
        *c = f->str[f->ir];
        f->elts--;
        if(f->ir > (FIFOSIZE - 2)){
            f->ir = 0;
        }else{
            f->ir += 1;
        }
        return 1;
    }
}

uint8_t WriteFifo(fifo * f, char c){
    if(f->elts == FIFOSIZE){
        return 0;
    }else{ 
        f->str[f->iw] = c;
        f->elts++;
        if(f->iw > FIFOSIZE - 2){
            f->iw = 0;
        }else{
            f->iw += 1;
        }
        return 1;
    }
}

void InitUsart(uint8_t baudrate, bool hs){
    
    if(hs){
        BRGH  = 1;
        SPBRG = VALUEBRG1(baudrate);
    }else{
        BRGH  = 0;
        SPBRG = VALUEBRG0(baudrate);
    }
    
    TRISC |= 0x80;
    
    SYNC  = 0;
    SPEN  = 1;
    
    TXEN  = 1;
    
    RCIE  = 1;
    
    CREN  = 1;
    
}

void ReceiveChar(char * c){
    *c = RCREG;
}
