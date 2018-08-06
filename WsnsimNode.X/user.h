
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#ifndef USER_H
#define USER_H

#include <xc.h>

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>  

#include "system.h"

#define TESTPICm    -1
#define IDLEm       0
#define SLEEPm      1
#define MEASUREm    2
#define TRANSMITm   3
#define COMPLETEm   4

#define DEFAULT     TESTPICm

#define FF              (flags.ff)             // Fifo Full flag
#define SETFF           flags.ff = 1
#define CLRFF           flags.ff = 0
#define FE              (flags.fe)             // Fifo Empty flag
#define SETFE           flags.fe = 1
#define CLRFE           flags.fe = 0
#define UER             (flags.fe)             // Unknown ERror flag
#define SETUER          flags.fe = 1
#define CLRUER          flags.fe = 0

/* HY-SRF05 *******************************************************************/
#define ECHO            RE1
#define TRIG            RE0
#define SOUNDSPEED      340
#define OOR             (flags.oor)             // Out Of Range flag
#define SETOOR          flags.oor = 1
#define CLROOR          flags.oor = 0

/* DS18B20 ********************************************************************/
#define OUTDS           RB2
#define SKIPROM         0xCC
#define CONVERTT        0x44
#define READSCRATCHPAD  0xBE
#define EROI            (flags.eroi)            // ERror Of Initialization 
#define SETEROI         flags.eroi = 1
#define CLREROI         flags.eroi = 0

/* DHT11 **********************************************************************/
#define OUTDHT          RB0
#define ERDHT           (flags.erdht)           // ERror DHT 
#define SETERDHT        flags.erdht = 1
#define CLRERDHT        flags.erdht = 0

/* SIM800L ********************************************************************/
#define TXSIM           RC6
#define RXSIM           RC7
#define FIFOSIZE        32
#define BAUDRATE        9600

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 // I/O and Peripheral Initialization 

struct flag_struct{
    uint8_t oor:1;
    uint8_t eroi:1;
    uint8_t erdht:1;
    uint8_t ff:1;
    uint8_t fe:1;
    uint8_t uer:1;
} flags;

typedef struct FIFO {
    char str[FIFOSIZE];
    uint8_t iw;
    uint8_t ir;
    uint8_t elts;
} fifo;

void ResetFifo(fifo * f);

uint8_t ReadFifo(fifo * f, char * c);

uint8_t WriteFifo(fifo * f, char c);

fifo bufferSIM;

/* HY-SRF05 *******************************************************************/
void TriggerHY(void);                   // Launch a measure 

int EchoDuration(void);                 // Return the raw duration of the echo 

double CalcDistance(int time);          // Return the distance in cm 

void MeasureHY(void);                   // Launch the HY measurement routine 

volatile double distance_cm;

/* DS18B20 ********************************************************************/
void InitializationSeqDS(void);

void Write1DS(void);

void Write0DS(void);

void SendInstructionDS(uint8_t c);

void SkipRom(void);

void ConvertT(void);

void ReadTemperature(void);

uint8_t ReadDS(void);

void MeasureDS(void);

volatile uint8_t temperatureDS[2];

/* DHT11 **********************************************************************/
inline void StartSeqDHT(void);

inline void ReadBitDHT(uint8_t * c);

void MeasureDHT(void);

volatile uint8_t DatasDHT[5];

/* SIM800L ********************************************************************/

uint8_t SyncPicSIM(void);

void ReceiveCharSIM(fifo * f);

uint8_t ReceiveStringSIM(fifo * f, char s[],  uint8_t size);

void SendCommandSIM(char * command);

#endif //USER_H
