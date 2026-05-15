/* ************************************************************************
 *
 *   ADS7843 functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef TOUCH_ADS7843

#ifndef ADS7843_H
#define ADS7843_H


#if defined (UI_COLORED_TITLES) && defined (UI_CENTER_ALIGN)
#define FUNC_DISPLAY_COLOREDEESTRING_CENTER
#endif


/*
 *  ADS7843 / XPT2046
 *  - SPI interface using hardware SPI
 */

#define TOUCH_PORT       PORTB     /* port data register */
#define TOUCH_DDR        DDRB      /* port data direction register */
#define TOUCH_PIN        PINB      /* port input pins register */
#define TOUCH_CS         PB0       /* port pin used for /CS */
#define TOUCH_PEN        PB1       /* port pin used for /PENIRQ */
//#define TOUCH_FLIP_X               /* enable horizontal flip */
//#define TOUCH_FLIP_Y               /* enable vertical flip */
//#define TOUCH_ROTATE               /* switch X and Y (rotate by 90°) */

#define SPI_HARDWARE               /* hardware SPI */
#define SPI_RW                     /* enable SPI read support */


extern void Touch_BusSetup(void);
extern void Touch_Init(void);
extern uint8_t Touch_Check(void);
extern uint8_t Touch_Adjust(void);


#endif // ADS7843_H

#endif // TOUCH_ADS873
