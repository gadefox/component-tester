/* ************************************************************************
 *
 *   ring tester tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_RING_TESTER

#ifndef RING_TESTER_H
#define RING_TESTER_H


/* requires pulse output */
#if !defined (RING_TESTER_PIN) && !defined (RING_TESTER_PROBES)
#error <<< Ring tester: select pulse output! >>>
#endif

/* prefer dedicated pin */
#ifdef RING_TESTER_PIN
#undef RING_TESTER_PROBES
#endif

#ifdef RING_TESTER_PROBES
#define FUNC_PROBE_PINOUT
#endif

extern void RingTester(void);

extern const unsigned char RingTester_str[];


#endif // RING_TESTER_H


#endif // HW_RING_TESTER
