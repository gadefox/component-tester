/* ************************************************************************
 *
 *   diode tools (hardware and software options)
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
 *   photodiode check
 * ************************************************************************ */

#ifdef SW_PHOTODIODE

/*
 *  check photodiodes
 *  - supports reverse-bias and no-bias mode
 *  - uses probes #1 (anode) and #3 (cathode)
 */

void PhotodiodeCheck(void)
{
  uint8_t           Flag;               /* loop control */
  uint8_t           Test;               /* user feedback */
  uint16_t          U;                  /* measured voltage */
  uint16_t          R = 0;              /* resistance (current shunt) */
  uint32_t          I;                  /* current I_P */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run / otherwise end */
  #define NO_BIAS             0b00000010     /* no-bias mode */
  #define REVERSE_BIAS        0b00000100     /* reverse-bias mode */
  #define UPDATE_BIAS         0b00001000     /* update bias mode */

  /*
   *  show info
   */

  LCD_Clear();
#ifdef UI_COLORED_TITLES
  /* display: Photodiode */
  Display_ColoredEEString(Photodiode_str, COLOR_TITLE);
#else
  Display_EEString(Photodiode_str);        /* display: Photodiode */
#endif
  ProbePinout(PROBES_DIODE);                 /* show probes used */

  /*
   *  init
   */

  UpdateProbes(PROBE_1, PROBE_2, PROBE_3);   /* update probes */

  /* enter processing loop and set reverse-bias mode */
  Flag = RUN_FLAG | REVERSE_BIAS | UPDATE_BIAS;

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  set up bias mode
     */

    if (Flag & UPDATE_BIAS)        /* update requested */
    {
      /* probe #1: anode, probe #3: cathode */

      if (Flag & REVERSE_BIAS)     /* reverse-bias mode */
      {
        /* safeguard when switching from no-bias mode */
        ADC_DDR = 0;                    /* remove direct pull down */

        /* set probes: Vcc -- Rl -- probe #3 / probe #1 -- Gnd */
        R_PORT = Probes.Rl_3;           /* pull up probe #3 via Rl */
        R_DDR = Probes.Rl_3;            /* enable pull-up resistor */
        ADC_PORT = 0;                   /* pull down directly */
        ADC_DDR = Probes.Pin_1;         /* enable Gnd for probe #1 */

        /* current shunt: Rl + RiH */
        R = (R_LOW * 10) + NV.RiH;      /* in 0.1 Ohms */
      }
      else                         /* no-bias mode */
      {
        /* set probes: probe #1 -- Rl -- Gnd / probe #3 -- Gnd */
        R_PORT = 0;                     /* pull down ... */
        R_DDR = Probes.Rl_1;            /* ... probe #1 via Rl */
        ADC_PORT = 0;                   /* pull down directly */
        ADC_DDR = Probes.Pin_3;         /* enable Gnd for probe #3 */

        /* current shunt: Rl + RiL */
        R = (R_LOW * 10) + NV.RiL;      /* in 0.1 Ohms */
      }

      Flag &= ~UPDATE_BIAS;                  /* clear flag */
    }

    /*
     *  monitor current I_P
     *  - measure voltage across current shunt Rl
     *  - calculate current
     *  - display current
     */

    /* measure voltage */
    if (Flag & REVERSE_BIAS)       /* reverse-bias mode */
      U = Cfg.Vcc - ReadU(Probes.Ch_3); /* voltage at probe #3 (cathode), in mV */
    else                           /* no-bias mode */
      U = ReadU(Probes.Ch_1);      /* voltage at probe #1 (anode), in mV */

    /* calculate I_P (= U / R) */
    I = U * 100000;                /* scale voltage to 0.01 µV */
    I /= R;                        /* / R (in 0.1 Ohms) -> I in 0.1 µA */ 

    /* display I_P */
    LCD_ClearLine2();              /* clear line #2 */
    if (Flag & REVERSE_BIAS)       /* reverse-bias mode */
      Display_EEString_Space(ReverseBias_str);    /* display: rev */
    else                           /* no-bias mode */
      Display_EEString_Space(NoBias_str);         /* display: no */
    Display_Value(I, -7, 'A');     /* display current */

    /*
     *  user feedback
     */

    /* check for user feedback and slow down update rate */
    Test = TestKey(200, CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_SHORT)         /* short key press */
    {
      /* toggle bias mode */
      if (Flag & REVERSE_BIAS)     /* in reverse-bias mode */
      {
        /* switch to no-bias mode */
        Flag &= ~REVERSE_BIAS;               /* clear flag */
        Flag |= NO_BIAS | UPDATE_BIAS;       /* set flag for new mode */
      }
      else                         /* in no-bias mode */
      {
        /* switch to reverse-bias mode */
        Flag &= ~NO_BIAS;                    /* clear flag */
        Flag |= REVERSE_BIAS | UPDATE_BIAS;  /* set flag for new mode */
      }
    }
    else if (Test == KEY_TWICE)    /* two short key presses */
      Flag = 0;                    /* end loop */
  }

  /*
   *  clean up
   */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef NO_BIAS
  #undef REVERSE_BIAS
  #undef UPDATE_BIAS
}

