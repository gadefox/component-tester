/* ************************************************************************
 *
 *   ST7565R graphic display controller
 *
 *   (c) 2015-2017 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifdef LCD_ST7565R

#ifndef ST7565R_H
#define ST7576R_H


/* ************************************************************************
 *   configure
 * ************************************************************************ */

#define LCD_GRAPHIC                     /* graphic display */

/* display settings */
#define LCD_DOTS_X       128            /* number of horizontal dots */
#define LCD_DOTS_Y       64             /* number of vertical dots */
#define LCD_OFFSET_X                    /* enable x offset of 4 dots */
//#define LCD_FLIP_X                      /* enable horizontal flip */
#define LCD_FLIP_Y                      /* enable vertical flip */
#define LCD_START_Y      0              /* start line (0-63) */
#define LCD_CONTRAST     22             /* default contrast (0-63) */
//#define LCD_LONG_RESET                   /* long reset for NT7538 */

/* font and symbols: vertically aligned & flipped, bank-wise grouping */
#if !defined(FONT_6X8_VF) && !defined(FONT_8X8_VF) && !defined(FONT_8X16_VFP) && !defined(FONT_6X8_ISO8859_2_VF) && \
    !defined(FONT_8X8_ISO8859_2_VF) && !defined(FONT_8X12T_ISO8859_2_VFP) && !defined(FONT_8X16_ISO8859_2_VFP) && \
     !defined(FONT_8X8_WIN1251_VF) && !defined(FONT_8X8ALT_WIN1251_VF) && !defined(FONT_8X8T_WIN1251_VF) && \
     !defined(FONT_8X12T_WIN1251_VFP) && !defined(FONT_8X16_WIN1251_VFP)
#error <<< Font not supported >>>
#endif

#if !defined(SYMBOLS_24X24_VFP) && !defined(SYMBOLS_24X24_ALT1_VFP) && !defined(SYMBOLS_24X24_ALT2_VFP)
#error <<< Symbol not supported >>>
#endif

/*
 *  ST7565R
 *  - SPI interface using bit-bang SPI
 *  - settings for Electronic Assembly EA DOGM/DOGL128-6
 *  - uses LCD_CS to support rotary encoder in parallel at PD2/3
 */

/* control and data lines */
#define LCD_PORT         PORTD          /* port data register */
#define LCD_DDR          DDRD           /* port data direction register */
#define LCD_RESET        PD0            /* port pin used for /RES (optional) */
#define LCD_CS           PD5            /* port pin used for /CS1 (optional) */
#define LCD_A0           PD1            /* port pin used for A0 */
#define LCD_SCL          PD2            /* port pin used for SCL */
#define LCD_SI           PD3            /* port pin used for SI (data input) */

/* SPI bus */
#define SPI_BITBANG                     /* bit-bang SPI */
#define SPI_PORT         LCD_PORT       /* SPI port data register */
#define SPI_DDR          LCD_DDR        /* SPI port data direction register */
#define SPI_SCK          LCD_SCL        /* port pin used for SCK */
#define SPI_MOSI         LCD_SI         /* port pin used for MOSI */

#if 0
#define SPI_HARDWARE                    /* hardware SPI */
#endif


/* ************************************************************************
 *   LCD commands
 * ************************************************************************ */

/*
 *  switch display on/off
 */

#define CMD_DISPLAY           0b10101110     /* switch display on/off */

#define FLAG_DISPLAY_OFF      0b00000000     /* display off */
#define FLAG_DISPLAY_ON       0b00000001     /* display on */


/*
 *  set start line
 *  - valid range: 0 - 63
 */

#define CMD_START_LINE        0b01000000     /* set start line */


/*
 *  set page
 *  - valid range: 0 - 8
 */

#define CMD_PAGE              0b10110000     /* set page */


/*
 *  set column MSB
 *  - valid range: 0 - 131
 */

#define CMD_COLUMN_H          0b00010000     /* set MSB (upper nibble) of column */


/*
 *  set column LSB
 *  - valid range: 0 - 131
 */

#define CMD_COLUMN_L          0b00000000     /* set LSB (lower nibble) of column */


/*
 *  segment driver direction (horizontal direction)
 */

#define CMD_SEGMENT_DIR       0b10100000     /* set segment driver direction (horizontal direction) */

#define FLAG_ADC_NORMAL       0b00000000     /* normal */
#define FLAG_ADC_REVERSE      0b00000001     /* reversed/flipped */


/*
 *  display image mode
 */

