/* ************************************************************************
 *
 *   square wave tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_SQUAREWAVE

#ifndef SQUARE_WAVE_H
#define SQUARE_WAVE_H


#if !defined(HW_ENCODER) && !defined(HW_INCDEC_KEYS) && !defined(HW_TOUCH)
#error <<< Square wave tool requires keys >>>
#endif

#define VAR_TIMER1_TABLES
#define FUNC_DISPLAY_FULLVALUE
#define FUNC_PROBE_PINOUT
#define SW_PROBEPINOUT_PWM

extern void SquareWave_SignalGenerator(void);

extern const unsigned char SquareWave_str[];


#endif // SQUARE_WAVE_H

#endif // SW_SQUAREWAVE
