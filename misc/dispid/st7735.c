#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
#include "st7735.h"
#include "spi.h"

/*
 * ST7735
 */

#define SWRESET    0x01
#define RDDPM      0x0A
#define RDDMADCTL  0x0B
#define RDDCOLMOD  0x0C
#define RDDIM      0x0D
#define RDDSM      0x0E
#define RDDSDR     0x0F
#define SLPIN      0x10
#define SLPOUT     0x11
#define INVOFF     0x20
#define INVON      0x21
#define DISPOFF    0x28
#define DISPON     0x29
#define CASET      0x2A
#define RASET      0x2B
#define RAMWR      0x2C
#define MADCTL     0x36
#define COLMOD     0x3A
#define RDID1      0xDA
#define RDID2      0xDB
#define RDID3      0xDC

#define COLMODE_12  0x03
#define COLMODE_16  0x05
#define COLMODE_18  0x06

/* (0b) bbbbbggggggrrrrr */
#define RGB(r, g, b)  ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#define MADCTL_MH   0b00000100
#define MADCTL_BGR  0b00001000
#define MADCTL_ML   0b00010000
#define MADCTL_MV   0b00100000
#define MADCTL_MX   0b01000000
#define MADCTL_MY   0b10000000

#define ROTATE 1

#if ROTATE == 1
#define DISPX  1
#define DISPY  26
#define DISPW  160
#define DISPH  80
#else
#define DISPX  26
#define DISPY  1
#define DISPW  80
#define DISPH  160
#endif /* ROTATE */

const uint16_t colors[] = {
  RGB(255, 0,   0),   /* Red */
  RGB(0,   255, 0),   /* Green */
  RGB(0,   0,   255), /* Blue */
  RGB(255, 255, 0),   /* Yellow */
  RGB(255, 0,   255), /* Magenta */
  RGB(0,   255, 255), /* Cyan */
  RGB(255, 255, 255)  /* White */
};

#define st7735_clearscr()  st7735_fillregion(0, 0, DISPW, DISPH, 0)  /* black */

/*
 * display fn(s)
 */

void st7735_setregion(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  spi_write8(CASET, 0);
  spi_write8(0, 1);
  spi_write8(x + DISPX, 1);          /* x + DISPX < 162 */
  spi_write8(0, 1);
  spi_write8(x + w - 1 + DISPX, 1);  /* x + w - 1 + DISPX < 162 */

  spi_write8(RASET, 0);
  spi_write8(0, 1);
  spi_write8(y + DISPY, 1);          /* y + DISPY < 162 */
  spi_write8(0, 1);
  spi_write8(y + h - 1 + DISPY, 1);  /* y + w - 1 + DISPY < 162 */

  spi_write8(RAMWR, 0);
}

void st7735_fillregion(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  st7735_setregion(x, y, w, h);
  spi_write16x(color, w * h);
}

void st7735_hardreset(void) {
  CB(PORTB, RST);
  delay(10);
  SB(PORTB, RST);
  delay(120);
}

void st7735_softreset(void) {
  spi_write8(SWRESET, 0);
  delay(120);
}

void st7735_sleepout(uint8_t out) {
  spi_write8(SLPIN + out, 0);  /* out {0, 1} */
}

void st7735_colormode(uint8_t mode) {
  spi_write8(COLMOD, 0);
  spi_write8(mode, 1);
}

void st7735_disppower(uint8_t on) {
  spi_write8(DISPOFF + on, 0);  /* on [0, 1] */
}

void st7735_invertmode(uint8_t on) {
  spi_write8(INVOFF + on, 0);  /* on [0, 1] */
}

void st7735_memaccess(uint8_t mode) {
  spi_write8(MADCTL, 0);
  spi_write8(mode, 1);
}

void st7735_init(void) {
  st7735_sleepout(1);
  delay(120);
  st7735_colormode(COLMODE_16);
  delay(500);
#if defined(ROTATE)
//  st7735_memaccess(MADCTL_BGR | MADCTL_MV | MADCTL_MX);
  st7735_memaccess(MADCTL_BGR | MADCTL_MV | MADCTL_MY);
#else
  st7735_memaccess(MADCTL_BGR);
#endif
  st7735_invertmode(1);
  st7735_clearscr();
  st7735_disppower(1);
  delay(500);
}

/*
 * registers
 */

uint32_t st7735_readreg(uint8_t reg) {
  st7735_softreset();
  return spi_readreg(reg, 1, 0);
}
