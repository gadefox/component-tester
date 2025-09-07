/* ************************************************************************
 *
 *   zener tool (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

/* local includes */
#include "../common.h"                /* common header file */


/*
 *  local constants
 */


/*
 *  local variabless
 */


/* ************************************************************************
 *   Zener tool / external voltage
 * ************************************************************************ */

#if defined (HW_ZENER) && ! defined (ZENER_UNSWITCHED)

/*
 *  Zener tool (standard mode)
 *  - hardware option for voltage measurement of Zener diode 
 *  - uses dedicated analog input (TP_ZENER) with voltage divider
 *    (default 10:1)
 *  - boost converter enabled by test push button (default) or
 *    dedicated I/O pin (switched mode)
 */

// Zener Tool states
#define ZENER_EXIT         0   // Exit tool
#define ZENER_IDLE         1   // Waiting for key press
#define ZENER_PRESSED      2   // Key is currently pressed

void Zener_Tool(void)
{
  uint8_t                Mode = ZENER_IDLE;  /* control flag */
  uint8_t                Counter;            /* length of key press */
  uint8_t                Counter2 = 0;       /* time between two key presses */
  uint16_t               U1;                 /* current voltage */
  uint16_t               Min = UINT16_MAX;   /* minimal voltage */
#ifdef ZENER_DIVIDER_CUSTOM
  uint32_t               Value;              /* value */
#endif

  /* show info */
  LCD_Clear();

#ifdef UI_COLORED_TITLES
  /* display: Zener */
  Display_ColoredEEString(Zener_str, COLOR_TITLE);
#else
  Display_EEString(Zener_str);   /* display: Zener */
#endif

  Display_NextLine();              /* line #2 */
  Display_Minus();                 /* display "no value" */

#ifdef UI_ZENER_DIODE
  /* display Zener diode symbol */
  Check.Symbol = SYMBOL_DIODE_ZENER;    /* set symbol ID */
  Display_FancySemiPinout(3);           /* show symbol starting in line #3 */
#endif

  /*
   *  processing loop
   */

  while (Mode != ZENER_EXIT)
  {
    /*
     *  manage timing
     */

    Counter = 0;              /* reset key press time */
    MilliSleep(30);           /* delay by 30ms */
    Counter2++;               /* increase delay time counter */
    if (Counter2 > 200)       /* prevent overflow & timer (about 6s) */
    {
      Counter2 = 10;          /* reset counter (above delay for quick key presses) */
#ifndef BAT_NONE
      CheckBattery();         /* and check battery */
#endif
    }

    /*
     *  key press triggers measurement
     *  - also enables boost converter via hardware
     *  - two short key presses exit tool
     */

    while (!(BUTTON_PIN & (1 << TEST_BUTTON)))    /* as long as key is pressed */
    {
#ifdef ZENER_SWITCHED
      /* turn on boost converter */
#ifdef ZENER_BOOST_HIGH
      /* high active */
      BOOST_PORT |= (1 << BOOST_CTRL);        /* set pin high */
#else
      /* low active */
      BOOST_PORT &= ~(1 << BOOST_CTRL);       /* set pin low */
#endif
#endif // ZENER_SWITCHED

      /* get voltage (10:1 voltage divider) */
      U1 = ReadU(TP_ZENER);        /* read voltage (in mV) */

#ifndef ZENER_DIVIDER_CUSTOM
      /* ADC pin is connected to a 10:1 voltage divider */
      /* so U1's scale is 10mV */
#endif

#ifdef ZENER_DIVIDER_CUSTOM
      /*
       *  ADC pin is connected to a voltage divider (top: R1 / bottom: R2).
       *  - U2 = (Uin / (R1 + R2)) * R2 
       *  - Uin = (U2 * (R1 + R2)) / R2
       */

      Value = (((uint32_t)(ZENER_R1 + ZENER_R2) * 1000) / ZENER_R2);  /* factor (0.001) */
      Value *= U1;                     /* voltage (0.001 mV) */
      Value /= 1000;                   /* scale to mV */
      U1 = (uint16_t)Value;            /* keep 2 bytes */
#endif

      /* display voltage */
      if (Counter % 8 == 0)        /* every 8 loop runs (240ms) */
      {
        LCD_ClearLine2();               /* clear line #2 */
#ifndef ZENER_DIVIDER_CUSTOM
        Display_Value(U1, -2, 'V');   /* display current voltage (10mV) */
#else
        Display_Value(U1, -3, 'V');   /* display current voltage (1mV) */
#endif
      }

      /* data hold */
      if (Counter == 0)            /* first loop run */
        Min = UINT16_MAX;          /* reset minimum to default */
      else if (Counter >= 10)      /* ensure stable voltage */
        if (U1 < Min) Min = U1;    /* update minimum */

      /* timing */
      MilliSleep(30);              /* delay next run / also debounce by 30ms */
      Counter++;                   /* increase key press time counter */
      if (Counter > 100)           /* prevent overflow & timer (about 3s) */
      {
        Counter = 12;              /* reset counter (above time for short key press) */
#ifndef BAT_NONE
        CheckBattery();            /* and check battery */
#endif
      }
    }

#ifdef ZENER_SWITCHED
    /* turn off boost converter */
#ifdef ZENER_BOOST_HIGH
    /* high active */
    BOOST_PORT &= ~(1 << BOOST_CTRL);    /* set pin low */
#else
    /* low active */
    BOOST_PORT |= (1 << BOOST_CTRL);     /* set pin high */
#endif
#endif // ZENER_SWITCHED

    /*
     *  user interface logic
     */

    if (Counter > 0)                         /* key was pressed */
    {
      /* detect two quick key presses */
      if (Mode == ZENER_PRESSED)              /* flag for short key press set */
      {
        if (Counter2 <= 8)                   /* short delay between key presses <= 250ms */
          Mode = ZENER_EXIT;                 /* end loop */
        else                                 /* long delay between key presses */
          Mode = ZENER_IDLE;                 /* reset flag */
      }
      else                                   /* flag not set */
      {
        if (Counter <= 10)                   /* short key press <= 300ms */
          Mode = ZENER_PRESSED;              /* set flag */
      }

      /* display hold value */
      LCD_ClearLine2();

      if (Min != UINT16_MAX)       /* got updated value */
      {
#ifndef ZENER_DIVIDER_CUSTOM
        Display_Value(Min, -2, 'V');     /* display minimal voltage */
#else
        Display_Value(Min, -3, 'V');     /* display minimal voltage */
#endif

        Display_Space();
        Display_EEString(Min_str);         /* display: Min */
      }
      else                         /* unchanged default */
        Display_Minus();                   /* display "no value" */

      Counter2 = 0;                /* reset delay time */
    }
  }
}

