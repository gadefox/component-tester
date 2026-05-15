/* ************************************************************************
 *
 *   MAX thermocouple functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined(HW_MAX31855) || defined(HW_MAX6675)

#ifndef MAX_H
#define MAX_H


#ifdef SPI_HARDWARE
/* SPI */
extern uint8_t             ClockRate;     /* SPI clock rate bits */
extern uint8_t             OldClockRate;  /* SPI clock rate bits */
#endif


#endif // MAX_H

#endif // HW_MAX31855 || HW_MAX6675
