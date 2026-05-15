/* ************************************************************************
 *
 *   SPI bit-bang
 *
 *   (c) 2017-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/*
 *  include header files
 */

#include "common.h"                /* common header file */


#ifdef SPI_BITBANG

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


/* ************************************************************************
 *   functions for bit-bang SPI
 * ************************************************************************ */

/*
 *  set up SPI bus
 *  - SCK, MOSI and MISO lines
 *  - don't care about clock rate
 */

void SPI_Setup(void)
{
  /* set up bus only once */
  if (Cfg.OP_Mode & OP_SPI)
    return;

  /*
   *  set up bitbang SPI:
   *  - master mode
   */

#ifdef SPI_RW
  /* set MISO line to input mode */
  SPI_DDR &= ~(1 << SPI_MISO);
#endif

  /* set SCK and MOSI to output mode */
  SPI_DDR |= (1 << SPI_SCK) | (1 << SPI_MOSI);

  /* preset lines to low */
  SPI_PORT &= ~((1 << SPI_SCK) | (1 << SPI_MOSI));

  /* bus is set up now */
  Cfg.OP_Mode |= OP_SPI;      /* set flag */
}


#ifdef SPI_9

/*
 *  write a single bit
 *  - for displays supporting D/C control via SPI
 *    (3-line SPI with 9 bit frames)
 *
 *  requires:
 *  - Bit: bit to write
 */

void SPI_Write_Bit(uint8_t Bit)
{
  /*
   *  expected state:
   *  - SCK low
   *  - MOSI undefined
   */

  /*
   *  bitbang 1 bit:
   *  - simulate SPI mode 0 (CPOL = 0, CPHA = 0)
   *    - set MOSI before rising SCK
   */

  /* set MOSI based on bit */
  if (Bit)                    /* 1 */
    SPI_PORT |= (1 << SPI_MOSI);        /* set MOSI high */
  else                        /* 0 */
    SPI_PORT &= ~(1 << SPI_MOSI);       /* set MOSI low */

  /* start clock pulse (slave takes bit on rising edge) */
#ifdef SPI_SLOWDOWN
  /* tiny delay */
  asm volatile("nop");
#endif
  SPI_PORT |= (1 << SPI_SCK);           /* set SCK high */

  /* end clock pulse (falling edge) */
#ifdef SPI_SLOWDOWN
  /* tiny delay */
  asm volatile("nop");
#endif
  SPI_PORT &= ~(1 << SPI_SCK);          /* set SCK low */

  /*
   *  current state:
   *  - SCK low
   *  - MOSI undefined
   */
}

#endif // SPI_9


/*
 *  write a single byte
 *
 *  requires:
 *  - Byte: byte to write
 */

void SPI_Write_Byte(uint8_t Byte)
{
  uint8_t           n = 8;         /* counter */

  /*
   *  expected state:
   *  - SCK low
   *  - MOSI undefined
   */

  /*
   *  bitbang 8 bits:
   *  - simulate SPI mode 0 (CPOL = 0, CPHA = 0)
   *    - set MOSI before rising SCK
   *  - MSB first
   */

  while (n > 0)               /* for 8 bits */
  {
    /* get current MSB and set MOSI */
    if (Byte & 0b10000000)    /* 1 */
      SPI_PORT |= (1 << SPI_MOSI);      /* set MOSI high */
    else                      /* 0 */
      SPI_PORT &= ~(1 << SPI_MOSI);     /* set MOSI low */

    /* start clock pulse (slave takes bit on rising edge) */
#ifdef SPI_SLOWDOWN
    /* tiny delay */
    asm volatile("nop");
#endif
    SPI_PORT |= (1 << SPI_SCK);         /* set SCK high */

    /* end clock pulse (falling edge) */
#ifdef SPI_SLOWDOWN
    /* tiny delay */
    asm volatile("nop");
#endif
    SPI_PORT &= ~(1 << SPI_SCK);        /* set SCK low */

    Byte <<= 1;               /* shift bits one step left */
    n--;                      /* next bit */
  }

  /*
   *  current state:
   *  - SCK low
   *  - MOSI undefined
   */
}


