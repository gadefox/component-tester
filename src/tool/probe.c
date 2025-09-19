/* ************************************************************************
 *
 *   probe tools (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef HW_LOGIC_PROBE

/*
 *  local constants
 */


/*
 *  local variabless
 */


/* ************************************************************************
 *   logic probe
 * ************************************************************************ */

const unsigned char TTL_str[] MEM_TYPE = "TTL";
const unsigned char CMOS_str[] MEM_TYPE = "CMOS";

/* logic families and voltages */
/* Vcc in mV                                                TTL   CMOS  CMOS  CMOS  CMOS   CMOS */
const uint16_t Logic_Vcc_table[NUM_LOGIC_TYPES] MEM_TYPE = {5000, 3300, 5000, 9000, 12000, 15000};


/*
 *  Logic Probe
 *  - analog input: TP_LOGIC
 *  - uses voltage divider (default: 4:1)
 *    LOGIC_PROBE_R1 and LOGIC_PROBE_R2
 */

void LogicProbe(void)
{
  uint8_t           Flag;               /* loop control */
  uint8_t           Test;               /* user feedback */
  uint8_t           Item;               /* menu item */
  uint8_t           VccIndex;           /* index for Vcc table */
  unsigned char     State;              /* logic state */
  uint16_t          U1;                 /* measured voltage */
  uint16_t          U_max = 0;          /* Vcc/Vdd */
  uint16_t          U_low = 0;          /* voltage threshold for low */
  uint16_t          U_high = 0;         /* voltage threshold for high */
  uint32_t          Value;              /* temporary value */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run / otherwise end */
  #define CHANGE_TYPE         0b00000010     /* change logic type */
  #define CHANGE_LOW          0b00001000     /* change low threshold */
  #define CHANGE_HIGH         0b00010000     /* change high threshold */

  /* local constants for Item */
  #define ITEM_TYPE      1              /* logic type (family and Vcc) */
  #define ITEM_LOW       2              /* voltage threshold for low */
  #define ITEM_HIGH      3              /* voltage threshold for high */

  /*
   *  show info
   */

  LCD_Clear();
#ifdef UI_COLORED_TITLES
  /* display: Logic Probe */
  Display_ColoredEEString(LogicProbe_str, COLOR_TITLE);
#else
  Display_EEString(LogicProbe_str);   /* display: Logic Probe */
#endif

  /*
   *  init
   */

  ADC_DDR &= ~(1 << TP_LOGIC);          /* set pin to HiZ */
  Cfg.Samples = 5;                      /* do just 5 samples to be fast */
  VccIndex = 0;                         /* TTL 5V */
  Item = ITEM_TYPE;                     /* default item */
  Flag = RUN_FLAG | CHANGE_TYPE | CHANGE_LOW | CHANGE_HIGH;

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  display family and Vcc/Vdd
     */

    if (Flag & CHANGE_TYPE)
    {
      LCD_ClearLine2();                 /* line #2 */
      MarkItem(ITEM_TYPE, Item);        /* mark item if selected */

      /* display family */
      if (VccIndex == 0)                /* TTL */
        Display_EEString(TTL_str);      /* display: TTL */
      else                              /* CMOS */
        Display_EEString(CMOS_str);     /* display: CMOS */

      /* read Vcc/Vdd from table */
      U_max = DATA_read_word(&Logic_Vcc_table[VccIndex]);

      /* display Vcc/Vdd */
      Display_Space();
      U1 = U_max / 100;                 /* scale to 0.1V */
      Display_Value(U1, -1, 'V');       /* display voltage in 0.1V */

      /* update thresholds */
      if (VccIndex == 0)                /* TTL */
      {
        /* fixed thresholds for 5V */
        U_low = 800;                    /* 0.8V */
        U_high = 2000;                  /* 2.0V */
      }
      else                              /* CMOS */
      {
        /* thresholds based on Vdd (in mV) */
        /* L: 0.3 Vdd */
        U_low = U1 * 30;                /* U in 0.1V * 0.3 * 100 */

        /* H: 0.7 Vdd */
        U_high = U1 * 70;               /* U in 0.1V * 0.7 * 100  */
      }

      Flag &= ~CHANGE_TYPE;             /* clear flag */
    }

    /*
     *  display low threshold
     */

    if (Flag & CHANGE_LOW)
    {
      LCD_ClearLine3();                 /* clear line #3 */
      MarkItem(ITEM_LOW, Item);         /* mark item if selected */
      LCD_Char('L');                    /* display: L */
      Display_Space();
      U1 = U_low / 100;                 /* scale to 0.1V */
      Display_Value(U1, -1, 'V');       /* display voltage in 0.1V */

      Flag &= ~CHANGE_LOW;              /* clear flag */
    }

    /*
     *  display high threshold
     */

    if (Flag & CHANGE_HIGH)
    {
      LCD_ClearLine(4);                 /* line #4 */
      LCD_CharPos(1, 4);
      MarkItem(ITEM_HIGH, Item);        /* mark item if selected */
      LCD_Char('H');                    /* display: H */
      Display_Space();
      U1 = U_high / 100;                /* scale to 0.1V */
      Display_Value(U1, -1, 'V');       /* display voltage in 0.1V */

      Flag &= ~CHANGE_HIGH;             /* clear flag */
    }

    /*
     *  display logic state
     *  - 0, 1 or Z
     *  - ADC pin is connected to a voltage divider (top: R1 / bottom: R2).
     *    - U2 = (Uin / (R1 + R2)) * R2 
     *    - Uin = (U2 * (R1 + R2)) / R2
     */

    /* get voltage */
    U1 = ReadU(TP_LOGIC);          /* read voltage */

    /* consider voltage divider */
    Value = (((uint32_t)(LOGIC_PROBE_R1 + LOGIC_PROBE_R2) * 1000) / LOGIC_PROBE_R2);  /* factor (0.001) */
    Value *= U1;                   /* voltage (0.001 mV) */
    Value /= 1000;                 /* scale to mV */
    U1 = (uint16_t)Value;          /* keep 2 bytes */

    /* compare with thresholds  */
    if (U1 <= U_low)               /* below low threshold */
    {
      State = '0';                 /* 0 for low */

#ifdef HW_BUZZER
      /* buzzer */

#ifdef BUZZER_ACTIVE
      /* active buzzer: one short beep (20ms) */
      BUZZER_PORT |= (1 << BUZZER_CTRL);   /* enable: set pin high */
      MilliSleep(20);                      /* wait for 20 ms */
      BUZZER_PORT &= ~(1 << BUZZER_CTRL);  /* disable: set pin low */
#endif

#ifdef BUZZER_PASSIVE
      /* passive buzzer: short beep, low freq (20ms, 2.5kHz) */
      PassiveBuzzer(BUZZER_FREQ_LOW);      /* low frequency beep */
#endif
#endif
    }
    else if (U1 >= U_high)         /* above high threshold */
    {
      State = '1';                 /* 1 for high */

#ifdef HW_BUZZER
      /* buzzer */

#ifdef BUZZER_ACTIVE
      /* active buzzer: long beep (60ms) */
      BUZZER_PORT |= (1 << BUZZER_CTRL);   /* enable: set pin high */
      MilliSleep(60);                      /* wait for 60 ms */
      BUZZER_PORT &= ~(1 << BUZZER_CTRL);  /* disable: set pin low */
#endif

#ifdef BUZZER_PASSIVE
      /* passive buzzer: short beep, high freq (20ms, 5kHz) */
      PassiveBuzzer(BUZZER_FREQ_HIGH);     /* high frequency beep */
#endif
#endif // HW_BUZZER
    }
    else                           /* in between (undefined, HiZ) */
      State = 'Z';                 /* z for undefined/HiZ */
      /* buzzer: no beep */

    /* display state and voltage */
    LCD_ClearLine(5);                   /* line #5 */
    LCD_CharPos(1, 5);
#ifdef LCD_COLOR
    UI.PenColor = COLOR_SYMBOL;         /* change color */
#endif
    LCD_Char(State);                    /* display state */
#ifdef LCD_COLOR
    UI.PenColor = COLOR_PEN;            /* reset color */
#endif
    Display_Space();
    Display_Value(U1, -3, 'V');         /* display voltage */

    /*
     *  user feedback
     */

    /* check for user feedback and slow down update rate */
    Test = TestKey(200, CHECK_KEY_TWICE | CHECK_BAT);

    /* process user input */
    if (Test == KEY_SHORT)              /* short key press */
    {
      /* select next item */
      Item++;                           /* next item */

      if (Item > ITEM_HIGH)             /* overflow */
        Item = ITEM_TYPE;               /* ... to first item */

      Flag |= CHANGE_TYPE | CHANGE_LOW | CHANGE_HIGH;       /* update display */
    }
    else if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                         /* end loop */
    else if (Test == KEY_RIGHT)         /* right key */
    {
      if (Item == ITEM_TYPE)                 /* family and Vcc/Vdd */
      {
        /* next set */
        VccIndex++;                          /* increase index */
        if (VccIndex >= NUM_LOGIC_TYPES)     /* overflow */
          VccIndex = 0;                      /* roll over to first */
        Flag |= CHANGE_TYPE | CHANGE_LOW | CHANGE_HIGH;     /* update display */
      }
      else if (Item == ITEM_LOW)             /* low threshold */
      {
        /* increase low threshold */
        U_low += 100;                        /* + 100mV */
        if (U_low > U_high)                  /* prevent overflow */
          U_low = U_high;                    /* limit to high threshold */
        Flag |= CHANGE_LOW;                  /* update display */
      }
      else if (Item == ITEM_HIGH)            /* high threshold */
      {
        /* increase high threshold */
        U_high += 100;                       /* + 100mV */
        if (U_high > U_max)                  /* prevent overflow */
          U_high = U_max;                    /* limit to Vcc/Vdd */
        Flag |= CHANGE_HIGH;                 /* update display */
      }
    }
    else if (Test == KEY_LEFT)          /* left key */
    {
      if (Item == ITEM_TYPE)                 /* family and Vcc/Vdd */
      {
        /* previous set */
        if (VccIndex > 0)                    /* prevent underun */
          VccIndex--;                        /* decrease index */
        else                                 /* roll over */
          VccIndex = NUM_LOGIC_TYPES - 1;    /* ... to last */
        Flag |= CHANGE_TYPE | CHANGE_LOW | CHANGE_HIGH;     /* update display */
      }
      else if (Item == ITEM_LOW)             /* low threshold */
      {
        /* decrease low threshold */
        if (U_low >= 100)                    /* prevent underflow (limit to 0V) */
          U_low -= 100;                      /* - 100mV */
        Flag |= CHANGE_LOW;                  /* update display */
      }
      else if (Item == ITEM_HIGH)            /* high threshold */
      {
        /* decrease high threshold */
        U1 = U_low + 100;                    /* limit to low threshold */
        if (U_high >= U1)                    /* prevent underflow */
          U_high -= 100;                     /* - 100mV */
        Flag |= CHANGE_HIGH;                 /* update display */
      }
    }
  }

  /*
   *  clean up
   */

  /* global settings */
  Cfg.Samples = ADC_SAMPLES;            /* set ADC samples back to default */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef CHANGE_TYPE
  #undef CHANGE_LOW
  #undef CHANGE_HIGH

  /* local constants for Item */
  #undef ITEM_LOW
  #undef ITEM_HIGH
}

#endif // HW_LOGIC_PROBE
