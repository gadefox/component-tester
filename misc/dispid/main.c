#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <math.h>

#ifdef __LGT8F__
#include "lgtx8p.h"
#endif

/*
 * misc
 */

#define COUNT(a)   (sizeof(a) / sizeof(a[0]))
#define delay(ms)  _delay_ms(ms)

/*
 * flags
 */

#define HI(w)  ((w) >> 8)
#define LO(w)  ((w) & 0xFF)
#define BV(b)  (1 << b)

#define SB(v, b)      v |= b
#define CB(v, b)      v &= ~(b)
#define TB(v, b)      v ^= f
#define ISB(v, b)     ((v) & (b))
#define SCB(c, v, b)  do { if (c) SB(v, b); else CB(v, b); } while(0)

/*
 * SPI
 */

#if 0
#define spi_bitbang()  { asm volatile("nop"); SB(PORTB, SCK); asm volatile("nop"); CB(PORTB, SCK); }
#else
#define spi_bitbang()  { SB(PORTB, SCK); CB(PORTB, SCK); }
#endif

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
//#define COL(r, g, b)  (((r) << 11) | ((g) << 5) | (b))

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

uint16_t colors1[8] = {
  RGB(255, 0,   0),
  RGB(0,   255, 0),
  RGB(0,   0,   255),
  RGB(255, 255, 0),
  RGB(255, 0,   255),
  RGB(0,   255, 255),
  RGB(255, 255, 255),
  RGB(0,   0,   0),
};

#define st7735_clearscr()  st7735_fillregion(0, 0, DISPW, DISPH, 0)  /* black */

/*
 * Segments
 *     a
 *    ---
 *  f|   |b
 *    ---
 *  e| g |c
 *    ---
 *     d
 */

const uint8_t digits[] = {
 /* abcdefg */
  0b1111110, /* 0 */
  0b0110000, /* 1 */
  0b1101101, /* 2 */
  0b1111001, /* 3 */
  0b0110011, /* 4 */
  0b1011011, /* 5 */
  0b1011111, /* 6 */
  0b1110000, /* 7 */
  0b1111111, /* 8 */
  0b1111011, /* 9 */
  0b1110111, /* A */
  0b0011111, /* b */
  0b1001110, /* C */
  0b0111101, /* d */
  0b1001111, /* E */
  0b1000111, /* F */
  0b0110111  /* h */
};

#define SEGMW   8
#define SEGMH   14
#define SEGMW2  (SEGMW + 2)
#define SEGMH2  (SEGMH + 2)

/*
 * LGT8F
 */

#define SCK   BV(PB5)
#define MOSI  BV(PB3)
#define DC    BV(PB1)
#define RST   BV(PB2)

/*
 * UART
 */

void uart_init(void) {
  UBRR0 = F_CPU / 16 / 19200 - 1;
  UCSR0B = BV(TXEN0) | BV(RXEN0);
  UCSR0C = BV(UCSZ01) | BV(UCSZ00);
}

void uart_ch(char c) {
  while (!ISB(UCSR0A, BV(UDRE0)))
    /* nop */;
  UDR0 = c;
}

void uart_nl(void) {
  uart_ch('\r');
  uart_ch('\n');
}

void uart_str(const char *s) {
  char c;

  while (1) {
    c = *s;
    if (c == '\0')
      break;
    uart_ch(c);
    s++;
  }
}

void uart_print(const char* s) {
  uart_str(s);
  uart_nl();
}

void uart_hex(uint32_t value, uint8_t digits) {
  uint8_t nibble;

  digits <<= 2;
  do {
    digits -= 4;

    nibble = (value >> digits) & 0xF;
    if (nibble < 10)
      nibble += '0';
    else
      nibble += 'A' - 10;

    uart_ch(nibble);
  } while (digits != 0);
}

void uart_uint16(uint16_t value) {
  char buf[5];  /* sizeof("65535")=5 */
  int i = 0;

  do {
    buf[i++] = '0' + (value % 10);
    value /= 10;
  } while (value != 0);

  while (i != 0)
    uart_ch(buf[--i]);
}

/*
 * SPI
 */

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
  st7735_colormode(COLMODE_16);
#if defined(ROTATE)
  st7735_memaccess(MADCTL_BGR | MADCTL_MV | MADCTL_MX);
#else
  st7735_memaccess(MADCTL_BGR);
#endif
  st7735_invertmode(1);
  st7735_clearscr();
  st7735_disppower(1);
}

/*
 * segments
 */

