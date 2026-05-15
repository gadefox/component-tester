/* ************************************************************************
 *
 *   HD44780 compatible character display
 *
 *   (c) 2012-2021 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifdef LCD_HD44780

#ifndef HD44780_H
#define HD44780_H


/* ************************************************************************
 *   configure
 * ************************************************************************ */

#define LCD_TEXT                        /* character display */

/* display settings */
#define LCD_CHAR_X       16             /* characters per line */
#define LCD_CHAR_Y       2              /* number of lines */

/* HD44780 has an internal 5x7 font */
#if !defined(FONT_HD44780_INT) && !defined(FONT_HD44780_WIN1251)
#error <<< Font not supported >>>
#endif


/*
 *  HD44780
 *  - 4 bit parallel interface
 *  - if you change LCD_DB4/5/6/7 comment out LCD_DB_STD!
 */

#if 0
#define LCD_PAR_4                       /* 4 bit parallel interface */

/* control and data lines */
#define LCD_PORT         PORTD          /* port data register */
#define LCD_DDR          DDRD           /* port data direction register */
#define LCD_DB_STD                      /* use standard pins 0-3 for DB4-7 */
#define LCD_DB4          PD0            /* port pin used for DB4 */
#define LCD_DB5          PD1            /* port pin used for DB5 */
#define LCD_DB6          PD2            /* port pin used for DB6 */
#define LCD_DB7          PD3            /* port pin used for DB7 */
#define LCD_RS           PD4            /* port pin used for RS */
#define LCD_EN1          PD5            /* port pin used for E */
#endif


/*
 *  HD44780 plus PCF8574 based backpack
 *  - I2C interface using bit-bang I2C
 *  - if you change LCD_DB4/5/6/7 comment out LCD_DB_STD!
 *  - PCF8574T is 0x27, PCF8574AT is 0x3f
 */

#if 0
#define LCD_I2C_ADDR     0x3f           /* PCF8574's I2C address */

/* control and data lines */
#define LCD_DB_STD                      /* use standard pins 4-7 for DB4-7 */
#define LCD_DB4          PCF8574_P4     /* port pin used for DB4 */
#define LCD_DB5          PCF8574_P5     /* port pin used for DB5 */
#define LCD_DB6          PCF8574_P6     /* port pin used for DB6 */
#define LCD_DB7          PCF8574_P7     /* port pin used for DB7 */
#define LCD_RS           PCF8574_P0     /* port pin used for RS */
#define LCD_RW           PCF8574_P1     /* port pin used for RW */
#define LCD_EN1          PCF8574_P2     /* port pin used for E */
#define LCD_LED          PCF8574_P3     /* port pin used for backlight */
//#define LCD_BACKLIGHT_LOW               /* backlight is low active */

/* I2C bus */
#define I2C_BITBANG                     /* bit-bang I2C */
#define I2C_STANDARD_MODE               /* 100kHz bus speed */
#define I2C_PORT         PORTD          /* I2C port data register */
#define I2C_DDR          DDRD           /* I2C port data direction register */
#define I2C_PIN          PIND           /* I2C port input pins register */
#define I2C_SDA          PD0            /* port pin used for SDA */
#define I2C_SCL          PD1            /* port pin used for SCL */
#endif


/* ************************************************************************
 *   addresses
 * ************************************************************************ */

/*
 *  CGROM
 *  - 1240 bytes
 *  - 208 characters 5x8 and 32 characters 5x10
 */


/*
 *  CGRAM
 *  - 64 bytes
 *  - user-defined characters
 *  - 8 characters 5x8 or 4 characters 5x10
 */


/*
 *  DDRAM
 *  - 80 bytes
 */

/* start addresses of text lines */
#define ADDR_LINE_1           0x00    /* line #1 */
#define ADDR_LINE_2           0x40    /* line #2 */
#define ADDR_LINE_3           0x14    /* line #3 */
#define ADDR_LINE_4           0x54    /* line #4 */


/* ************************************************************************
 *   LCD commands
 * ************************************************************************ */

/*
 *  clear display
 *  - 1 byte cmd
 */

#define CMD_CLEAR_DISPLAY     0x01    /* clear display */


/*
 *  return home
 *  - 1 byte cmd
 *  - exec time: 1.52ms
 */

#define CMD_RETURN_HOME       0x02    /* return home */


/*
 *  entry mode set
 *  - 1 byte cmd
 *  - exec time: 37µs
 */

#define CMD_ENTRY_MODE_SET    0x04    /* entry mode set */

#define FLAG_CURSOR_DECREASE  0b00000000     /* auto-decrease cursor position */
#define FLAG_CURSOR_INCREASE  0b00000010     /* auto-increase cursor position */
#define FLAG_DISPLAY_NOSHIFT  0b00000000     /* enable display auto-shift */
#define FLAG_DISPLAY_SHIFT    0b00000001     /* disable display auto-shift */


/*
 *  display control
 *  - 1 byte cmd
 */

#define CMD_DISPLAY_CONTROL   0x08    /* display control */

#define FLAG_DISPLAY_OFF      0b00000000     /* display off */
#define FLAG_DISPLAY_ON       0b00000100     /* display on */
#define FLAG_CURSOR_OFF       0b00000000     /* cursor off */
#define FLAG_CURSOR_ON        0b00000010     /* cursor on */
#define FLAG_BLINK_OFF        0b00000000     /* blinking off */
#define FLAG_BLINK_ON         0b00000001     /* blinking on */


/*
 *  shift
 *  - 1 byte cmd
 */

#define CMD_SHIFT             0x10    /* shift */

#define FLAG_SHIFT_CURSOR     0b00000000     /* shift cursor */
#define FLAG_SHIFT_DISPLAY    0b00001000     /* shift display */
#define FLAG_SHIFT_LEFT       0b00000000     /* shift left */
#define FLAG_SHIFT_RIGHT      0b00000100     /* shift right */


/*
 *  function set
 *  - 1 byte cmd
 */

#define CMD_FUNCTION_SET      0x20    /* function set */

#define FLAG_INTERFACE_4BIT   0b00000000     /* enable 4 bit data interface */
#define FLAG_INTERFACE_8BIT   0b00010000     /* enable 8 bit data interface */
#define FLAG_LINES_1          0b00000000     /* display one line */
#define FLAG_LINES_2          0b00001000     /* display two lines */
#define FLAG_FONT_5X7         0b00000000     /* select 5x7 font */
#define FLAG_FONT_5X10        0b00000100     /* select 5x10 font */


/*
 *  set CG RAM address
 *  - 1 byte cmd
 */

#define CMD_SET_CG_RAM_ADDR   0x40    /* set CG RAM address (custom character) */


/*
 *  set DD RAM address
 *  - 1 byte cmd
 */

#define CMD_SET_DD_RAM_ADDR   0x80    /* set DD RAM address (cursor position) */


#endif // HD44780_H

#endif // LCD_HD44780
