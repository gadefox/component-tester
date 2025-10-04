#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#ifdef __LGT8F__
#include "lgtx8p.h"
#endif

#include "common.h"
#include "segm.h"
#include "spi.h"
#include "st7735.h"
#include "uart.h"


#define DELAY  2000

const uint32_t colors[] = {
  RGB(0,   255, 0),   /* Green */
  RGB(255, 255, 0),   /* Yellow */
  RGB(0,   255, 255), /* Cyan */
  RGB(255, 255, 255), /* White */
  RGB(255, 0,   0),   /* Red */
  RGB(0,   0,   255), /* Blue */
  RGB(255, 0,   255)  /* Magenta */
};

uint32_t reg_read(const char* name, uint8_t reg) {
  uint32_t value = st7735_readreg(reg, 1, 0);
  uart_str(name);
  uart_str(": 0x");
  uart_hex(value, 2);
  uart_nl();
  return value;
}

void drawcolors(const uint32_t* colors, uint8_t count) {
#if COLMODE == COLMODE_18
  uint32_t col;
#endif
  uint8_t x = 0, y = 0;
  uint8_t width = sqrt(count >> 1) + 1;

  width =
#ifdef ROTATE
    DISPH
#else
    DISPW
#endif /* ROTATE */
    / width;

  while (count-- != 0) {
#if COLMODE == COLMODE_18
    col = *colors++;
    uart_uint16(RED(col));
    uart_nl();
    uart_uint16(GREEN(col));
    uart_nl();
    uart_uint16(BLUE(col));
    uart_nl();
    st7735_fillrgb(x, y, width, width, RED(col), GREEN(col), BLUE(col));
#else
    st7735_fillregion(x, y, width, width, *colors++);
#endif

    x += width;
    if (x + width > DISPW) {
      x = 0;
      y += width;
    }
    delay(DELAY);
  }
}

int main(void) {
#ifdef __LGT8F__
  /* set clock prescaler to 1 */
  CLKPR = BV(WCE);
  CLKPR = 0;
#endif /* __LGT8F__ */
  DDRB = SCK | MOSI | DC | RST;

  uart_init();
  st7735_hardreset();

#if 1
  st7735_init();
  segm_test(colors, COUNT(colors));
//  delay(DELAY);
//  drawcolors(colors, COUNT(colors));
#else
  uint8_t id1, id2, id3;

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
  segm_drawhex(0, 0, colors[1], id1, 2);
  segm_drawhex(0, SEGMH2, colors[3], id2, 2);
  segm_drawhex(0, SEGMH2 * 2, colors[5], id3, 2);
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
