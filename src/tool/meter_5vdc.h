/* ************************************************************************
 *
 *   meter 5V DC tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_METER_5VDC

#ifndef METER_5VDC_H
#define METER_5VDC_H


#define FUNC_PROBE_PINOUT
#define FUNC_SMOOTHLONGKEYPRESS
#define SW_PROBEPINOUT_ESR

extern void Meter_5VDC(void);

extern const unsigned char Meter_5VDC_str[];


#endif // METER_5VDC_H

#endif // SW_METER_5VDC
