/* ************************************************************************
 *
 *   SPI hardware
 *
 *   (c) 2017-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/*
 *  include header files
 */

#include "common.h"                /* common header file */


#ifdef SPI_HARDWARE

/*
 *  HINTs:
 *  - port and pins for bit-bang SPI
 *    SPI_PORT      port data register
 *    SPI_DDR       port data direction register
 *    SPI_PIN       port input pins register
 *    SPI_SCK       pin for SCK
 *    SPI_MOSI      pin for MOSI
 *    SPI_MISO      pin for MISO
 *  - For hardware SPI the MCU specific pins are used:
 *    ATmega 328: SCK PB5, MOSI PB3, MISO PB4, /SS PB2
 *    ATmega 644: SCK PB7, MOSI PB5, MISO PB6, /SS PB4
 *    ATmega 2560: SCK PB1, MOSI PB2, MISO PB3, /SS PB0
 *  - /CS and other control signals have to be managed by the specific
 *    chip driver
 *  - we use SPI mode 0 (set MOSI before rising SCK)
 */

/*
 *  local constants
 */

SPI_Type       SPI;       /* SPI */


/* ************************************************************************
 *   functions for hardware SPI
 * ************************************************************************ */

/*
 *  set SPI clock rate
 *  - uses ClockRate for input
 */

void SPI_Clock(void)
{
  uint8_t           Rate;     /* clock rate flags */
  uint8_t           Bits;     /* bitfield */

  Rate = SPI.ClockRate;           /* get clock rate flags */

  /*
   *  update clock rate divider
   */

  Bits = SPCR;                            /* get control register */
  Bits &= ~((1 << SPR1) | (1 << SPR0));   /* clear clock rate bits */

  /* set divider bits */
  if (Rate & SPI_CLOCK_R0)
    Bits |= (1 << SPR0);
  if (Rate & SPI_CLOCK_R1)
    Bits |= (1 << SPR1);

  SPCR = Bits;                     /* set new clock rate */

  /*
   *  update double-speed mode
   */

  Bits = 0;                        /* reset variable */

  if (Rate & SPI_CLOCK_2X)        /*  */
    Bits = (1 << SPI2X);           /* set bit to double SPI speed */

  SPSR = Bits;                     /* update register */
}


/*
 *  set up SPI bus
 *  - clock and mode
 *  - lines are set up automatically
 */

void SPI_Setup(void)
{
  uint8_t           Bits;     /* register bits */

  /* set up bus only once */
  if (Cfg.OP_Mode & OP_SPI)
    return;

  /* set SCK and MOSI to output mode */
  /* also /SS to keep SPI system in master mode */
  Bits = SPI_DDR;
  Bits |= (1 << SPI_SCK) | (1 << SPI_MOSI) | (1 << SPI_SS);
  SPI_DDR = Bits;

  /* MISO is automatically set to input mode by enabling SPI */

  /*
   *  set up hardware SPI
   *  - master mode (MSTR = 1)
   *  - SPI mode 0 (CPOL = 0, CPHA = 0)
   *  - MSB first (DORD = 0)
   *  - polling mode (SPIE = 0)
   */

  /* set mode and enable SPI */
  SPCR = (1 << SPE) | (1 << MSTR);

  /* set clock rate */
  SPI_Clock();

  /* clear SPI interrupt flag, just in case */
  Bits = SPSR;           /* read flag */
  Bits = SPDR;           /* clear flag by reading data */

  /* SPI bus is set up now */
  Cfg.OP_Mode |= OP_SPI;      /* set flag */
}


/*
 *  write a single byte
 *
 *  requires:
 *  - Byte: byte to write
 */

void SPI_Write_Byte(uint8_t Byte)
{
  /* send byte */
  SPDR = Byte;                     /* start transmission */
  while (!(SPSR & (1 << SPIF)));   /* wait for flag */
  Byte = SPDR;                     /* clear flag by reading data */
}


#ifdef SPI_RW

/*
 *  write and read a single byte
 *
 *  requires:
 *  - Byte: byte to write
 *
 *  returns:
 *  - byte read
 */

uint8_t SPI_WriteRead_Byte(uint8_t Byte)
{
  uint8_t           Byte2;         /* return value */

  /* send byte */
  SPDR = Byte;                     /* start transmission */
  while (!(SPSR & (1 << SPIF)));   /* wait for flag */

  /* get received byte */
  Byte2 = SPDR;                    /* also clears flag (by reading data) */

  return Byte2;
}


#endif // SPI_RW

#endif // SPI_HARDWARE
