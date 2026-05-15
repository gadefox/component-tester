/* ************************************************************************
 *
 *   support for global configuration
 *
 *   (c) 2012-2025 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef CONFIG_ATMEGA_H
#define CONFIG_ATMEGA_H


/* ************************************************************************
 *   MCU specific setup to support different AVRs
 * ************************************************************************ */

/* estimated internal resistance of port to GND (in 0.1 Ohms) */
#define R_MCU_LOW           200  /* 209 */

/* estimated internal resistance of port to VCC (in 0.1 Ohms) */
#define R_MCU_HIGH          220  /* 235 */

/* voltage offset of MCU's analog comparator (in mV): -50 up to 50 */
#define COMPARATOR_OFFSET   0


/* ************************************************************************
 *   internal stuff
 * ************************************************************************ */

/* ADC reference selection: AVcc */
#define ADC_REF_VCC           (1 << REFS0)

/* ADC reference selection: filter mask for register bits */
#define ADC_REF_MASK          ((1 << REFS1) | (1 << REFS0))


/* ************************************************************************
 *   ADC clock
 * ************************************************************************ */

/*
 *  ADC clock 
 *  - The ADC clock is 125000Hz by default.
 *  - You could also set 250000Hz, but that exceeds the max. ADC clock
 *    of 200kHz for 10 bit resolution!
 *  - Special case for 20MHz MCU clock: 156250Hz
 */

#if F_CPU == 20000000
  /* 20MHz MCU clock */
  #define ADC_FREQ    156250
#elif F_CPU < 20000000
  /* all other MCU clocks */
  #define ADC_FREQ    125000
#else
  #error <<< No or wrong MCU type selected! >>>
#endif // F_CPU


#endif // CONFIG_ATMEGA_H
