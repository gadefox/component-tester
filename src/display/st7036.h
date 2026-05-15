/* ************************************************************************
 *
 *   ST7036 display controller
 *
 *   (c) 2019-2020 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifdef LCD_ST7036

#ifndef ST7036_H
#define ST7036_H


/* ************************************************************************
 *   configure
 * ************************************************************************ */

#define LCD_TEXT                        /* character display */

/* display settings */
#define LCD_CHAR_X       16             /* characters per line */
#define LCD_CHAR_Y       3              /* number of lines */
#define LCD_EXTENDED_CMD                /* extended instruction set (EXT pin high) */
#define LCD_CONTRAST     32             /* default contrast (0-63) */

/* ST7036 has internal 5x8 font */
#if !defined(FONT_ST7036)
#error <<< Font not supported >>>
#endif


/*
 *  ST7036
 *  - 4 bit parallel interface
 *  - enable LCD_DB_STD when using port pins 0-3 for LCD_DB4/5/6/7
 *  - untested!!!
 */

#if 0
#define LCD_PAR_4                       /* 4 bit parallel interface */

/* control and data lines */
#define LCD_PORT         PORTB          /* port data register */
#define LCD_DDR          DDRB           /* port data direction register */
//#define LCD_DB_STD                      /* use standard pins 0-3 for DB4-7 */
#define LCD_DB4          PB4            /* port pin used for DB4 */
#define LCD_DB5          PB5            /* port pin used for DB5 */
#define LCD_DB6          PB6            /* port pin used for DB6 */
#define LCD_DB7          PB7            /* port pin used for DB7 */
#define LCD_RS           PB2            /* port pin used for RS */
#define LCD_EN           PB3            /* port pin used for E */
//#define LCD_RW           ???            /* port pin used for R/W (optional) */
//#define LCD_RESET        ???            /* port pin used for XRESET (optional) */
#endif

/*
 *  ST7036
 *  - 4 wire SPI interface using bit-bang SPI
 *  - untested!!!
 */

#if 0
/* control and data lines */
#define LCD_PORT         PORTB          /* port data register */
#define LCD_DDR          DDRB           /* port data direction register */
#define LCD_RESET        PB2            /* port pin used for XRESET (optional) */
#define LCD_CS           PB4            /* port pin used for CSB (optional) */
#define LCD_RS           PB3            /* port pin used for RS */
#define LCD_SCL          PB7            /* port pin used for SCL */
#define LCD_SI           PB5            /* port pin used for SI (data input) */

/* SPI bus */
#define SPI_BITBANG                     /* bit-bang SPI */
#define SPI_PORT         LCD_PORT       /* SPI port data register */
#define SPI_DDR          LCD_DDR        /* SPI port data direction register */
#define SPI_SCK          LCD_SCL        /* port pin used for SCK */
#define SPI_MOSI         LCD_SI         /* port pin used for MOSI */
#endif


/* ************************************************************************
 *   I2C
 * ************************************************************************ */

/*
 *  I2C slave addreses
 */

#define ST7036_I2C_ADDR_0     0b00111100     /* A1=0 A0=0 0x3c */
#define ST7036_I2C_ADDR_1     0b00111101     /* A1=0 A0=1 0x3d */
#define ST7036_I2C_ADDR_2     0b00111110     /* A1=1 A0=0 0x3e */
#define ST7036_I2C_ADDR_3     0b00111111     /* A1=1 A0=1 0x3f */


/*
 *  I2C control byte
 */

#define LCD_CONTROL_BYTE      0b00000000     /* 0x00 */
/* continuation flag */
#define FLAG_CTRL_MULTI       0b00000000     /* stream of bytes follows */
#define FLAG_CTRL_SINGLE      0b10000000     /* only 1 byte follows */
/* RS */
#define FLAG_CTRL_CMD         0b00000000     /* command follows */
#define FLAG_CTRL_DATA        0b01000000     /* data follows */


/* ************************************************************************
 *   commands
 * ************************************************************************ */

/*
 *  clear display
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 1.08ms
 */

#define CMD_CLEAR             0b00000001     /* clear display */


/*
 *  return home
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 1.08ms
 */

#define CMD_HOME              0b00000010     /* return home */


/*
 *  entry mode set
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 26.3탎
 */

#define CMD_ENTRY_MODE        0b00000100     /* entry mode set */

#define FLAG_CURSOR_DECREASE  0b00000000     /* auto-decrease cursor position */
#define FLAG_CURSOR_INCREASE  0b00000010     /* auto-increase cursor position */
#define FLAG_DISPLAY_NOSHIFT  0b00000000     /* enable display auto-shift */
#define FLAG_DISPLAY_SHIFT    0b00000001     /* disable display auto-shift */


/*
 *  display control
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 26.3탎
 */

#define CMD_DISPLAY_CONTROL   0b00001000     /* display control */

#define FLAG_DISPLAY_OFF      0b00000000     /* display off */
#define FLAG_DISPLAY_ON       0b00000100     /* display on */
#define FLAG_CURSOR_OFF       0b00000000     /* cursor off */
#define FLAG_CURSOR_ON        0b00000010     /* cursor on */
#define FLAG_BLINK_OFF        0b00000000     /* blinking off */
#define FLAG_BLINK_ON         0b00000001     /* blinking on */


/*
 *  cursor/display shift
 *  - 1 byte cmd
 *  - modes: normal & extension (instruction table 0)
 *  - exec time: 26.3탎
 */

