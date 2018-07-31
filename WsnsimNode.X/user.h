
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#ifndef USER_H
#define USER_H

#include <xc.h>

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>  
#include "system.h"

/* HY-SRF05 *******************************************************************/
#define ECHO            RE1
#define TRIG            RE0
#define SOUNDSPEED      340
#define OOR             (flags & 0x01 == 0x01)  /* Out Of Range Flag*/
#define SETOOR          flags = (flags & 0xFE) + 0x1
#define CLROOR          flags = flags & 0xFE

/* DS18B20 ********************************************************************/
#define OUTDS           RB2
#define SKIPROM         0xCC
#define CONVERTT        0x44
#define READSCRATCHPAD  0xBE
#define EROI            (flags & 0x02 == 0x02)  /* ERror Of Initialization */
#define SETEROI         flags = (flags & 0xFD) + 0x2
#define CLREROI         flags = flags & 0xFD

/* DHT11 **********************************************************************/
#define OUTDHT          RB0
#define ERDHT           (flags & 0x04 == 0x04)  /* ERror DHT */
#define SETERDHT        flags = (flags & 0xFB) + 0x4
#define CLRERDHT        flags = flags & 0xFB

/* SIM800L ********************************************************************/
#define TXSIM           RC6
#define RXSIM           RC7
#define CRCD            (flags & 0x08 == 0x08)  /* Character ReCeiveD */
#define SETCRCD         flags = (flags & 0xF7) + 0x08
#define CLRCRCD         flags = flags & 0xF7
#define STRSIZE         16
#define BAUDRATE        9600

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 /* I/O and Peripheral Initialization */

volatile char flags;

/* HY-SRF05 *******************************************************************/
void TriggerHY(void);                   /* Launch a measure */

int EchoDuration(void);                 /* Return the raw duration of the echo */

double CalcDistance(int time);          /* Return the distance in cm */

void MeasureHY(void);                   /* Launch the HY measurement routine */

volatile double distance_cm;

/* DS18B20 ********************************************************************/
void InitializationSeqDS(void);

void Write1DS(void);

void Write0DS(void);

void SendInstructionDS(char c);

void SkipRom(void);

void ConvertT(void);

void ReadTemperature(void);

char ReadDS(void);

void MeasureDS(void);

volatile char temperatureDS[2];

/* DHT11 **********************************************************************/
inline void StartSeqDHT(void);

inline void ReadBitDHT(char * c);

void MeasureDHT(void);

volatile char DatasDHT[5];

/* SIM800L ********************************************************************/
void SendCharSIM(char c);

void ReceiveCharSIM(char * c);

void ReceiveStringSIM(char c[]);

void SendStringSIM(char c[], uint8_t size);

void SyncPicSIM(void);

volatile char bufferSIM;

volatile char stringSIM[STRSIZE];

#endif //USER_H
