#include <avr/io.h>

#include "spi.h"
#include "common.h"


/*
 * SPI
 */

#if 0
#define spi_bitbang()  { asm volatile("nop"); SB(PORTB, SCK); asm volatile("nop"); CB(PORTB, SCK); }
#else
#define spi_bitbang()  { SB(PORTB, SCK); CB(PORTB, SCK); }
#endif


uint32_t spi_read(uint8_t bytes, uint8_t dummy) {
  uint32_t ret = 0;

  CB(DDRB, MOSI);
  SB(PORTB, MOSI | DC);
 
  /* any dummy clocks */
  if (dummy)
    spi_bitbang();

  /* 1 byte = 8 bits */
  bytes <<= 3;

  /* read results */
  while (bytes-- != 0) {
    ret <<= 1;
    if (ISB(PINB, MOSI))
      ret |= 1;

    spi_bitbang();
  }
  return ret;
}

void spi_write8(uint8_t value, uint8_t dc) {
  uint8_t n;

  SB(DDRB, MOSI);
  SCB(dc, PORTB, DC);

  /* send command */
  for (n = 8; n != 0; n--) {
    SCB(value & 0x80, PORTB, MOSI);
    spi_bitbang();
    value <<= 1;
  }
}

void spi_write8x(uint8_t value, uint16_t count) {
  while (count-- != 0)
    spi_write8(value, 1);
}

void spi_write16(uint16_t value) {
  spi_write8(HI(value), 1);
  spi_write8(LO(value), 1);
}

void spi_write16x(uint16_t value, uint16_t count) {
  while (count-- != 0)
    spi_write16(value);
}

uint32_t spi_readreg(uint8_t reg, uint8_t bytes, uint8_t dummy) {
  spi_write8(reg, 0);
  return spi_read(bytes, dummy);
}
