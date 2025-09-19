/* ************************************************************************
 *
 *   counter tools (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

/* local includes */
#include "../common.h"                /* common header file */


/*
 *  local constants
 */


/*
 *  local variables
 */

/* pulse counter */
#if defined (HW_FREQ_COUNTER_EXT) || defined (HW_EVENT_COUNTER) || defined (HW_LC_METER)
volatile uint32_t      Pulses;        /* number of pulses */
#elif defined (HW_FREQ_COUNTER_BASIC) || defined (HW_RING_TESTER)
volatile uint16_t      Pulses;        /* number of pulses */
#endif


/* ************************************************************************
 *   shared ISRs for counter tools
 *   - also used by Get_LC_Frequency() in tools_LC_Meter.c
 * ************************************************************************ */

#if defined (HW_FREQ_COUNTER_BASIC) || defined (HW_FREQ_COUNTER_EXT) || defined (HW_EVENT_COUNTER) || defined (HW_LC_METER) || defined (HW_RING_TESTER)

/*
 *  ISR for overflow of Timer0
 *  - catch overflows of pulse counter 
 */

ISR(TIMER0_OVF_vect, ISR_BLOCK)
{
  /*
   *  HINTs:
   *  - the TOV0 interrupt flag is cleared automatically
   *  - interrupt processing is disabled while this ISR runs
   *    (no nested interrupts)
   */

  Pulses += 256;              /* add overflow to global counter */
}

#endif // HW_FREQ_COUNTER_BASIC || HW_FREQ_COUNTER_EXT || HW_EVENT_COUNTER || HW_LC_METER || HW_RING_TESTER


#if defined (HW_FREQ_COUNTER_BASIC) || defined (HW_FREQ_COUNTER_EXT) || defined (HW_LC_METER) || defined (HW_RING_TESTER)

/*
 *  ISR for match of Timer1's OCR1A (Output Compare Register A)
 *  - for gate time of frequency counter
 */

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
  /*
   *  hints:
   *  - the OCF1A interrupt flag is cleared automatically
   *  - interrupt processing is disabled while this ISR runs
   *    (no nested interrupts)
   */

  /* gate time has passed */
  TCCR1B = 0;                 /* disable Timer1 */
  TCCR0B = 0;                 /* disable Timer0 */

  /* break TestKey() processing */
  Cfg.OP_Control |= OP_BREAK_KEY;       /* set break signal */
}

#endif // HW_FREQ_COUNTER_BASIC || HW_FREQ_COUNTER_EXT || HW_LC_METER || HW_RING_TESTER
