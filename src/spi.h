/* ************************************************************************
 *
 *   SPI functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SPI

#ifndef SPI_H
#define SPI_H


/* clock rate flags (bitfield) */
#define SPI_CLOCK_R0          0b00000001     /* divider bit 0 (SPR0) */
#define SPI_CLOCK_R1          0b00000010     /* divider bit 1 (SPR1) */
#define SPI_CLOCK_2X          0b00000100     /* double clock rate (SPI2X) */


#if defined(SPI_HARDWARE)

/* SPI_HARDWARE */
#ifdef SPI_BITBANG
/* SPI: either bit-bang or hardware */
#error <<< SPI: select either bitbang or hardware SPI! >>>
#endif // SPI_BITBANG

extern void SPI_Clock(void);

#elif defined(SPI_BITBANG)

#ifdef SPI_RW
/* bit-bang SPI with read support requires SPI_PIN and SPI_MISO */
#ifndef SPI_PIN
#error <<< SPI: bit-bang SPI with read support requires SPI_PIN to be set! >>>
#endif // SPI_PIN

#ifndef SPI_MISO
#error <<< SPI: bit-bang SPI with read support requires SPI_MISO to be set! >>>
#endif // SPI_MISO
#endif // SPI_RW

/* SPI_BITBANG */
#ifdef SPI_9
extern void SPI_Write_Bit(uint8_t Bit);
#endif // SPI_9

#elif defined(SPI_9)
/* 9-Bit SPI requires bit-bang mode */
#error <<< SPI: 9-Bit SPI requires bit-bang mode! >>>
#endif // SPI_HARDWARE || SPI_BIGBANG || SPI_9


extern void SPI_Setup(void);
extern void SPI_Write_Byte(uint8_t Byte);

#ifdef SPI_RW
extern uint8_t SPI_WriteRead_Byte(uint8_t Byte);
#else
#undef LCD_READ

/* options which require SPI read support */
#undef HW_MAX6675
#undef HW_MAX31855
#endif // SPI_RW


#ifdef SPI_HARDWARE
extern uint8_t         SPI_ClockRate;     /* clock rate bits */
#endif


#endif // SPI_H

#else

#ifdef SPI_RW
#error <<< SPI_RW requires SPI >>>
#endif

#endif // SPI
