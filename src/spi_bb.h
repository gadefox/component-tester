/* ************************************************************************
 *
 *   SPI bitbang functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SPI_BITBANG

#ifndef SPI_H
#define SPI_H


#ifdef SPI_HARDWARE
#error <<< SPI: select either bitbang or hardware SPI! >>>  /* SPI: either bit-bang or hardware */
#endif


#ifdef SPI_RW

/* bit-bang SPI with read support requires SPI_PIN and SPI_MISO */
#ifndef SPI_PIN
#error <<< SPI: bit-bang SPI with read support requires SPI_PIN to be set! >>>
#endif // SPI_PIN

#ifndef SPI_MISO
#error <<< SPI: bit-bang SPI with read support requires SPI_MISO to be set! >>>
#endif // SPI_MISO

#endif // SPI_RW


/* clock rate flags (bitfield) */
#define SPI_CLOCK_R0          0b00000001     /* divider bit 0 (SPR0) */
#define SPI_CLOCK_R1          0b00000010     /* divider bit 1 (SPR1) */
#define SPI_CLOCK_2X          0b00000100     /* double clock rate (SPI2X) */


extern void SPI_Setup(void);
extern void SPI_Write_Byte(uint8_t Byte);

#ifdef SPI_RW
extern uint8_t SPI_WriteRead_Byte(uint8_t Byte);
#endif // SPI_RW

#ifdef SPI_9
extern void SPI_Write_Bit(uint8_t Bit);
#endif // SPI_9


#endif // SPI_H

#else

#ifdef SPI_9
#error <<< SPI: 9-Bit SPI requires bit-bang mode! >>>
#endif

#endif // SPI_BITBANG
