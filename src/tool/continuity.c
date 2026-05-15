/* ************************************************************************
 *
 *   continuity tool (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_CONTINUITY_CHECK

/*
 *  local constants
 */


/*
 *  local variabless
 */


/* ************************************************************************
 *   continuity check
 * ************************************************************************ */

/*
 *  continuity check
 *  - uses probes #1 (pos) and #3 (neg)
 *  - requires buzzer
 */

void ContinuityCheck(void)
{
  uint8_t           Flag;               /* loop control */
  uint8_t           Test;               /* user feedback */
  uint16_t          U1;                 /* measured voltage #1 */
  uint16_t          U2;                 /* measured voltage #2 */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run / otherwise end */
  #define BEEP_SHORT          0b00000010     /* short beep */

  /*
   *  show info
   */

  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: Continuity */
    Display_ColoredEEString(ContinuityCheck_str, COLOR_TITLE);
#else
    Display_EEString(ContinuityCheck_str);   /* display: Continuity */
#endif
  ProbePinout(PROBES_ESR);         /* show probes used */

  /*
   *  init
   */

  UpdateProbes(PROBE_1, PROBE_2, PROBE_3);        /* update probes */

  /* set probes: Vcc -- Rl -- probe #1 / probe #3 -- Gnd */
  R_PORT = Probes.Rl_1;                 /* pull up probe #1 via Rl */
  R_DDR = Probes.Rl_1;                  /* enable pull-up resistor */
  ADC_PORT = 0;                         /* pull down directly */
  ADC_DDR = Probes.Pin_3;               /* enable Gnd for probe #3 */

  Cfg.Samples = 5;                      /* do just 5 samples to be fast */
  Flag = RUN_FLAG;                      /* enter processing loop */

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  check voltage across probes
     *  and output result
     */

    U1 = ReadU(Probes.Ch_1);            /* read voltage at probe #1 */
    U2 = ReadU(Probes.Ch_3);            /* read voltage at probe #3 */

    /* consider voltage drop by RiL */
    if (U1 > U2)                        /* sanity check */
      U1 -= U2;                         /* subtract voltage of RiL */

    /* compare voltage with thresholds */
    if (U1 < 100)                       /* < 100mV */
    {
      /* short */

#ifdef BUZZER_ACTIVE
      /* active buzzer: continuous beep (enable) */
      BUZZER_PORT |= (1 << BUZZER_CTRL);     /* set pin high */
#endif

#ifdef BUZZER_PASSIVE
      /* passive buzzer: short beep, high freq (20ms, 5kHz) */
      PassiveBuzzer(BUZZER_FREQ_HIGH);       /* high frequency beep */
#endif
    }
    else if (U1 <= 700)                 /* 100-700mV */
    {
      /* pn junction */

#ifdef BUZZER_ACTIVE
      /* active buzzer: short beep (enable) */
      BUZZER_PORT |= (1 << BUZZER_CTRL);     /* set pin high */
      Flag |= BEEP_SHORT;                    /* set flag */
#endif

#ifdef BUZZER_PASSIVE
      /* passive buzzer: short beep, low freq (20ms, 2.5kHz) */
      PassiveBuzzer(BUZZER_FREQ_LOW);        /* low frequency beep */
#endif
    }
    else                                /* > 700mV */
    {
      /* something else or open circuit: no beep */

#ifdef BUZZER_ACTIVE
      /* active buzzer: disable */
      BUZZER_PORT &= ~(1 << BUZZER_CTRL);    /* set pin low */
#endif
    }

    /* display voltage */
    LCD_ClearLine2();                   /* line #2 */
    Display_Value(U1, -3, 'V');         /* display voltage */
    /* this is also used as delay for the short beep */
    /* TODO: do we need an additional delay for fast displays? */

#ifdef BUZZER_ACTIVE
    if (Flag & BEEP_SHORT)              /* short beep */
    {
      /* active buzzer: disable */
      BUZZER_PORT &= ~(1 << BUZZER_CTRL);    /* set pin low */
      Flag &= ~BEEP_SHORT;                   /* clear flag */
    }
#endif

    /*
     *  user feedback
     */

    /* check for user feedback and slow down update rate */
    Test = TestKey(50, CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                    /* end loop */
  }

  /*
   *  clean up
   */

  /* global settings */
  Cfg.Samples = ADC_SAMPLES;            /* set ADC samples back to default */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef BEEP_SHORT
}

#endif // SW_CONTINUITY_CHECK
