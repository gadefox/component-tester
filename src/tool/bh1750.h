/* ************************************************************************
 *
 *   BH1750 functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_BH1750

#ifndef BH1750_H
#define BH1750_H

#if (!defined(I2C_BITBANG) && !defined(I2C_HARDWARE)) || !defined(I2C_RW)
#error <<< BH1750 requires I2C_RW >>>
#endif

#define VAR_START_STR
#define FUNC_DISPLAY_FULLVALUE

extern void BH1750_Tool(void);

extern const unsigned char BH1750_str[];


#endif // BH1750_H

#endif // HW_BH1750
