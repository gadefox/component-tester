/* ************************************************************************
 *
 *   SEMI_ST7735 color graphic display controller
 *
 *   (c) 2016-2020 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifdef LCD_VT100

#ifndef VT100_H
#define VT100_H


#if !defined(SERIAL_BITBANG) && !defined(SERIAL_HARDWARE)
#error <<< VT100 requires TTL serial >>>
#endif

#warning <<< VT100 display driver disables other options for serial interface >>>
#undef UI_SERIAL_COPY
#undef UI_SERIAL_COMMANDS
#undef UI_BATTERY


/* ************************************************************************
 *   configure
 * ************************************************************************ */

/*
 *  VT100 serial terminal, TTL serial
 */

#define LCD_TEXT                        /* character display */
#define LCD_COLOR                       /* color display */
#define LCD_CHAR_X       40             /* characters per line */
#define LCD_CHAR_Y       24             /* number of lines */

#define SERIAL_BITBANG                  /* bit-bang serial */
//#define SERIAL_HARDWARE                 /* hardware serial */


/*
 *  VT100 serial terminal
 *  - 8 fixed foreground and background colors
 *  - foreground colors: 30-37
 *  - background colors: 40-47
 *  - firmware adds automatically 10 for background colors
 */

/* foreground */
#define COLOR_BLACK         30
#define COLOR_RED           31
#define COLOR_GREEN         32
#define COLOR_YELLOW        33
#define COLOR_BLUE          34
#define COLOR_MAGENTA       35
#define COLOR_CYAN          36
#define COLOR_WHITE         37


/* ************************************************************************
 *   system function commands
 * ************************************************************************ */


#endif // VT100_H

#endif // LCD_VT100
