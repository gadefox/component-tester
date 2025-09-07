/* ************************************************************************
 *
 *   SPI hardware functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SPI_HARDWARE

#ifndef SPI_H
#define SPI_H


#ifdef SPI_BITBANG
#error <<< SPI: select either bitbang or hardware SPI! >>>  /* SPI: either bit-bang or hardware */
#endif


/* clock rate flags (bitfield) */
#define SPI_CLOCK_R0          0b00000001     /* divider bit 0 (SPR0) */
#define SPI_CLOCK_R1          0b00000010     /* divider bit 1 (SPR1) */
#define SPI_CLOCK_2X          0b00000100     /* double clock rate (SPI2X) */


typedef struct
{
  uint8_t           ClockRate;     /* clock rate bits */
} SPI_Type;


extern void SPI_Clock(void);
extern void SPI_Setup(void);
extern void SPI_Write_Byte(uint8_t Byte);

#ifdef SPI_RW
extern uint8_t SPI_WriteRead_Byte(uint8_t Byte);
#endif // SPI_RW


extern SPI_Type       SPI;       /* SPI */


#endif // SPI_H

#endif // SPI_HARDWARE