void segm_draw(int x, int y, uint16_t color, uint8_t value) {
  uint8_t mask = digits[value];

  /* a */
  if (mask & 0b1000000)
    st7735_fillregion(x + 2, y, SEGMW - 4, 2, color);
  /* g */
  if (mask & 0b0000001)
    st7735_fillregion(x + 2, y + SEGMH / 2 - 1, SEGMW - 4, 2, color);
  /* d */
  if (mask & 0b0001000)
    st7735_fillregion(x + 2, y + SEGMH - 2, SEGMW - 4, 2, color);

  /* f */
  if (mask & 0b0000010)
    st7735_fillregion(x, y + 2, 2, SEGMH / 2 - 3, color);
  /* b */
  if (mask & 0b0100000)
    st7735_fillregion(x + SEGMW - 2, y + 2, 2, SEGMH / 2 - 3, color);

  /* e */
  if (mask & 0b0000100)
    st7735_fillregion(x, y + SEGMH / 2 + 1, 2, SEGMH / 2 - 3, color);
  /* c */
  if (mask & 0b0010000)
    st7735_fillregion(x + SEGMW - 2, y + SEGMH / 2 + 1, 2, SEGMH / 2 - 3, color);
}

void segm_drawhex(uint8_t x, uint8_t y, uint16_t color, uint32_t value, uint8_t digits) {
  uint8_t nibble;

  digits <<= 2;
  do {
    digits -= 4;
    nibble = (value >> digits) & 0xF;

    segm_draw(x, y, color, nibble);
    x += SEGMW2;
  } while (digits != 0);

  segm_draw(x, y, color, 16);
}

void segm_test(uint16_t color) {
  uint8_t x = 0, y = 0;

  for (uint8_t i = 0; i < 80; i++) {
    segm_draw(x, y, color, i % COUNT(digits));

    x += SEGMW2;
    if (x > DISPW - SEGMW2) {
      x = 0;
      y += SEGMH2;
      if (y > DISPH - SEGMH2)
        break;
    }
  }
}

/*
 * registers
 */

uint32_t reg_read(const char* name, uint8_t reg) {
  uint32_t value;

  st7735_softreset();
  value = spi_readreg(reg, 1, 0);

  uart_str(name);
  uart_str(": 0x");
  uart_hex(value, 2);
  uart_nl();

  return value;
}

/*
 * misc
 */
void drawcolors(uint16_t* colors, uint8_t count) {
#define DELAY  10000

  uint8_t x = 0, y = 0;
  uint8_t width = sqrt(count >> 1);

#ifdef ROTATE
  width = DISPH / count;
#else
  width = DISPW / count;
#endif /* ROTATE */

  while (count-- != 0) {
    st7735_fillregion(x, y, width, width, *colors++);

    x += width;
    if (x + width > DISPW) {
      x = 0;
      y += width;
    }
    delay(DELAY);
  }
}

int main(void) {
  uint8_t id1, id2, id3;

#ifdef __LGT8F__
  /* set clock prescaler to 1 */
  CLKPR = BV(WCE);
  CLKPR = 0;
#endif /* __LGT8F__ */
  DDRB = SCK | MOSI | DC | RST;

  uart_init();
  st7735_hardreset();

#if 0
  st7735_init();
  segm_test(colors1[1]);
  delay(DELAY);
  drawcolors(colors1, COUNT(colors1));
#else
  reg_read("RDDPM",     RDDPM);
  reg_read("RDDMADCTL", RDDMADCTL);
  reg_read("RDDCOLMOD", RDDCOLMOD);
  reg_read("RDDIM",     RDDIM);
  reg_read("RDDSM",     RDDSM);
  reg_read("RDDSDR",    RDDSDR);

  id1 = reg_read("RDID1", RDID1);
  id2 = reg_read("RDID2", RDID2);
  id3 = reg_read("RDID3", RDID3);

  st7735_init();
  segm_drawhex(0, 0, colors1[1], id1, 2);
  segm_drawhex(0, SEGMH2, colors1[3], id2, 2);
  segm_drawhex(0, SEGMH2 * 2, colors1[5], id3, 2);
#endif

/*  
 * ID1 ID2 ID3  MFR         IC        GEOMETRY
 * -------------------------------------------
 * 00h 91h 06h  GalaxyCore  GC9106    128x160
 * 54h 80h 66h              ILI9163C
 * 5Ch 88h 35h  Sitronix    ST7735
 * 7Ch 89h F0h  Sitronix    ST7735S   132x162
 * 5Ch 89h F0h  Sitronix    ST7735R
 * 83h 76h 0Fh  clone?      ST7735S   132x162
 * 
 */
  return 0;
}
