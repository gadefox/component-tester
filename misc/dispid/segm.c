#include "common.h"
#include "segm.h"
#include "st7735.h"


/*
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
  0b0010111  /* h */
};


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

void segm_test(const uint32_t* colors, uint32_t count) {
  uint8_t x = 0, y = 0;

  for (uint8_t i = 0; i < 80; i++) {
    segm_draw(x, y, colors[i % count], i % COUNT(digits));

    x += SEGMW2;
    if (x > DISPW - SEGMW2) {
      x = 0;
      y += SEGMH2;
      if (y > DISPH - SEGMH2)
        break;
    }
  }
}