#define CMD_SHIFT             0b00010000     /* shift */

#define FLAG_SHIFT_CURSOR     0b00000000     /* shift cursor */
#define FLAG_SHIFT_DISPLAY    0b00001000     /* shift display */
#define FLAG_SHIFT_LEFT       0b00000000     /* shift left */
#define FLAG_SHIFT_RIGHT      0b00000100     /* shift right */


/*
 *  bias set
 *  - 1 byte cmd
 *  - modes: extension (instruction table 1)
 *  - exec time: 26.3탎
 */

#define CMD_BIAS              0b00010100     /* set bias */

#define FLAG_BIAS_5           0b00000000     /* bias 1/5 */
#define FLAG_BIAS_4           0b00001000     /* bias 1/4 */
#define FLAG_12LINE           0b00000000     /* 1 or 2-line mode */
#define FLAG_3LINE            0b00000001     /* 3-line mode */


/*
 *  double height position select (when 5x16 font is enabled)
 *  - 1 byte cmd
 *  - modes: extension (instruction table 2)
 *  - exec time: 26.3탎
 */

#define CMD_DOUBLEHEIGHT_POS  0b00010000     /* double height position */

#define FLAG_DOUBLE_BOTTOM    0b00000000     /* 5x16 font for bottom line */
#define FLAG_DOUBLE_TOP       0b00001000     /* 5x16 font for top line */


/*
 *  function set
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 26.3탎
 */

#define CMD_FUNCTION_SET      0b00100000     /* function set */

#define FLAG_INTERFACE_4BIT   0b00000000     /* enable 4 bit data interface */
#define FLAG_INTERFACE_8BIT   0b00010000     /* enable 8 bit data interface */
#define FLAG_LINES_1          0b00000000     /* display one line */
#define FLAG_LINES_2          0b00001000     /* display two lines */
                                             /* must be also set for 3-line mode */

/* additional flags for extension mode */
#define FLAG_FONT_5X8         0b00000000     /* select 5x8 font */
#define FLAG_FONT_5X16        0b00000100     /* select 5x16 font */
#define FLAG_INST_TABLE_0     0b00000000     /* instruction table 0 */
#define FLAG_INST_TABLE_1     0b00000001     /* instruction table 1 */
#define FLAG_INST_TABLE_2     0b00000010     /* instruction table 2 */


/*
 *  set CGRAM address (custom character)
 *  - 1 byte cmd
 *  - modes: normal & extension (instruction table 0)
 *  - exec time: 26.3탎
 */

#define CMD_SET_CGRAM         0b01000000     /* set CGRAM address  */


/*
 *  set ICON address (?)
 *  - 1 byte cmd
 *  - modes: extension (instruction table 1)
 *  - exec time: 26.3탎
 *  - valid address range: 00-0F
 */

#define CMD_SET_ICON          0b01000000     /* set ICON address  */


/*
 *  power, ICON control and contrast set (high bits)
 *  - 1 byte cmd
 *  - modes: extension (instruction table 1)
 *  - exec time: 26.3탎
 *  - contrast: upper 2 bits (of 6 bits, valid range 0-63)
 */

#define CMD_MISC_CONTROL      0b01010000     /* set misc stuff */

#define FLAG_ICON_OFF         0b00000000     /* disable display of ICON */
#define FLAG_ICON_ON          0b00001000     /* enable display of ICON */
#define FLAG_BOOSTER_OFF      0b00000000     /* disable booster circuit */
#define FLAG_BOOSTER_ON       0b00000100     /* enable booster circuit */
/* bits 0-1: upper 2 bits of contrast */


/*
 *  voltage follower control
 *  - 1 byte cmd
 *  - modes: extension (instruction table 1)
 *  - exec time: 26.3탎
 */

#define CMD_FOLLOWER          0b01100000     /* follower control */

#define FLAG_FOLLOWER_OFF     0b00000000     /* disable internal follower */
#define FLAG_FOLLOWER_ON      0b00001000     /* enable internal follower */
/* bits 0-2: V0 generator ratio */
#define FLAG_FOLLOWER_100     0b00000000     /* factor 1.00 */
#define FLAG_FOLLOWER_125     0b00000001     /* factor 1.25 */
#define FLAG_FOLLOWER_150     0b00000010     /* factor 1.50 */
#define FLAG_FOLLOWER_180     0b00000011     /* factor 1.80 */
#define FLAG_FOLLOWER_200     0b00000100     /* factor 2.00 */
#define FLAG_FOLLOWER_250     0b00000101     /* factor 2.50 */
#define FLAG_FOLLOWER_300     0b00000110     /* factor 3.00 */
#define FLAG_FOLLOWER_375     0b00000111     /* factor 3.75 */


/*
 *  contrast set (low bits)
 *  - 1 byte cmd
 *  - modes: extension (instruction table 1)
 *  - exec time: 26.3탎
 *  - contrast: lower 4 bits (of 6 bits, valid range 0-63)
 */

#define CMD_CONTRAST          0b01110000     /* set contrast */


/*
 *  set DDRAM address (cursor position)
 *  - 1 byte cmd
 *  - modes: normal & extension
 *  - exec time: 26.3탎
 *  - valid address ranges:
 *    1-line mode: 00-4F
 *    2-line mode: 00-27, 40-67
 *    3-line mode: 00-0F, 10-1F, 20-2F 
 */

#define CMD_SET_DDRAM         0b10000000    /* set DDRAM address */


#endif // ST7036_H

#endif // LCD_ST7036
