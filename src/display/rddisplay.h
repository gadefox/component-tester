/* ************************************************************************
 *
 *   LCD display functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef LCD_RDDISPLAY

#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H


#ifdef SW_DISPLAY_ID

#if defined(LCD_READ) && !defined(I2C_RW)
#error <<< Display ID requires LCD read >>>
#endif

#define FUNC_DISPLAY_HEXVALUE
#endif
 
#define LCD_GRAPHIC                     /* graphic display */
#define LCD_COLOR                       /* color display */

/* display settings */
#define LCD_DOTS_X       240            /* number of horizontal dots */
#define LCD_DOTS_Y       320            /* number of vertical dots */
//#define LCD_FLIP_X                      /* enable horizontal flip */
//#define LCD_FLIP_Y                      /* enable vertical flip */
//#define LCD_ROTATE                      /* switch X and Y (rotate by 90°) */
//#define LCD_BGR                         /* reverse red and blue color channels */

/* font and symbols: horizontally aligned & flipped */
#if !defined(FONT_16X26_HF)
#error <<< Font not supported >>>
#endif

#if !defined(SYMBOLS_32X32_HF) && !defined(SYMBOLS_32X32_ALT1_HF) && !defined(SYMBOLS_32X32_ALT2_HF) && !defined(SYMBOLS_32X39_HF)
#error <<< Symbol not supported >>>
#endif


/*
 *  R&D Display
 *  - 8 bit parallel interface
 *  - LCD_DB0 to LCD_DB7 have to match port pins 0 to 7
 */

#define LCD_PAR_8                       /* 8 bit parallel interface */

/* control lines */
#define LCD_PORT         PORTC          /* port data register */
#define LCD_DDR          DDRC           /* port data direction register */
#define LCD_RES          PC0            /* port pin used for /RESX (optional) */
#define LCD_CS           PC1            /* port pin used for /CSX (optional) */
#define LCD_DC           PC2            /* port pin used for D/CX */
#define LCD_WR           PC3            /* port pin used for WRX */
#define LCD_RD           PC4            /* port pin used for RDX */

/* data lines DB0-7 */
#define LCD_PORT2        PORTL          /* port data register */
#define LCD_DDR2         DDRL           /* port data direction register */
#define LCD_PIN2         PINL           /* port data direction register */
#define LCD_DB0          PL0            /* port pin used for D0 */
#define LCD_DB1          PL1            /* port pin used for D1 */
#define LCD_DB2          PL2            /* port pin used for D2 */
#define LCD_DB3          PL3            /* port pin used for D3 */
#define LCD_DB4          PL4            /* port pin used for D4 */
#define LCD_DB5          PL5            /* port pin used for D5 */
#define LCD_DB6          PL6            /* port pin used for D6 */
#define LCD_DB7          PL7            /* port pin used for D7 */


/*
 *  R&D Display
 *  - RGB565
 */

/* common colors */
#define COLOR_BLACK         0x0000    /* RGB 000000 */
#define COLOR_BLUE          0x001f    /* RGB 0000F8 */
#define COLOR_GREEN         0x07e0    /* RGB 00FC00 */
#define COLOR_CYAN          0x07ff    /* RGB 00FCF8 */
#define COLOR_RED           0xf800    /* RGB F80000 */
#define COLOR_MAGENTA       0xf81f    /* RGB F800F8 */
#define COLOR_YELLOW        0xffe0    /* RGB F8FC00 */
#define COLOR_ORANGE        0xfd20    /* RGB F8A400 */
#define COLOR_GREY          0xc618    /* RGB C0C0C0 */
#define COLOR_WHITE         0xffff    /* RGB FFFFFF */
#define COLOR_BROWN         0x6920    /* RGB 682400 */
#define COLOR_VIOLET        0x780f    /* RGB 780078 */
#define COLOR_PALE_YELLOW   0xe70c    /* RGB E2E164 */
#define COLOR_PALE_GREEN    0x9fec    /* RGB 99FF66 */
#define COLOR_PALE_RED      0xfb6d    /* RGB FF6F6F */
#define COLOR_STEEL_BLUE    0x64df    /* RGB 6098F8 */
#define COLOR_SKY_BLUE      0x06ff    /* RGB 06DCF9 */

/* component color codes */
#define COLOR_CODE_BLACK    0x0000    /* RGB 000000 */
#define COLOR_CODE_BROWN    0x6920    /* RGB 682400 */
#define COLOR_CODE_RED      0xf800    /* RGB F80000 */
#define COLOR_CODE_ORANGE   0xfc80    /* RGB F89300 */
#define COLOR_CODE_YELLOW   0xffe0    /* RGB F8FC00 */
#define COLOR_CODE_GREEN    0x05c0    /* RGB 00BB00 */
#define COLOR_CODE_BLUE     0x001f    /* RGB 0000F8 */
#define COLOR_CODE_VIOLET   0x780f    /* RGB 780078 */
#define COLOR_CODE_GREY     0x9cd3    /* RGB 9A9A9A */
#define COLOR_CODE_WHITE    0xffff    /* RGB FFFFFF */
#define COLOR_CODE_GOLD     0xf606    /* RGB F6C337 */
#define COLOR_CODE_SILVER   0xe71c    /* RGB E0E0E0 */

/* component's body color */
#define COLOR_CODE_NONE     0x06ff    /* RGB 06DCF9 */


#endif // LCD_DISPLAY_H

#endif // LCD_RD_DISPLAY
