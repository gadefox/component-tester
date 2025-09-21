#ifndef ST7735_H
#define ST7735_H

#include <stdint.h>

#define COLMODE_12  0x03
#define COLMODE_16  0x05
#define COLMODE_18  0x06

/* (0b) bbbbbggggggrrrrr */
#define RGB(r, g, b)  ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

/*
 * MADCTL bits
 */
#define MADCTL_MH   0b00000100
#define MADCTL_BGR  0b00001000
#define MADCTL_ML   0b00010000
#define MADCTL_MV   0b00100000
#define MADCTL_MX   0b01000000
#define MADCTL_MY   0b10000000

/*
 * Display rotation
 */
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
#endif

/*
 * Predefined colors
 */
extern const uint16_t colors[];

/*
 * Helper macros
 */
#define st7735_clearscr()  st7735_fillregion(0, 0, DISPW, DISPH, 0)

/*
 * API
 */
void st7735_setregion(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void st7735_fillregion(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void st7735_hardreset(void);
void st7735_softreset(void);
void st7735_sleepout(uint8_t out);
void st7735_colormode(uint8_t mode);
void st7735_disppower(uint8_t on);
void st7735_invertmode(uint8_t on);
void st7735_memaccess(uint8_t mode);
void st7735_init(void);
uint32_t st7735_readreg(uint8_t reg);

#endif /* ST7735_H */
