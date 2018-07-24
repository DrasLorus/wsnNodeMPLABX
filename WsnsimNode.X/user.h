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
#define ECHO RE1
#define TRIG RE0
#define SOUNDSPEED 340
#define OOR (flags & 0x01 == 0x01)  /* Out Of Range Flag*/

/* DS18B20 ********************************************************************/
#define SKIPROM         0xCC
#define CONVERTT        0x44
#define READSCRATCHPAD  0x4E


#define BAUDRATE 57600

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 /* I/O and Peripheral Initialization */

volatile char flags;

/* HY-SRF05 *******************************************************************/
void Trigger();                     /* Launch a measure */
int EchoDuration();                 /* Return the raw duration of the echo */
inline double CalcDistance(int time);        /* Return the distance in cm */
void MeasureHY();                    /* Launch the HY measurement routine */
double distance_cm;

/* DS18B20 ********************************************************************/
void InitializationSeqDS();
void Write1DS();
void Write0DS();

#endif //USER_H
