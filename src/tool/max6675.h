/* ************************************************************************
 *
 *   MAX6675 functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_MAX6675

#ifndef MAX6675_H
#define MAX6675_H


#define VAR_START_STR
#define FUNC_DISPLAY_FULLVALUE

#ifdef UI_FAHRENHEIT
#define FUNC_CELSIUS2FAHRENHEIT
#endif

extern void MAX6675_BusSetup(void);
extern void MAX6675_Tool(void);

extern const unsigned char MAX6675_str[];


#endif // MAX6675_H

#endif // HW_MAX6675
