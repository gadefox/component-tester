/* ************************************************************************
 *
 *   counter tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined (HW_FREQ_COUNTER_BASIC) || defined (HW_FREQ_COUNTER_EXT) || defined (HW_EVENT_COUNTER) || defined (HW_LC_METER) || defined (HW_RING_TESTER)

#ifndef COUNTER_H
#define COUNTER_H


ISR(TIMER0_OVF_vect, ISR_BLOCK);

/* pulse counter */
#if defined (HW_FREQ_COUNTER_EXT) || defined (HW_EVENT_COUNTER) || defined (HW_LC_METER)
extern volatile uint32_t      Pulses;        /* number of pulses */
#elif defined (HW_FREQ_COUNTER_BASIC) || defined (HW_RING_TESTER)
extern volatile uint16_t      Pulses;        /* number of pulses */
#endif


#endif // COUNTER_H

#endif // HW_FREQ_COUNTER_BASIC || HW_FREQ_COUNTER_EXT || HW_EVENT_COUNTER || HW_LC_METER || HW_RING_TESTER
