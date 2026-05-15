/* ************************************************************************
 *
 *   cap tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef CAP_TOOL_H
#define CAP_TOOL_H

#ifdef SW_ESR_TOOL


#if !defined(SW_ESR) && !defined(SW_OLD_ESR)
#error <<< ESR tool requires ESR measurement >>>
#endif

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_ESR

extern void ESR_Tool(void);

extern const unsigned char ESR_str[];


#endif // SW_ESR_TOOL


#ifdef SW_CAP_LEAKAGE

extern void Cap_Leakage(void);

extern const unsigned char CapLeak_str[];
extern const unsigned char CapCharge_str[];
extern const unsigned char CapHigh_str[];
extern const unsigned char CapLow_str[];
extern const unsigned char CapDischarge_str[];

#endif // SW_CAP_LEAKAGE


#endif // CAP_TOOL_H