// Clear states
#undef ZENER_EXIT
#undef ZENER_IDLE
#undef ZENER_KEY_PRESSED

#endif // HW_ZENER && !ZENER_UNSWITCHED


#if defined (HW_ZENER) && defined (ZENER_UNSWITCHED)

/*
 *  Zener tool (alternative mode: unswitched boost converter)
 *  - hardware option for voltage measurement of Zener diode
 *    or external voltage
 *  - uses dedicated analog input (TP_ZENER) with voltage divider
 *    (default 10:1)
 *  - boost converter runs all the time or circuit without boost
 *    converter (just measuring external voltage)
 */

void Zener_Tool(void)
{
  uint8_t                Test;          /* user feedback */
  uint16_t               U1;            /* voltage */
#ifdef ZENER_DIVIDER_CUSTOM
  uint32_t               Value;         /* value */
#endif

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: Zener */
    Display_ColoredEEString(Zener_str, COLOR_TITLE);
#else
    Display_EEString(Zener_str);   /* display: Zener */
#endif

#ifdef UI_ZENER_DIODE
  /* display Zener diode symbol */
  Check.Symbol = SYMBOL_DIODE_ZENER;    /* set symbol ID */
  Display_FancySemiPinout(3);           /* show symbol starting in line #3 */
#endif

  /*
   *  processing loop
   */

  while (1)
  {
    /* get voltage */
    U1 = ReadU(TP_ZENER);          /* read voltage (in mV) */

#ifndef ZENER_DIVIDER_CUSTOM
    /* ADC pin is connected to a 10:1 voltage divider */
    /* so U1's scale is 10mV */
#endif

#ifdef ZENER_DIVIDER_CUSTOM
    /*
     *  ADC pin is connected to a voltage divider (top: R1 / bottom: R2).
     *  - U2 = (Uin / (R1 + R2)) * R2 
     *  - Uin = (U2 * (R1 + R2)) / R2
     */

    Value = (((uint32_t)(ZENER_R1 + ZENER_R2) * 1000) / ZENER_R2);  /* factor (0.001) */
    Value *= U1;                     /* voltage (0.001 mV) */
    Value /= 1000;                   /* scale to mV */
    U1 = (uint16_t)Value;            /* keep 2 bytes */
#endif

    /* display voltage */
    LCD_ClearLine2();              /* clear line #2 */
#ifndef ZENER_DIVIDER_CUSTOM
      Display_Value(U1, -2, 'V');  /* display current voltage */
#else
      Display_Value(U1, -3, 'V');  /* display current voltage */
#endif

    /* user feedback (1s delay) */
    Test = TestKey(1000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      return;                      /* end processing loop */
  }
}

#endif // HW_ZENER || HW_ZENER_UNSWITCHED


#ifdef HW_PROBE_ZENER

/*
 *  check for Zener diode
 *  - hardware option for voltage measurement of Zener diode
 *    or external voltage
 *  - uses dedicated analog input (TP_ZENER) with voltage divider
 *    (default 10:1)
 *  - boost converter runs all the time, circuit without boost
 *    converter (just measuring external voltage), or boost converter
 *    switched by dedicated I/O pin
 */

void CheckZener(void)
{
  uint16_t               U1;            /* voltage */
#ifdef ZENER_DIVIDER_CUSTOM
  uint32_t               Value;         /* value */
#endif

#ifdef ZENER_SWITCHED
  /* turn on boost converter */
#ifdef ZENER_BOOST_HIGH
  /* high active */
  BOOST_PORT |= (1 << BOOST_CTRL);       /* set pin high */
#else
  /* low active */
  BOOST_PORT &= ~(1 << BOOST_CTRL);      /* set pin low */
#endif
  MilliSleep(300);                           /* time for stabilization */
#endif // ZENER_SWITCHED

  /* get voltage */
  U1 = ReadU(TP_ZENER);            /* read voltage (in mV) */

#ifdef ZENER_SWITCHED
  /* turn off boost converter */
#ifdef ZENER_BOOST_HIGH
      /* high active */
      BOOST_PORT &= ~(1 << BOOST_CTRL);      /* set pin low */
#else
      /* low active */
      BOOST_PORT |= (1 << BOOST_CTRL);       /* set pin high */
#endif
#endif // ZENER_SWITCHED

#ifndef ZENER_DIVIDER_CUSTOM
  /* ADC pin is connected to a 10:1 voltage divider */
  U1 *= 10;                        /* voltage (mV) */
#endif

#ifdef ZENER_DIVIDER_CUSTOM
  /*
   *  ADC pin is connected to a voltage divider (top: R1 / bottom: R2).
   *  - U2 = (Uin / (R1 + R2)) * R2 
   *  - Uin = (U2 * (R1 + R2)) / R2
   */

  Value = (((uint32_t)(ZENER_R1 + ZENER_R2) * 1000) / ZENER_R2);  /* factor (0.001) */
  Value *= U1;                     /* voltage (0.001 mV) */
  Value /= 1000;                   /* scale to mV */
  U1 = (uint16_t)Value;            /* keep 2 bytes */
#endif

  /* check for valid voltage */
  if ((U1 >= ZENER_VOLTAGE_MIN) && (U1 <= ZENER_VOLTAGE_MAX))
  {
    Check.Found = COMP_ZENER;      /* we got a Zener */
    Semi.U_1 = U1;                 /* save voltage (V_Z) */
  }
}


#endif // HW_PROBE_ZENER