#define CMD_IMAGE_MODE        0b10100110     /* set display image mode */

#define FLAG_IMAGE_NORMAL     0b00000000     /* normal / positive */
#define FLAG_IMAGE_REVERSE    0b00000001     /* reverse / negative */


/*
 *  all points mode
 */

#define CMD_ALL_POINTS        0b10100100     /* set "all points" mode */

#define FLAG_POINTS_OFF       0b00000000     /* normal mode */
#define FLAG_POINTS_ON        0b00000001     /* display all points */


/*
 *  bias ratio of LCD drive voltage
 */

#define CMD_LCD_BIAS          0b10100010     /* set bias ratio of LCD drive voltage */

#define FLAG_BIAS_19          0b00000000     /* 1/9 */
#define FLAG_BIAS_17          0b00000001     /* 1/7 */


/* 
 *  enter read-modify-write mode
 */

#define CMC_R_M_W             0b11100000     /* enter read-modify-write mode */


/*
 *  end read-modify-write mode
 */

#define CMD_END               0b11101110     /* end read-modify-write mode */


/*
 *  internal reset
 */

#define CMD_RESET             0b11100010     /* perform internal reset */


/*
 *  common driver direction (vertical direction)
 */

#define CMD_COMMON_DIR        0b11000000     /* set common driver direction (vertical direction) */

#define FLAG_COM_NORMAL       0b00000000     /* normal */
#define FLAG_COM_REVERSE      0b00001000     /* reversed/flipped */


/*
 *  power supply operation mode
 */

#define CMD_POWER_MODE        0b00101000     /* select power supply operation mode */

#define FLAG_FOLOWER_OFF      0b00000000     /* voltage folower circuit off */
#define FLAG_FOLOWER_ON       0b00000001     /* voltage folower circuit on */
#define FLAG_REGULATOR_OFF    0b00000000     /* voltage regulator off */
#define FLAG_REGULATOR_ON     0b00000010     /* voltage regulator on */
#define FLAG_BOOSTER_OFF      0b00000000     /* booster circuit off */
#define FLAG_BOOSTER_ON       0b00000100     /* booster circuit on */


/*
 *  V0 regulator resistor ratio (1 + Rb/Ra)
 */

#define CMD_V0_RATIO          0b00100000     /* select resistor ratio for V0/contrast voltage regulator */

#define FLAG_RATIO_30         0b00000000     /* ratio 3.0 */
#define FLAG_RATIO_35         0b00000001     /* ratio 3.5 */
#define FLAG_RATIO_40         0b00000010     /* ratio 4.0 */
#define FLAG_RATIO_45         0b00000011     /* ratio 4.5 */
#define FLAG_RATIO_50         0b00000100     /* ratio 5.0 */
#define FLAG_RATIO_55         0b00000101     /* ratio 5.5 */
#define FLAG_RATIO_60         0b00000110     /* ratio 6.0 */
#define FLAG_RATIO_65         0b00000111     /* ratio 6.5 */


/*
 *  set contrast
 *  - 2 byte command
 *  - valid range: 63 - 0
 */

/* byte #1: command */
#define CMD_V0_MODE           0b10000001     /* enable V0/contrast value set mode */

/* byte #2: V0/contrast value */
/* valid range: 63 (00000) - 0 (11111) */


/*
 *  indicator state
 *  - 2 byte command when enabling
 */

/* byte #1: command */
#define CMD_INDICATOR_MODE    0b10101100     /* enable/disable indicator */

/* byte #2: indicator state */
#define FLAG_INDICATOR_OFF    0b00000000     /* off */
#define FLAG_INDICATOR_1HZ    0b00000001     /* blinking at 1Hz */
#define FLAG_INDICATOR_05HZ   0b00000010     /* blinking at 0.5Hz */
#define FLAG_INDICATOR_ON     0b00000011     /* on (permanently) */


/*
 *  booster ratio
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_BOOSTER_MODE      0b11111000     /* enable booster ratio set mode */

/* byte #2: booster ratio */
#define FLAG_BOOSTER_234      0b00000000     /* 2x, 3x, 4x */
#define FLAG_BOOSTER_5        0b00000001     /* 5x */
#define FLAG_BOOSTER_6        0b00000010     /* 6x */


/*
 *  no operation
 */

#define CMD_NOP               0b11100011     /* no operation */


/*
 *  test
 */

#define CMD_TEST              0b11111100     /* test chip (don't use this cmd!) */


#endif // ST7565R_H

#endif // LCD_ST7565R
