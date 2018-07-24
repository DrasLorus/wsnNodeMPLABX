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

/** HY-SRF05 ******************************************************************/
#define ECHO RE1
#define TRIG RE0
#define SOUNDSPEED 340

#define BAUDRATE 57600

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 /* I/O and Peripheral Initialization */

/* HY-SRF05 *******************************************************************/
void Trigger();                     /* Launch a measure */
int EchoDuration();                 /* Return the raw duration of the echo */
inline double CalcDistance(int time);        /* Return the distance in cm */
void MeasureHY();                    /* Launch the HY measurement routine */
double distance_cm;

#endif //USER_H
