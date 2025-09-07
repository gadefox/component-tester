/* ************************************************************************
 *
 *   MAX31855 functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_MAX31855

#ifndef MAX31855_H
#define MAX31855_H


#define VAR_START_STR
#define FUNC_DISPLAY_SIGNEDFULLVALUE

#ifdef UI_FAHRENHEIT
#define FUNC_CELSIUS2FAHRENHEIT
#endif

extern void MAX31855_BusSetup(void);
extern void MAX31855_Tool(void);

extern const unsigned char MAX31855_str[];


#endif // MAX31855_H

#endif // HW_MAX31855
