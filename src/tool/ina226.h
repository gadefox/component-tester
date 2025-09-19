/* ************************************************************************
 *
 *   INA226 functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_INA226

#ifndef INA226_H
#define INA226_H

#if (!defined(I2C_BITBANG) && !defined(I2C_HARDWARE)) || !defined(I2C_RW)
#error <<< INA226 requires I2C_RW >>>
#endif

#if defined(INA226_POWER_ALARM) && !defined(HW_BUZZER)
#error <<< Continuity check tool requires buzzer >>>
#endif

#define VAR_START_STR

extern void INA226_Tool(void);

extern const unsigned char INA226_str[];


#endif // INA226_H

#endif // HW_226