#endif // SW_PHOTODIODE


/* ************************************************************************
 *   diode/LED check
 * ************************************************************************ */

#ifdef SW_DIODE_LED

/*
 *  display diode pinout and Uf
 *
 *  requires:
 *  - Diode: pointer of diode data
 */

void Show_Single_Diode(Diode_Type *Diode)
{
  Display_ProbeNumber(Diode->A);        /* show probe number of anode */
  Display_EEString(Diode_AC_str);       /* show: ->|- */
  Display_ProbeNumber(Diode->C);        /* show probe number of cathode */
  Display_Space();
  Display_Value(Diode->V_f, -3, 'V');   /* show Uf (in mV) */
}


/*
 *  quick-check diodes and LEDs
 *  - uses probes #1 and #3
 *  - requires a display with >= 3 text lines
 */

void Diode_LED_Check(void)
{
  uint8_t           Flag = 1;           /* loop control */
  uint8_t           Test;               /* user feedback */

  /*
   *  show info
   */

  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: diode/LED */
    Display_ColoredEEString(Diode_LED_str, COLOR_TITLE);
#else
    Display_EEString(Diode_LED_str);         /* display: diode/LED */
#endif
  ProbePinout(PROBES_RCL);                   /* show probes used */

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  check for diodes
     */

    /* reset values */
    Check.Diodes = 0;                   /* reset diode counter */

    /* check for diode in one direction (probe #1: A, probe #3: C */
    UpdateProbes2(PROBE_1, PROBE_3);         /* update probes */
    CheckDiode();                            /* run diode check */

    /* check for diode in other direction (probe #1: C, probe #3: A */
    UpdateProbes2(PROBE_3, PROBE_1);         /* update probes */
    CheckDiode();                            /* run diode check */

    /*
     *  process results
     */

    LCD_ClearLine(3);                   /* clear line #3 */
    LCD_ClearLine2();                   /* clear line #2 */

    if (Check.Diodes == 0)              /* no diode */
      Display_Minus();                  /* display: - */
    else                                /* one diode or more */
      Show_Single_Diode(&Diodes[0]);    /* display pinout and Uf of first diode */

    if (Check.Diodes == 2)              /* two anti-parallel diodes */
    {
      LCD_CharPos(1, 3);                /* go to start of line #3 */
      Show_Single_Diode(&Diodes[1]);    /* display pinout and Uf of second diode */
    }

    /*
     *  user feedback
     */

    /* check for user feedback and slow down update rate */
    Test = TestKey(250, CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)    /* two short key presses */
      Flag = 0;                    /* end loop */
  }
}

#endif // SW_DIODE_LED
