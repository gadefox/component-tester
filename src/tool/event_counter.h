/* ************************************************************************
 *
 *   event counter tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_EVENT_COUNTER

#ifndef EVENT_COUNTER_H
#define EVENT_COUNTER_H


#if !defined(HW_ENCODER) && !defined(HW_INCDEC_KEYS) && !defined(HW_TOUCH)
#error <<< Event counter tool requires keys >>>
#endif

#define VAR_START_STR
#define FUNC_DISPLAY_FULLVALUE
#define FUNC_SMOOTHLONGKEYPRESS

extern void EventCounter(void);

extern const unsigned char EventCounter_str[];
extern const unsigned char Count_str[];
extern const unsigned char Time_str[];
extern const unsigned char Events_str[];
extern const unsigned char Stop_str[];


#endif // EVENT_COUNTER_H

#endif // HW_EVENT_COUNTER
