/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H
/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        20000000
#define FCY             SYS_FREQ/4
#define _XTAL_FREQ SYS_FREQ

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

// void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

#endif //SYSTEM_H
