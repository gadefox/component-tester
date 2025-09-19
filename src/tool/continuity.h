/* ************************************************************************
 *
 *   continuity tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_CONTINUITY_CHECK

#ifndef CONTINUITY_H
#define CONTINUITY_H


#ifndef HW_BUZZER
#error <<< Continuity check tool requires buzzer >>>
#endif

#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_ESR

extern void ContinuityCheck(void);

extern const unsigned char ContinuityCheck_str[];


#endif // CONTINUITY_H

#endif // SW_CONTINUITY_CHECK
