#pragma once

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#ifndef USER_H
#define USER_H
/* TODO Application specific user parameters used in user.c may go here */
#include <xc.h>

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>  
#include "system.h"

/* HY-SRF05 *******************************************************************/
#define ECHO            RE1
#define TRIG            RE0
#define SOUNDSPEED      340
#define SETOOR          flags = (flags & 0xFE) + 0x1
#define OOR             (flags & 0x01 == 0x01)  /* Out Of Range Flag*/
#define CLROOR          flags = flags & 0xFE

/* DS18B20 ********************************************************************/
#define OUTDS           RB2
#define SKIPROM         0xCC
#define CONVERTT        0x44
#define READSCRATCHPAD  0xBE
#define EROI            (flags & 0x02 == 0x02)
#define SETEROI         flags = (flags & 0xFD) + 0x2
#define CLREROI         flags = flags & 0xFD

#define BAUDRATE        57600

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 /* I/O and Peripheral Initialization */

volatile char flags;

/* HY-SRF05 *******************************************************************/
void TriggerHY(void);                     /* Launch a measure */
int EchoDuration(void);                 /* Return the raw duration of the echo */
double CalcDistance(int time);        /* Return the distance in cm */
void MeasureHY(void);                    /* Launch the HY measurement routine */

volatile double distance_cm;

/* DS18B20 ********************************************************************/
void InitializationSeqDS(void);
void Write1DS(void);
void Write0DS(void);
void SendInstructionDS(char c);
void SkipRom(void);
void ConvertT(void);
void ReadTemperature(char c[]);
void ReadDS(char * c);
void MeasureDS(void);

char temperatureDS[2];
char bufferDS;

#endif //USER_H
