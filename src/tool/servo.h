/* ************************************************************************
 *
 *   servo tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_SERVO

#ifndef SERVO_H
#define SERVO_H
 

#ifndef HW_KEYS
#error <<< Servo tool requires keys >>>
#endif


#define FUNC_DISPLAY_FULLVALUE
#define FUNC_SMOOTHLONGKEYPRESS
#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_PWM

extern const unsigned char Servo_str[];
extern const unsigned char Sweep_str[];

extern void Servo_Check(void);


#endif // SIGNAL_H

#endif // SW_SERVO