#ifdef SPI_RW

/*
 *  write and read a single byte
 *  - SPI_PIN needs be set in config_<MCU>.h
 *
 *  requires:
 *  - Byte: byte to write
 *
 *  returns:
 *  - byte read
 */

uint8_t SPI_WriteRead_Byte(uint8_t Byte)
{
  uint8_t           Byte2 = 0;     /* return value */
  uint8_t           n = 8;         /* counter */
  uint8_t           Temp;          /* temporary value */

  /*
   *  expected state:
   *  - SCK low
   *  - MOSI undefined
   */

  /*
   *  bitbang 8 bits:
   *  - simulate SPI mode 0 (CPOL = 0, CPHA = 0)
   *    - set MOSI before rising SCK
   *    - read MISO after lowering SCK
   *  - MSB first
   */

  while (n > 0)               /* for 8 bits */
  {
    /* get current MSB and set MOSI */
    if (Byte & 0b10000000)    /* 1 */
      SPI_PORT |= (1 << SPI_MOSI);      /* set MOSI high */
    else                      /* 0 */
      SPI_PORT &= ~(1 << SPI_MOSI);     /* set MOSI low */

    /* start clock pulse (slave takes bit on rising edge) */
#ifdef SPI_SLOWDOWN
    /* tiny delay */
    asm volatile("nop");
#endif
    SPI_PORT |= (1 << SPI_SCK);         /* set SCK high */

    /* slave needs some time for processing */
    /* wait about 200ns (half cycle for SPI clock of 2.5MHz) */
#if F_CPU == 8000000
      /* one cycle is 125ns: wait 2 cycles (250ns) */
      asm volatile(
        "nop\n\t"
        "nop\n\t"
        ::
      );
#elif F_CPU == 16000000
      /* one cycle is 62.5ns: wait 3 cycles (187.6ns) */
      asm volatile(
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 20000000
      /* one cycle is 50ns: wait 4 cycles (200ns) */
      asm volatile(
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 24000000
      /* one cycle is 50ns: wait 5 cycles (208ns) */
      asm volatile(
        "nop\n\t"
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 32000000
      /* one cycle is 50ns: wait 7 cycles (219ns) */
      asm volatile(
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#else
  #error <<< SPI_WriteRead_Byte(): no supported MCU clock >>>
#endif

    /* end clock pulse (slave shifts bit out on falling edge) */
#ifdef SPI_SLOWDOWN
    /* tiny delay */
    asm volatile("nop");
#endif
    SPI_PORT &= ~(1 << SPI_SCK);        /* set SCK low */

    /* slave needs some time to shift out bit */
    /* wait about 200ns (half cycle for SPI clock of 2.5MHz) */
#if F_CPU == 8000000
      /* one cycle is 125ns: wait 2 cycles (250ns) */
      asm volatile(
        "nop\n\t"
        "nop\n\t"
        ::
      );
#elif F_CPU == 16000000
      /* one cycle is 62.5ns: wait 3 cycles (187.6ns) */
      asm volatile(
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 20000000
      /* one cycle is 50ns: wait 4 cycles (200ns) */
      asm volatile(
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 24000000
      /* one cycle is 50ns: wait 5 cycles (208ns) */
      asm volatile(
        "nop\n\t"
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#elif F_CPU == 32000000
      /* one cycle is 50ns: wait 7 cycles (219ns) */
      asm volatile(
        "nop\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        "brcc .\n\t"
        "brcs .\n\t"
        ::
      );
#else
      #error <<< SPI_WriteRead_Byte(): no supported MCU clock >>>
#endif

    /* read MISO */
    Temp = SPI_PIN;              /* read port */
    Temp &= (1 << SPI_MISO);     /* filter MISO */
    Byte2 <<= 1;                 /* shift bits one step left */
    if (Temp)                    /* MISO set */
      Byte2 |= 0b00000001;       /* set bit */

    Byte <<= 1;               /* shift bits one step left */
    n--;                      /* next bit */
  }

  /*
   *  current state:
   *  - SCK low
   *  - MOSI undefined
   */

  return Byte2;
}


#endif // SPI_RW

#endif // SPI_BITBANG
