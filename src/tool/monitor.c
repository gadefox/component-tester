/* ************************************************************************
 *
 *   monitor tool (hardware and software options)
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
 *   monitoring R/C/L
 * ************************************************************************ */

#ifdef SW_MONITOR_R

/*
 *  monitor R on probes #1 and #3
 */

void Monitor_R(void)
{
  uint8_t           Flag = 1;           /* loop control flag */
  uint8_t           Test;               /* user feedback */
  Resistor_Type     *R1;                /* pointer to resistor #1 */

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: R monitor */
    Display_ColoredEEString(Monitor_R_str, COLOR_TITLE);
#else
    Display_EEString(Monitor_R_str);    /* display: R monitor */
#endif
  ProbePinout(PROBES_RCL);              /* show probes used */

  /* init */
  UpdateProbes2(PROBE_1, PROBE_3);      /* update probes */
  R1 = &Resistors[0];                   /* pointer to first resistor */
  /* increase number of samples to lower spread of measurement values */
  Cfg.Samples = 100;                    /* perform 100 ADC samples */

  /*
   *  processing loop
   */

  while (Flag)
  {
    /* measure R and display value */
    Check.Resistors = 0;                /* reset resistor counter */
    CheckResistor();                    /* check for resistor */
    LCD_ClearLine2();                   /* clear line #2 */

    if (Check.Resistors == 1)           /* found resistor */
      /* display value */
      Display_Value(R1->Value, R1->Scale, LCD_CHAR_OMEGA);
    else                                /* no resistor */
      Display_Minus();                  /* display: nothing */

    /* user feedback (1s delay) */
    Test = TestKey(1000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                    /* end processing loop */
  }

  /* clean up */
  Cfg.Samples = ADC_SAMPLES;       /* set ADC samples back to default */
}

#endif // SW_MONITOR_R


#ifdef SW_MONITOR_C

/*
 *  monitor C on probes #1 and #3
 *  - optionally ESR
 */

void Monitor_C(void)
{
  uint8_t           Flag = 1;           /* loop control flag */
  uint8_t           Test;               /* user feedback */
  Capacitor_Type    *Cap;               /* pointer to cap */
#if defined (SW_ESR) || defined (SW_OLD_ESR)
  uint16_t          ESR;                /* ESR (in 0.01 Ohms) */
#endif

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: C monitor */
    Display_ColoredEEString(Monitor_C_str, COLOR_TITLE);
#else
    Display_EEString(Monitor_C_str);    /* display: C monitor */
#endif
  ProbePinout(PROBES_RCL);              /* show probes used */

  /* init */
  Check.Diodes = 0;                     /* reset diode counter */
  Cap = &Caps[0];                       /* pointer to first cap */

  /*
   *  processing loop
   */

  while (Flag)
  {
#ifdef SW_MONITOR_HOLD_ESR
    if (Flag == 2)                      /* display former measurement */
    {
      LCD_ClearLine3();                 /* clear line #3 */

      Display_Colon();                            /* display: ':' */
      Display_Value(Cap->Value, Cap->Scale, 'F'); /* display capacitance */
      Display_Space();                            /* display: ' ' */
      Display_Value(ESR, -2, LCD_CHAR_OMEGA);     /* display ESR */

      Flag = 1;                        /* reset flag */
    }
#endif // SW_MONITOR_HOLD_ESR

    /* measure and display C */
    Check.Found = COMP_NONE;                 /* no component */
    /* keep probe order of normal probing cycle */
    MeasureCap(PROBE_3, PROBE_1, 0);         /* measure capacitance */
    LCD_ClearLine2();                        /* clear line #2 */

    if (Check.Found == COMP_CAPACITOR)       /* found cap */
    {
      /* display capacitance */
      Display_Value(Cap->Value, Cap->Scale, 'F');

#if defined (SW_ESR) || defined (SW_OLD_ESR)
      /* measure and show ESR */
      ESR = MeasureESR(Cap);                 /* measure ESR */
      if (ESR < UINT16_MAX)                  /* if successfull */
      {
        Display_Space();
        Display_Value(ESR, -2, LCD_CHAR_OMEGA);   /* display ESR */

#ifdef SW_MONITOR_HOLD_ESR
        Flag = 2;                            /* signal valid ESR */
#endif // SW_ESR || SW_OLD_ESR
      }
#endif
    }
    else                                     /* no cap */
      Display_Minus();                       /* display: nothing */

    /* user feedback (2s delay) */
    Test = TestKey(2000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                    /* end processing loop */
  }
}

#endif // SW_MONITOR_C 


#ifdef SW_MONITOR_L

/*
 *  monitor L on probes #1 and #3
 */

void Monitor_L(void)
{
  uint8_t           Flag = 1;           /* loop control flag */
  uint8_t           Test;               /* user feedback */
  Resistor_Type     *R1;                /* pointer to resistor #1 */

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: L monitor */
    Display_ColoredEEString(Monitor_L_str, COLOR_TITLE);
#else
    Display_EEString(Monitor_L_str);    /* display: L monitor */
#endif
  ProbePinout(PROBES_RCL);              /* show probes used */

  /* init */
  R1 = &Resistors[0];                   /* pointer to first resistor */

  /*
   *  processing loop
   */

  while (Flag)
  {
#ifdef SW_MONITOR_HOLD_L
    if (Flag == 2)                      /* display former measurement */
    {
      LCD_ClearLine3();                 /* clear line #3 */

      Display_Colon();                  /* display: ':' */
      Display_Value(Inductor.Value, Inductor.Scale, 'H');   /* display L */

      Flag = 1;                         /* reset flag */
    }
#endif

    /* measure R */
    UpdateProbes2(PROBE_1, PROBE_3);    /* update probes */
    Check.Resistors = 0;                /* reset resistor counter */
    CheckResistor();                    /* check for resistor */
    LCD_ClearLine2();                   /* clear line #2 */

    if (Check.Resistors == 1)           /* found resistor */
    {
      /* get inductance and display if relevant */
      if (MeasureInductor(R1) == 1)
      {
        /* display inductance */
        Display_Value(Inductor.Value, Inductor.Scale, 'H');

#ifdef SW_MONITOR_HOLD_L
        Flag = 2;                       /* signal valid L */
#endif
      }
      else                              /* no inductor */
        Display_Minus();                /* display: nothing */
    }
    else                                /* no resistor */
      Display_Minus();                  /* display: nothing */

    /* user feedback (1s delay) */
    Test = TestKey(1000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                    /* end processing loop */
  }
}

#endif // SW_MONITOR_L


#ifdef SW_MONITOR_RCL

/*
 *  monitor R plus L, or C plus ESR on probes #1 and #3
 */

void Monitor_RCL(void)
{
  uint8_t           Run = 1;            /* loop control flag */
  uint8_t           Test;               /* user feedback */
  Resistor_Type     *R1;                /* pointer to resistor #1 */
  Capacitor_Type    *Cap;               /* pointer to cap */
#if defined (SW_ESR) || defined (SW_OLD_ESR)
  uint16_t          ESR;                /* ESR (in 0.01 Ohms) */
#endif

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
  /* display: RCL monitor */
  Display_ColoredEEString(Monitor_RCL_str, COLOR_TITLE);
#else
  Display_EEString(Monitor_RCL_str);  /* display: RCL monitor */
#endif
  ProbePinout(PROBES_RCL);              /* show probes used */

  /* init */
  R1 = &Resistors[0];                   /* pointer to first resistor */
  Cap = &Caps[0];                       /* pointer to first cap */
  Check.Diodes = 0;                     /* reset diode counter */

  /*
   *  processing loop
   */

  while (Run)
  {
    /*
     *  check for R, L and C
     */

    Check.Found = COMP_NONE;            /* no component */

    if (Run != COMP_CAPACITOR)          /* none, R or L */
    {
      /* increase number of samples to lower spread of measurement values */
      Cfg.Samples = 100;                /* perform 100 ADC samples */

      /* measure R */
      UpdateProbes2(PROBE_1, PROBE_3);       /* update probes */
      Check.Resistors = 0;                   /* reset resistor counter */
      CheckResistor();                       /* check for resistor */

      if (Check.Resistors == 1)         /* found resistor */
      {
        Check.Found = COMP_RESISTOR;         /* is R */

        /* measure L */
        if (MeasureInductor(R1) == 1)        /* got inductance */
          Run = COMP_INDUCTOR;               /* is an inductor */
        else                                 /* no inductance */
          Run = COMP_RESISTOR;               /* is a resistor */
      }
      else                              /* no resistor */
        Run = 1;                             /* reset to "no component" */

      Cfg.Samples = ADC_SAMPLES;        /* set ADC samples back to default */
    }

    if (Run != COMP_INDUCTOR)           /* none, C or R */
    {
      /* measure capacitance */

      /* keep probe order of normal probing cycle */
      MeasureCap(PROBE_3, PROBE_1, 0);       /* measure capacitance */

      if (Check.Found == COMP_CAPACITOR)     /* found cap */
        Run = COMP_CAPACITOR;                /* is a cap */
      else if (Run != COMP_RESISTOR)         /* no cap and no resistor */
        Run = 1;                             /* reset to "no component" */
    }

    /*
     *  display measurement
     */

    LCD_ClearLine2();                   /* clear line #2 */

    if (Run == 1)                       /* no component */
      Display_Minus();                  /* display: nothing */
    else if (Run == COMP_CAPACITOR)     /* C */
    {
      /* display capacitance */
      Display_Value(Cap->Value, Cap->Scale, 'F');

#if defined (SW_ESR) || defined (SW_OLD_ESR)
      /* show ESR */
      ESR = MeasureESR(Cap);                 /* measure ESR */
      if (ESR < UINT16_MAX)                  /* if successfull */
      {
        Display_Space();
        Display_Value(ESR, -2, LCD_CHAR_OMEGA);   /* display ESR */
      }
#endif
    }
    else                                 /* R or L */
    {
      /* display resistance */
      Display_Value(R1->Value, R1->Scale, LCD_CHAR_OMEGA);

      if (Run == COMP_INDUCTOR)          /* L */
      {
        /* display inductance */
        Display_Space();
        Display_Value(Inductor.Value, Inductor.Scale, 'H');
      }
    }

    /* user feedback (1s delay) */
    Test = TestKey(1000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      Run = 0;                     /* end processing loop */
  }
}

#endif // SW_MONITOR_RCL


#ifdef SW_MONITOR_RL

/*
 *  monitor R plus L on probes #1 and #3
 */

void Monitor_RL(void)
{
  uint8_t           Flag = 1;           /* loop control flag */
  uint8_t           Test;               /* user feedback */
  Resistor_Type     *R1;                /* pointer to resistor #1 */

  /* show info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
  /* display: monitor RL */
  Display_ColoredEEString(Monitor_RL_str, COLOR_TITLE);
#else
  Display_EEString(Monitor_RL_str);   /* display: monitor RL */
#endif
  ProbePinout(PROBES_RCL);              /* show probes used */

  /* init */
  R1 = &Resistors[0];                   /* pointer to first resistor */
  /* increase number of samples to lower spread of measurement values */
  Cfg.Samples = 100;                    /* perform 100 ADC samples */

  /*
   *  processing loop
   */

  while (Flag)
  {
    /* measure R and display value */
    UpdateProbes2(PROBE_1, PROBE_3);    /* update probes */
    Check.Resistors = 0;                /* reset resistor counter */
    CheckResistor();                    /* check for resistor */
    LCD_ClearLine2();                   /* clear line #2 */

    if (Check.Resistors == 1)           /* found resistor */
    {
      /* display value */
      Display_Value(R1->Value, R1->Scale, LCD_CHAR_OMEGA);

      /* get inductance and display if relevant */
      if (MeasureInductor(R1) == 1)
      {
        Display_Space();
        Display_Value(Inductor.Value, Inductor.Scale, 'H');
      }
    }
    else                                /* no resistor */
      Display_Minus();                  /* display: nothing */

    /* user feedback (1s delay) */
    Test = TestKey(1000, CHECK_KEY_TWICE | CHECK_BAT | CURSOR_STEADY);

    if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                    /* end processing loop */
  }

  /* clean up */
  Cfg.Samples = ADC_SAMPLES;       /* set ADC samples back to default */
}

#endif // SW_MONITOR_RL
