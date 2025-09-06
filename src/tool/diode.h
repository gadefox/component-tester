/* ************************************************************************
 *
 *   diode tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef DIODE_TOOL_H
#define DIODE_TOOL_H

#ifdef SW_PHOTODIODE


#define FUNC_PROBE_PINOUT

extern void PhotodiodeCheck(void);

extern const unsigned char Photodiode_str[];
extern const unsigned char NoBias_str[];
extern const unsigned char ReverseBias_str[];


#endif // SW_PHOTODIODE


#ifdef SW_DIODE_LED

 
#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Diode_LED_Check(void);

extern const unsigned char Diode_LED_str[];


#endif // SW_DIODE_LED


#endif // DIODE_TOOL_H
