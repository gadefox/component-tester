/* ************************************************************************
 *
 *   freq counter tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined(HW_FREQ_COUNTER_BASIC) || defined(HW_FREQ_COUNTER_EXT)

#ifndef FREQ_COUNTER_H
#define FREQ_COUNTER_H


#if defined(HW_FREQ_COUNTER_BASIC) && defined(HW_FREQ_COUNTER_EXT)
/* frequency counter: can't have both variants */
#error <<< Counter: select either basic or extended frequency counter! >>>
#endif

#define VAR_TIMER1_TABLES

extern void FrequencyCounter(void);

extern const unsigned char FreqCounter_str[];

#ifdef HW_FREQ_COUNTER_EXT

#define FUNC_DISPLAY_FULLVALUE

extern const unsigned char CounterChannel_str[];
extern const unsigned char FreqInput_str[];
extern const unsigned char LF_Crystal_str[];
extern const unsigned char HF_Crystal_str[];

#endif // HW_FREQ_COUNTER_EXT


#endif // FREQ_COUNTER_H

#endif // HW_FREQ_COUNTER_BASIC || HW_FREQ_COUNTER_EXT
