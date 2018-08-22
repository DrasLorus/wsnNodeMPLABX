
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#ifndef USER_H
#define USER_H

#include <xc.h>

#include <stdint.h>         /* For uint8_t definition */
 
#include "system.h"
#include "usart.h"

#define TESTPICm    -1
#define IDLEm       0
#define SLEEPm      1
#define MEASUREm    2
#define TRANSMITm   3
#define COMPLETEm   4

#define DEFAULT     TESTPICm

#define UER             (flags.uer)             // Unknown ERror flag
#define SETUER          flags.uer = 1
#define CLRUER          flags.uer = 0

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
#define CR              0xd //0x0D
#define LF              0xa //0x0A
#define SUB             26
#define CTRL_Z          SUB
#define ESC             27

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 // I/O and Peripheral Initialization 

typedef struct flag_struct{
    uint8_t oor:1;
    uint8_t eroi:1;
    uint8_t erdht:1;
    uint8_t uer:1;
} flag_t;

volatile flag_t flags;

/* HY-SRF05 *******************************************************************/
void MeasureHY(void);                   // Launch the HY measurement routine 

volatile double distance_cm;            // The result of the HY measure

/* DS18B20 ********************************************************************/
void ResetDS(void);                     // Cancel all unfinished previous actions, could cause dataloss

void SendInstructionDS(uint8_t c);      // Launch the DS specific sending routine

void SkipRom(void);                     // Allow to skip the rom step

void ConvertT(void);                    // Launch a temperature measure

void ReadTemperature(void);             // Execute READSCRATCHPAD and store the result

uint8_t ReadDS(void);                   // Return the value given by the DS18B20

void MeasureDS(void);                   // Launch the DS measurement routine

volatile uint8_t temperatureDS[2];      // The result of the DS measure

/* DHT11 **********************************************************************/
void MeasureDHT(void);                  // Launch the DHT measurement routine

volatile uint8_t DatasDHT[5];           // The result of the DHT measure

/* SIM800L ********************************************************************/

void AutobaudSIM(void);

void SendCommandSIM(char * command);

void SendSmsSIM(char * numero, char * message);

#endif //USER_H
