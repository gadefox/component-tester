#include <avr/io.h>
#include <util/delay.h>

#include "common.h"
#include "st7735.h"
#include "spi.h"


/*
 * ST7735
 */

#define SWRESET    0x01
#define SLPIN      0x10
#define SLPOUT     0x11
#define PTLON      0x12
#define NORON      0x13
#define INVOFF     0x20
#define INVON      0x21
#define DISPOFF    0x28
#define DISPON     0x29
#define CASET      0x2A
#define RASET      0x2B
#define RAMWR      0x2C
#define PTLAR      0x30
#define SCRLAR     0x33
#define MADCTL     0x36
#define VSCSAD     0x37
#define IDMOFF     0x38
#define IDMON      0x39
#define COLMOD     0x3A

#define COLMODE_12  0x03
#define COLMODE_16  0x05
#define COLMODE_18  0x06

#define MADCTL_MH   0b00000100
#define MADCTL_BGR  0b00001000
#define MADCTL_ML   0b00010000
#define MADCTL_MV   0b00100000
#define MADCTL_MX   0b01000000
#define MADCTL_MY   0b10000000


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

void st7735_fillrgb(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t g, uint8_t b) {
  st7735_setregion(x, y, w, h);
  spi_writergbx(r, g, b, w * h);
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

void st7735_dispmode(uint8_t norm) {
  spi_write8(PTLON + norm, 0);  /* norm [0, 1] */
}

void st7735_partarea(uint8_t y, uint8_t h) {
  spi_write8(PTLAR, 0);
  spi_write8(0, 1);
  spi_write8(y + DISPY, 1);
  spi_write8(0, 1);
  spi_write8(y + h - 1 + DISPY, 1);
}

void st7735_scrollarea(uint8_t tfa, uint8_t vsa, uint8_t bfa) {
  spi_write8(SCRLAR, 0);
  spi_write8(0, 1);
  spi_write8(tfa, 1);
  spi_write8(0, 1);
  spi_write8(vsa, 1);
  spi_write8(0, 1);
  spi_write8(bfa, 1);
}

void st7735_scroll(uint8_t addr) {
  spi_write8(VSCSAD, 0);
  spi_write8(0, 1);
  spi_write8(addr, 1);
}

void st7735_idlemode(uint8_t on) {
  spi_write8(IDMOFF + on, 0);  /* on {0, 1} */
}

void st7735_memaccess(uint8_t mode) {
  spi_write8(MADCTL, 0);
  spi_write8(mode, 1);
}

void st7735_init(void) {
  st7735_sleepout(1);
  st7735_colormode(COLMODE);
#if defined(ROTATE)
  st7735_memaccess(MADCTL_RGB | MADCTL_MV | MADCTL_MX);
#else
  st7735_memaccess(MADCTL_RGB);
#endif
  st7735_invertmode(1);  /* 0 black; 255 white */
  st7735_idlemode(1);    /* broken module, so only 8 colors */
  st7735_clearscr();
  st7735_disppower(1);
}

/*
 * registers
 */

uint32_t st7735_readreg(uint8_t reg, uint8_t bytes, uint8_t dummy) {
  st7735_softreset();
  return spi_readreg(reg, bytes, dummy);
}
