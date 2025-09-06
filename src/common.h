/* ************************************************************************
 *
 *   common header file
 *
 *   (c) 2012-2025 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *
 * ************************************************************************ */

#ifndef COMMON_H
#define COMMON_H

/*
 *  include header files
 */

/* basic includes */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* AVR */
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

/* local includes */
#include "cfg/options.h"           /* global configuration */
#include "font/all.h"
#include "symbol/all.h"
#include "display/all.h"
#include "tool/all.h"

#include "display.h"
#include "user.h"
#include "main.h"
#include "adc.h"
#include "adjust.h"
#include "cap.h"
#include "commands.h"
#include "i2c.h"
#include "resistor.h"
#include "inductor.h"
#include "pause.h"
#include "probes.h"
#include "semi.h"
#include "serial.h"
#include "spi.h"
#include "wait.h"


/* ************************************************************************
 *   constants for arrays in variables.h
 * ************************************************************************ */

/* manage RZero */
#ifdef R_MULTIOFFSET
  #define NV_R_ZERO         {R_ZERO, R_ZERO, R_ZERO}
#else
  #define NV_R_ZERO         R_ZERO
#endif

/* string buffer sizes */
#define OUT_BUFFER_SIZE       12        /* 11 chars + terminating 0 */
#define RX_BUFFER_SIZE        11        /* 10 chars + terminating 0 */

/* number of entries in data tables */
#define NUM_PREFIXES          8         /* unit prefixes */
#define NUM_LARGE_CAP         46        /* large cap factors */
#define NUM_SMALL_CAP         9         /* small cap factors */
#define NUM_PWM_FREQ          8         /* PWM frequencies */
#define NUM_INDUCTOR          32        /* inductance factors */
#define NUM_TIMER1            5         /* Timer1 prescalers and bits */
#define NUM_PROBE_COLORS      3         /* probe colors */
#define NUM_E6                6         /* E6 norm values */
#define NUM_E12              12         /* E12 norm values */
#define NUM_E24              24         /* E24 norm values */
#define NUM_E96              96         /* E24 norm values */
#define NUM_COLOR_CODES      10         /* color codes */
#define NUM_EIA96_MULT        9         /* EIA-96 multiplier codes */
#define NUM_LOGIC_TYPES       6         /* logic families and voltages */


/*
 *  flags for Info_Type
 */

#define INFO_NONE             0b00000000     /* no flags set */

/* resistor */
#define INFO_R_L              0b00000001     /* measured inductance */

/* diode */
#define INFO_D_R_BE           0b00000001     /* detected B-E resistor */
#define INFO_D_BJT_NPN        0b00000010     /* possible NPN BJT */
#define INFO_D_BJT_PNP        0b00000100     /* possible PNP BJT */
#define INFO_D_I_R            0b00001000     /* measured reverse leakage current */
#define INFO_D_CAP1           0b00010000     /* diode #1: measured capacitance */
#define INFO_D_CAP2           0b00100000     /* diode #2: measured capacitance */

/* BJT */
#define INFO_BJT_D_FB         0b00000001     /* detected flyback diode */
#define INFO_BJT_R_BE         0b00000010     /* detected B-E resistor */
#define INFO_BJT_SCHOTTKY     0b00000100     /* detected Schottky-clamped BJT */

/* FET/IGBT */
#define INFO_FET_D_FB         0b00000001     /* detected body/flyback diode */
#define INFO_FET_SYM          0b00000010     /* symmetrical drain and source */
#define INFO_FET_V_TH         0b00000100     /* measured Vth */
#define INFO_FET_C_GS         0b00001000     /* measured C_GS */
#define INFO_FET_R_DS         0b00010000     /* measured R_DS_on */


/* ************************************************************************
 *   constants for probing
 * ************************************************************************ */

/* component IDs */
/* non-components */
#define COMP_NONE             0
#define COMP_ERROR            1
/* passive components */
#define COMP_RESISTOR        10
#define COMP_CAPACITOR       11
#define COMP_INDUCTOR        12
/* 2 pin semiconductors */
#define COMP_DIODE           20
#define COMP_ZENER           21
/* 3 pin semiconductors */
#define COMP_BJT             30
#define COMP_FET             31
#define COMP_IGBT            32
#define COMP_TRIAC           33
#define COMP_THYRISTOR       34
#define COMP_PUT             35
#define COMP_UJT             36


/* error type IDs */
#define TYPE_DISCHARGE        1    /* discharge error */
#define TYPE_DETECTION        2    /* detection error */


/* multiplicator table IDs */
#define TABLE_SMALL_CAP       1              /* table for small caps */
#define TABLE_LARGE_CAP       2              /* table for large caps */
#define TABLE_INDUCTOR        3              /* table for inductors */


/* ************************************************************************
 *   structures
 * ************************************************************************ */

/* touch screen adjustment values (stored in EEPROM) */
typedef struct
{
  uint16_t          X_Start;       /* X start value */
  uint16_t          X_Stop;        /* X stop value */
  uint16_t          Y_Start;       /* Y start value */
  uint16_t          Y_Stop;        /* Y stop value */
  uint8_t           CheckSum;      /* checksum for stored values */
} Touch_Type;

