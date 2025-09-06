/* ************************************************************************
 *
 *   pwm plus tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_PWM_PLUS

#ifndef PWM_PLUS_H
#define PWM_PLUS_H


#ifdef SW_PWM_SIMPLE
/* PWM generators: can't have both variants */
#error <<< PWM: select either PWM generator with simple UI or fancy UI! >>>
#endif

#ifndef HW_KEYS
#error <<< PWM plus tool requires keys >>>
#endif

#define VAR_TIMER1_TABLES
#define FUNC_DISPLAY_FULLVALUE
#define FUNC_SMOOTHLONGKEYPRESS
#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_PWM

extern void PWM_Tool(void);

extern const unsigned char PWM_str[];


#endif // PWM_PLUS_H

#endif // SW_PWM_PLUS
