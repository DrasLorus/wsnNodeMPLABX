/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: usart.h
 * Author: Camille Monière
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <stdint.h>
#include <stdbool.h>

#ifndef SYSTEM_H

#define SYS_FREQ      20000000

#else

#include "system.h"

#endif

// TODO Insert declarations

#define FIFOSIZE        32
#define VALUEBRG0(br)   (uint8_t)(SYS_FREQ/(64*(br+1)))
#define VALUEBRG1(br)   (uint8_t)(SYS_FREQ/(16*(br+1)))

typedef struct FIFO {
    char str[FIFOSIZE];
    uint8_t iw;
    uint8_t ir;
    uint8_t elts;
} fifo;

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype: void InitUsart(uint8_t baudrate);</b></p>
  
    <p><b>Summary: Initialyse the transiever and the baudrate generator </b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
**/
void InitUsart(uint8_t baudrate, bool hs);

/**
    <p><b>Function prototype: void ResetFifo(fifo * f);</b></p>
  
    <p><b>Summary: Initialyse the transiever and the baudrate generator </b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
**/
void ResetFifo(fifo * f);

uint8_t ReadFifo(fifo * f, char * c);

uint8_t WriteFifo(fifo * f, char c);

uint8_t SendChar(char c);

/**
    <p><b>Function prototype: void ReceiveChar(char * c);</b></p>
  
    <p><b>Summary: Initialyse the transiever and the baudrate generator </b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
**/
void ReceiveChar(char * c);

#endif	/* XC_HEADER_TEMPLATE_H */

