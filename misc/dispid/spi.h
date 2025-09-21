#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define SCK   BV(PB5)
#define MOSI  BV(PB3)
#define DC    BV(PB1)
#define RST   BV(PB2)

/*
 * SPI API (bit-bang)
 */
uint32_t spi_read(uint8_t bytes, uint8_t dummy);
void spi_write8(uint8_t value, uint8_t dc);
void spi_write8x(uint8_t value, uint16_t count);
void spi_write16(uint16_t value);
void spi_write16x(uint16_t value, uint16_t count);
uint32_t spi_readreg(uint8_t reg, uint8_t bytes, uint8_t dummy);

#endif /* SPI_H */
