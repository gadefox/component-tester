/* ************************************************************************
 *
 *   LC meter functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_LC_METER

#ifndef LC_METER_H
#define LC_METER_H


#define VAR_TIMER1_TABLES
#define FUNC_SMOOTHLONGKEYPRESS

#ifdef LC_METER_SHOW_FREQ
#define FUNC_DISPLAY_FULLVALUE
#endif

extern uint8_t LC_Meter(void);

extern const unsigned char LC_Meter_str[];
extern const unsigned char Adjusting_str[];


#endif // LC_METER_H

#endif // HW_LC_METER