/* tester modes, states, offsets and values */
typedef struct
{
  uint8_t           OP_Mode;       /* operation mode & state flags */
  volatile uint8_t  OP_Control;    /* operation control & signal flags */
#ifdef SAVE_POWER
  uint8_t           SleepMode;     /* MCU sleep mode */
#endif
  uint8_t           Samples;       /* number of ADC samples */
  uint8_t           AutoScale;     /* flag to disable/enable ADC auto scaling */
  uint8_t           Ref;           /* track reference source used lastly */
  uint16_t          Bandgap;       /* voltage of internal bandgap reference (mV) */
  uint16_t          Vcc;           /* voltage of Vcc (mV) */
#ifndef BAT_NONE
  uint16_t          Vbat;          /* battery voltage (mV) */
  uint8_t           BatTimer;      /* timer for battery check (100ms) */
#endif
#ifdef SW_DISPLAY_ID
  uint16_t          DisplayID;     /* ID of display controller */
#endif
} Config_Type;


/* checking/probing */
typedef struct
{
  uint8_t           Found;         /* component type */ 
  uint8_t           Type;          /* component specific subtype */
  uint8_t           Done;          /* flag for transistor detection done */
  uint8_t           AltFound;      /* alternative component type */
  uint8_t           Resistors;     /* number of resistors found */
  uint8_t           Diodes;        /* number of diodes found */
  uint8_t           Probe;         /* error: probe pin */ 
  uint16_t          U;             /* error: voltage in mV */
#ifdef SW_SYMBOLS
  uint8_t           Symbol;        /* symbol ID */
  uint8_t           AltSymbol;     /* symbol ID for alternative component */
#endif
} Check_Type;


extern Config_Type     Cfg;                /* offsets and values */

/* probing */
extern Check_Type      Check;              /* checking/testing */

#ifdef HW_TOUCH
extern Touch_Type    Touch;              /* touch screen adjustment offsets */
#endif


#ifdef HW_TOUCH
/* touch screen adjustment offsets */
extern const Touch_Type   NV_Touch;
#endif

/*
 *  constant strings
 *  - stored in EEPROM/Flash
 */

/* units */
extern const unsigned char Hertz_str[];


/* options */
#ifdef VAR_START_STR
extern const unsigned char Start_str[];
#endif


/*
 *  constant tables
 *  - stored in EEPROM/Flash
 */

#ifdef VAR_TIMER1_TABLES
/* Timer1 prescalers and corresponding register bits */
extern const uint16_t T1_Prescaler_table[];
extern const uint8_t T1_RegBits_table[];
#endif // VAR_TIMER1_TABLES


/* ************************************************************************
 *   functions from LCD module specific driver
 *   NOTE: We assume the tester has display
 *
 * ************************************************************************ */

extern void LCD_BusSetup(void);
extern void LCD_Init(void);
extern void LCD_ClearLine(uint8_t Line);
extern void LCD_Clear(void);
extern void LCD_CharPos(uint8_t x, uint8_t y);
extern void LCD_Cursor(uint8_t Mode);
extern void LCD_Char(unsigned char Char);

#if !defined(UI_SERIAL_COPY) && !defined(UI_SERIAL_COMMANDS)
/* make Display_Char() an alias for LCD_Char() */
#define Display_Char LCD_Char
#endif

#ifdef SW_CONTRAST
extern void LCD_Contrast(uint8_t Contrast);
#endif

#ifdef SW_SYMBOLS
extern void LCD_Symbol(uint8_t ID);
#endif

#ifdef FUNC_COLORCODE
extern void LCD_Band(uint16_t Color, uint8_t Align);
#endif


/*
 * language
 */

/* firmware */
extern const unsigned char Tester_str[];

/* common terms and texts */
extern const unsigned char Probing_str[];
extern const unsigned char Timeout_str[];
extern const unsigned char Failed1_str[];
extern const unsigned char Failed2_str[];
extern const unsigned char Done_str[];
extern const unsigned char Select_str[];
extern const unsigned char Adjustment_str[];
extern const unsigned char Save_str[];
extern const unsigned char Load_str[];
extern const unsigned char Show_str[];
extern const unsigned char Remove_str[];
extern const unsigned char Create_str[];
extern const unsigned char ShortCircuit_str[];
extern const unsigned char DischargeFailed_str[];
extern const unsigned char Error_str[];
extern const unsigned char Exit_str[];
extern const unsigned char Checksum_str[];
extern const unsigned char BJT_str[];
extern const unsigned char Thyristor_str[];
extern const unsigned char Triac_str[];
extern const unsigned char PUT_str[];
extern const unsigned char Bye_str[];

/* units */
extern const unsigned char Hertz_str[];

/* options */
#ifdef VAR_START_STR
extern const unsigned char Start_str[];
#endif

#ifdef HW_TOUCH
extern const unsigned char TouchSetup_str[];
#endif


#endif // COMMON_H
