/* ************************************************************************
 *
 *   pwm simple tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_PWM_SIMPLE

#ifndef PWM_SIMPLE_H
#define PWM_SIMPLE_H


#ifdef SW_PWM_PLUS
/* PWM generators: can't have both variants */
#error <<< PWM: select either PWM generator with simple UI or fancy UI! >>>
#endif


/* number of entries in data tables */
#define NUM_PWM_FREQ          8         /* PWM frequencies */


#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_PWM

extern void PWM_Tool(uint16_t Frequency);

/* PWM menu: frequencies */
extern const uint16_t PWM_Freq_table[];

extern const unsigned char PWM_str[];


#endif // PWM_SIMPLE_H

#endif // SW_PWM_SIMPLE
