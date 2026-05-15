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
#include "tool/all.h"
#include "display/all.h"

#include "spi_bb.h"
#include "spi_hw.h"
#include "i2c.h"

#include "resistor.h"
#include "inductor.h"
#include "semi.h"
#include "cap.h"

#include "serial.h"
#include "probes.h"

#include "display.h"
#include "user.h"
#include "main.h"
#include "adc.h"
#include "adjust.h"
#include "commands.h"
#include "pause.h"
#include "wait.h"

#include "lang/var.h"


/* ************************************************************************
 *   constants for arrays in variables.h
 * ************************************************************************ */

/* number of entries in data tables */
#define NUM_TIMER1            5         /* Timer1 prescalers and bits */


/* ************************************************************************
 *   constants for probing (Check.Type)
 * ************************************************************************ */

/* error type IDs */
#define TYPE_DISCHARGE        1    /* discharge error */
#define TYPE_DETECTION        2    /* detection error */

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


extern Config_Type     Cfg;        /* offsets and values */

/* probing */
extern Check_Type      Check;      /* checking/testing */


#ifdef HW_TOUCH

extern Touch_Type    Touch;              /* touch screen adjustment offsets */


/* touch screen adjustment offsets */
extern const Touch_Type   NV_Touch;

#endif // HW_TOUCH


/*
 *  constant tables
 *  - stored in EEPROM/Flash
 */

#ifdef VAR_TIMER1_TABLES
/* Timer1 prescalers and corresponding register bits */
extern const uint16_t T1_Prescaler_table[];
extern const uint8_t T1_RegBits_table[];
#endif // VAR_TIMER1_TABLES


#ifdef HW_TOUCH
extern const unsigned char TouchSetup_str[];
#endif


#endif // COMMON_H
