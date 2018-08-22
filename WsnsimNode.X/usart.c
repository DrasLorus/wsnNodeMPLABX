/*
 * File:   usart.c
 * Author: Camille
 *
 * Created on 13 août 2018, 11:53
 */


#include <xc.h>
#include "usart.h"

void InitUsart(USART_BAUD baudrate){
    
    TXIE = 0;
    RCIE = 0;
    
    RCSTA = 0;
    TXSTA = 0;
    
    RX9   = 0;
    TX9   = 0;
    ADDEN = 0;
        
    TRISC = 0x80;
    
    switch (baudrate)
    {
        case USART_BAUD_MAXIMUM:
            SPBRG = SPBRG_AT_MAXIMUM;
            BRGH  = 1; /* Setting high speed */
            break;
        case USART_BAUD_115200:
            SPBRG = SPBRG_AT_115200;
            BRGH  = BRGH_AT_115200;
            break;
        case USART_BAUD_57600:
            SPBRG = SPBRG_AT_57600;
            BRGH  = BRGH_AT_57600;
            break;
        case USART_BAUD_38400:
            SPBRG = SPBRG_AT_38400;
            BRGH  = BRGH_AT_38400;
            break;
        case USART_BAUD_19200:
            SPBRG = SPBRG_AT_19200;
            BRGH  = BRGH_AT_19200;
            break;
        case USART_BAUD_9600:
            SPBRG = SPBRG_AT_9600;
            BRGH  = BRGH_AT_9600;
            break;
        /*case USART_BAUD_4800:
            SPBRG = SPBRG_AT_4800;
            BRGH  = BRGH_AT_4800;
            break;*/
        case USART_BAUD_2400:
            SPBRG = SPBRG_AT_2400;
            BRGH  = BRGH_AT_2400;
            break;
        /*case USART_BAUD_1200:
            SPBRG = SPBRG_AT_1200;
            BRGH  = BRGH_AT_1200;
            break;*/
        default:
            /* default is the slowest baud rate */
            SPBRG = 0xFF;
            BRGH = 0;
            break;
    }
    
    SYNC  = 0;
    
    CREN  = 1;
    TXEN  = 1;
    SPEN  = 1;   
    
    RCREG;
    RCREG;
    RCREG;
}

void SendChar(unsigned char c){
    while(!TX_RDY)
        ;
    TXREG = c;
}

void SendString(unsigned char * s){
    if(s){
        while(*s){
            SendChar(*s++);
        }
    }
}


uint8_t ReceiveChar(unsigned char * c){
    if(RX_RDY){
        uint8_t rxerr = 0;
        
        if(OERR){
            rxerr = 1;
            CREN = 0; /* reset receiver */
            CREN = 1;
            RCREG;
            RCREG;
            RCREG;
            if(OERRcounter < 255) 
                OERRcounter++;
        }
        
        if (FERR) {
            rxerr = 1;
            RCREG; /* Discard character with framing error */
            if(FERRcounter < 255) 
                FERRcounter++;
        } 
        
        if(!rxerr){
            *c = RCREG;
            return 1;
        }       
    }
    return 0;
    
}
