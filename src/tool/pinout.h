/* ************************************************************************
 *
 *   probe pinout tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef FUNC_PROBE_PINOUT

#ifndef PROBE_PINOUT_H
#define PROBE_PINOUT_H

/* ************************************************************************
 *   constants for operation and UI
 * ************************************************************************ */

/* modes for probe pinout */
#define PROBES_PWM            0         /* PWM output */
#define PROBES_ESR            1         /* ESR measurement */
#define PROBES_RCL            2         /* monitoring RCL */
#define PROBES_RINGTESTER     3         /* ring tester */
#define PROBES_DIODE          4         /* diode */

extern void ProbePinout(uint8_t Mode);


#endif // PROBE_PINOUT_H

#endif // FUNC_PROBE_PINOUT
