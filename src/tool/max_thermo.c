/* ************************************************************************
 *
 *   common definitions for MAXxx (thermocouple ADC, SPI bus)
 *
 *   (c) 2021-2024 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#if defined(HW_MAX31855) || defined(HW_MAX6675)

/*
 *  local variables
 */

#ifdef SPI_HARDWARE
/* SPI */
uint8_t             ClockRate;     /* SPI clock rate bits */
uint8_t             OldClockRate;  /* SPI clock rate bits */
#endif


#endif // HW_MAX31855 || HW_MAX6675
