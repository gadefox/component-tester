/* ************************************************************************
 *
 *   meter 5V DC tool (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_METER_5VDC

/*
 *  local constants
 */


/*
 *  local variabless
 */


/* ************************************************************************
 *   Voltmeter 0-5V DC
 * ************************************************************************ */

/*
 *  Voltmeter 0-5V DC
 *  - input impedance:
 *    470 kOhms (Rh, default)
 *    680 Ohms + RiL (Rl)
 *  - with optional buzzer:
 *    beep when default threshold is exceeded
 *  - with additional keys and buzzer:
 *    adjustable threshold, beep when exceeded
 *  - uses probes #1 (positive) and #3 (ground)
 */

void Meter_5VDC(void)
{
  uint8_t           Flag;               /* loop control */
  uint8_t           Test = 0;           /* user feedback */
  uint16_t          U;                  /* voltage */
  #ifdef HW_BUZZER
  uint16_t          Threshold;          /* voltage threshold */
  #endif
  uint8_t           n;                  /* counter */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run / otherwise end */
  #define BUZZER_ON           0b00000010     /* buzzer enabled */
  #define UPDATE_THRESHOLD    0b00000100     /* update threshold */
  #define INPUT_RH            0b00001000     /* use Rh as input impedance */
  #define UPDATE_INPUT        0b00010000     /* update input impedance */

  /* default voltage threshold: scale to mV */
  #define THRESHOLD           (METER_5VDC_THRESHOLD * 100)

  /*
   *  show info
   */

  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: 5V-Meter */
    Display_ColoredEEString(Meter_5VDC_str, COLOR_TITLE);
#else
    Display_EEString(Meter_5VDC_str);        /* display: 5V Meter */
#endif
  ProbePinout(PROBES_ESR);                   /* show probes used */

  /*
   *  init
   */

  UpdateProbes(PROBE_1, PROBE_2, PROBE_3);   /* update probes */

  /* set probes: Gnd - probe #3 / probe #1 - Rh - Gnd */
  ADC_PORT = 0;                    /* pull down directly */
  ADC_DDR = Probes.Pin_3;          /* enable Gnd for probe #3 */
  R_PORT = 0;                      /* pull down */
  R_DDR = Probes.Rh_1;             /* pull down probe #1 via Rh */

  #ifdef HW_BUZZER
  Threshold = THRESHOLD;           /* set default */
  #endif

  /* enter loop, Rh as input impedance, update input status */
  Flag = RUN_FLAG | INPUT_RH | UPDATE_INPUT;

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  update input impedance status
     */

    if (Flag & UPDATE_INPUT)       /* update requested */
    {
      /* show title */
      LCD_ClearLine(1);                 /* clear line #1 */
      LCD_CharPos(1, 1);                /* pos #1 in line #1 */
#ifdef UI_COLORED_TITLES
      /* display: 5V-Meter */
      Display_ColoredEEString(Meter_5VDC_str, COLOR_TITLE);
#else
      /* display: 5V Meter */
      Display_EEString(Meter_5VDC_str);
#endif

      /* show input impedance */
      Display_Space();

      if (Flag & INPUT_RH)              /* Rh selected */
        Display_Char('H');              /* display: H */
      else                              /* Rl selected */
        Display_Char('L');              /* display: L */

      Flag &= ~UPDATE_INPUT;            /* clear flag */
    }

    /* smooth UI after long key press */
    if (Test == KEY_LONG)          /* long key press */
      SmoothLongKeyPress();             /* delay next key press */

#ifdef HW_BUZZER
    /*
     *  update threshold status
     */

    if (Flag & UPDATE_THRESHOLD)   /* update requested */
    {
      /* clear threshold anyway */
      LCD_CharPos(9, 2);                /* pos #9 in line #2 */
      LCD_ClearLine(0);                 /* clear rest of line */

      if (Flag & BUZZER_ON)             /* buzzer enabled */
      {
        /* show threshold */
        U = Threshold / 100;            /* scale to 100mV */
        LCD_CharPos(9, 2);              /* pos #9 in line #2 */
        Display_Char('(');              /* display: ( */
        Display_Value(U, -1, 'V');      /* display voltage */
        Display_Char(')');              /* display: ) */
      }

      Flag &= ~UPDATE_THRESHOLD;        /* clear flag */
    }
#endif // HW_BUZZER

    /*
     *  measure voltage
     */

    U = ReadU(Probes.Ch_1);        /* voltage at probe #1 (postive), in mV */

    /*
     *  display voltage
     */

    /* clear pos #1 - #6 in line #2 */
    LCD_CharPos(1, 2);             /* pos #1 in line #2 */
    for (n = 0; n < 6; n++)        /* clear pos #1 - #6 */
      Display_Space();

    /* display voltage */
    LCD_CharPos(1, 2);             /* pos #1 in line #2 */
    Display_Value(U, -3, 'V');     /* display voltage */

#ifdef HW_BUZZER
    if (Flag & BUZZER_ON)          /* buzzer enabled */
    {
      if (U >= Threshold)          /* threshold exceeded */
      {
#ifdef BUZZER_ACTIVE
        /* active buzzer: short beep (20ms) */
        BUZZER_PORT |= (1 << BUZZER_CTRL);     /* enable: set pin high */
        wait20ms();                            /* wait 20 ms */
        BUZZER_PORT &= ~(1 << BUZZER_CTRL);    /* disable: set pin low */
#endif

#ifdef BUZZER_PASSIVE
        /* passive buzzer: short beep, high freq (20ms, 5kHz) */
        PassiveBuzzer(BUZZER_FREQ_HIGH);       /* high frequency beep */
#endif
      }
    }
#endif // HW_BUZZER

    /*
     *  user feedback
     */

    /* check for user feedback and slow down update rate */
    Test = TestKey(250, CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                         /* end loop */

#ifdef HW_BUZZER
    else if (Test == KEY_SHORT)    /* short key press */
    {
      /* toggle buzzer */
      if (Flag & BUZZER_ON)             /* buzzer enabled */
        /* disable buzzer */
        Flag &= ~BUZZER_ON;             /* clear flag */
      else                              /* buzzer disabled */
        /* enable buzzer */
        Flag |= BUZZER_ON;              /* set flag */

      Flag |= UPDATE_THRESHOLD;         /* update status */
    }
#endif // HW_BUZZER

#if defined(HW_BUZZER) && (defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH))
    else if (Test == KEY_RIGHT)    /* right turn */
    {
      /* increase voltage threshold (by 0.1V) */
      if (Threshold <= 4900)            /* prevent overrun */
      {
        Threshold += 100;               /* + 0.1 V */
        Flag |= UPDATE_THRESHOLD;       /* update status */
      }
    }
    else if (Test == KEY_LEFT)     /* left turn */
    {
      /* decrease voltage threshold (by 0.1V) */
      if (Threshold >= 100)             /* prevent underrun */
      {
        Threshold -= 100;               /* - 0.1 V */
        Flag |= UPDATE_THRESHOLD;       /* update status */
      }
    }
    else if (Test == KEY_LONG)     /* long key press */
    {
      /* change input impedance */
      if (Flag & INPUT_RH)              /* Rh selected */
      {
        /* change to Rl */
        /* set probes: probe #1 - Rl - Gnd */
        R_DDR = Probes.Rl_1;            /* pull down probe #1 via Rl */
        Flag &= ~INPUT_RH;              /* clear flag */
      }
      else                              /* Rl selected */
      {
        /* change to Rh */
        /* set probes: probe #1 - Rh - Gnd */
        R_DDR = Probes.Rh_1;            /* pull down probe #1 via Rh */
        Flag |= INPUT_RH;               /* set flag */
      }

      Flag |= UPDATE_INPUT;             /* update status */

#if 0
      /* alternative: reset voltage threshold */
      if (Flag & BUZZER_ON)             /* buzzer enabled */
      {
        /* set threshold to default value */
        Threshold = THRESHOLD;          /* set default */
        Flag |= UPDATE_THRESHOLD;       /* update status */
      }
#endif // 0
    }
#endif // HW_BUZZER && HW_KEYS
  }

  /*
   *  clean up
   */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef BUZZER_ON
  #undef UPDATE_THRESHOLD
  #undef INPUT_RH
  #undef UPDATE_INPUT

  /* default threshold */
  #undef THRESHOLD
}


#endif // SW_METER_5VDC
