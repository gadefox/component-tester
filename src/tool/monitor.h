/* ************************************************************************
 *
 *   monitor tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef MONITOR_H
#define MONITOR_H


#if !defined(SW_INDUCTOR) && (defined(SW_MONITOR_L) || defined(SW_MONITOR_RCL) || defined(SW_MONITOR_RL))
#error <<< Monitor RCL requires inductance measurement >>>
#endif


/* ************************************************************************
 *   constants for operation and UI
 * ************************************************************************ */

#ifdef SW_MONITOR_R

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Monitor_R(void);

extern const unsigned char Monitor_R_str[];

#endif // SW_MONITOR_R


#ifdef SW_MONITOR_C

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Monitor_C(void);

extern const unsigned char Monitor_C_str[];

#endif // SW_MONITOR_C


#ifdef SW_MONITOR_L

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Monitor_L(void);

extern const unsigned char Monitor_L_str[];

#endif // SW_MONITOR_L


#ifdef SW_MONITOR_RCL

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Monitor_RCL(void);

extern const unsigned char Monitor_RCL_str[];

#endif // SW_MONITOR_RCL


#ifdef SW_MONITOR_RL

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_RCL

extern void Monitor_RL(void);

extern const unsigned char Monitor_RL_str[];

#endif // SW_MONITOR_RL


#endif // MONITOR_H
