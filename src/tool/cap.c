/* ************************************************************************
 *
 *   cap tools (hardware and software options)
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

#ifdef SW_ESR_TOOL
const unsigned char ESR_str[] MEM_TYPE = "ESR";
#endif


/* ************************************************************************
 *   ESR tool
 * ************************************************************************ */

#ifdef SW_ESR_TOOL

/*
 *  ESR tool
 *  - uses probe #1 (pos) and probe #3 (neg)
 */

void ESR_Tool(void)
{
  uint8_t           Run = 1;       /* control flag */
  uint8_t           Test;          /* temp. value */
  Capacitor_Type    *Cap;          /* pointer to cap */
  uint16_t          ESR;           /* ESR (in 0.01 Ohms) */

  Check.Diodes = 0;                /* disable diode check in cap measurement */
  Cap = &Caps[0];                  /* pointer to first cap */

#ifdef HW_DISCHARGE_RELAY
  /* discharge relay: short circuit probes */
                                   /* ADC_PORT should be 0 */
  ADC_DDR = (1 << TP_REF);         /* disable relay */
#endif

  /* show tool info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
    /* display: ESR */
    Display_ColoredEEString(ESR_str, COLOR_TITLE);
#else
    Display_EEString(ESR_str);     /* display: ESR */
#endif
  ProbePinout(PROBES_ESR);         /* show probes used */
  Display_Minus();                 /* display "no value" */

  while (Run > 0)
  {
    /*
     *  short or long key press -> measure
     *  two short key presses -> exit tool
     */

    /* wait for user feedback */
    Test = TestKey(0, CURSOR_BLINK | CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)              /* two short key presses */
      Run = 0;                          /* end loop */

    /* measure cap */
    if (Run > 0)                        /* key pressed */
    {
#ifdef HW_DISCHARGE_RELAY
      /* discharge relay: remove short circuit */
                                        /* ADC_PORT should be 0 */
      ADC_DDR = 0;                      /* enable relay (via extrenal reference) */
#endif

      LCD_ClearLine2();                 /* update line #2 */
      Display_EEString(Probing_str);    /* display: probing... */
      Check.Found = COMP_NONE;          /* no component */
      MeasureCap(PROBE_1, PROBE_3, 0);  /* probe-1 = Vcc, probe-3 = Gnd */
      LCD_ClearLine2();                 /* update line #2 */
      
      if (Check.Found == COMP_CAPACITOR)     /* found capacitor */
      {
        /* show capacitance */
        Display_Value(Cap->Value, Cap->Scale, 'F');

        /* show ESR */
        Display_Space();
        ESR = MeasureESR(Cap);
        if (ESR < UINT16_MAX)           /* got valid ESR */
          Display_Value(ESR, -2, LCD_CHAR_OMEGA);
        else                            /* no ESR */
          Display_Minus();
      }
      else                                   /* no capacitor */
        Display_Minus();

#ifdef HW_DISCHARGE_RELAY
      ADC_DDR = (1 << TP_REF);          /* short circuit probes */
#endif
    }
  }

#ifdef HW_DISCHARGE_RELAY
  ADC_DDR = 0;                     /* remove short circuit */
#endif
}

#endif // SW_ESR_TOOL


/* ************************************************************************
 *   capacitor leakage current
 * ************************************************************************ */

#ifdef SW_CAP_LEAKAGE

/*
 *  tool for measuring the leakage current of a capacitor
 *  - uses probe #1 (pos) and probe #3 (neg)
 *  - requires display with more than 2 lines
 */

void Cap_Leakage(void)
{
  uint8_t           Flag;               /* loop control flag */
  uint8_t           Test = 0;           /* user feedback */
  uint8_t           Mode;               /* mode */
  uint16_t          U1 = 0;             /* voltage #1 */
  uint32_t          Value;              /* temp. value */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run flag */
  #define CHANGED_MODE        0b00000100     /* mode has changed */

  /* local constants for Mode */
  #define MODE_NONE           0         /* no mode (show pinout) */
  #define MODE_HIGH           1         /* charge cap: high current */
  #define MODE_LOW            2         /* charge cap: low current */
  #define MODE_DISCHARGE      3         /* discharge cap */

  /* show info */
  LCD_Clear();                          /* clear display */
#ifdef UI_COLORED_TITLES
  /* display: cap leakage */
  Display_ColoredEEString(CapLeak_str, COLOR_TITLE);
#else
  Display_EEString(CapLeak_str);      /* display: cap leakage */
#endif

  /* set start values */
  Flag = RUN_FLAG | CHANGED_MODE;
  Mode = MODE_NONE;

  UpdateProbes(PROBE_1, 0, PROBE_3);    /* update register bits and probes */

  while (Flag > 0)       /* processing loop */
  {
    /*
     *  display mode and set probes
     */

    if (Flag & CHANGED_MODE)       /* mode has changed */
    {
      LCD_ClearLine2();            /* clear line #2 */

      switch (Mode)                /* based on mode */
      {
        case MODE_NONE:            /* display pinout */
          /* probe-1: Vcc / probe-3: Gnd */
          Show_SimplePinout('+', 0, '-');
          LCD_ClearLine(3);             /* clear line #3 */
          break;

        case MODE_HIGH:            /* charge cap with high current (Rl) */
          Display_EEString_Space(CapCharge_str);
          Display_EEString(CapHigh_str);

          /* set probes: probe-3 -- Rl -- Gnd / probe-1 -- Vcc */
          ADC_DDR = 0;                  /* set to HiZ */
          R_DDR = Probes.Rl_3;          /* select Rl for probe-3 */
          R_PORT = 0;                   /* pull down probe-3 via Rl */
          ADC_PORT = Probes.Pin_1;      /* pull up probe-1 directly */
          ADC_DDR = Probes.Pin_1;       /* enable pull-up of probe-1 */
          break;

        case MODE_LOW:             /* charge cap with low current (Rh) */
          /* max. charge current I = 5V/Rh = 10.6µA */
          Display_EEString_Space(CapCharge_str);
          Display_EEString(CapLow_str);

          /* set probes: probe-3 -- Rh -- Gnd / probe-1 -- Vcc */
          /* simply switch pull-down resistor Rl to Rh */
          R_DDR = Probes.Rh_3;          /* select Rh for probe-3 */
          break;

        case MODE_DISCHARGE:       /* discharge cap */
          Display_EEString(CapDischarge_str);
          /* set probes: probe-3 -- Gnd / probe-1 -- Rl -- Gnd */
          ADC_DDR = 0;                  /* set to HiZ */
          R_DDR = Probes.Rl_1;          /* select Rl for probe-1 */
          /* R_PORT set to 0 already: pull down probe-1 via Rl */
          ADC_DDR = Probes.Pin_3;       /* set probe-3 to output */
          ADC_PORT = 0;                 /* pull down probe-3 directly */
          break;
      }

      Flag &= ~CHANGED_MODE;       /* clear flag */
    }

    /*
     *  manage modes
     */

    if (Mode != MODE_NONE)
    {
      LCD_ClearLine3();            /* clear line #3 */

      switch (Mode)                /* based on mode */
      {
        case MODE_HIGH:            /* charge cap with high current (Rl) */
          /* voltage across Rl and RiL at probe-3 */
          U1 = ReadU(Probes.Ch_3);           /* read voltage at probe-3 */

          /* calculate current: I = U / R (ignore R_Zero) */
          Value = U1;                        /* U across Rl and RiL in mV */
          Value *= 100000;                   /* scale to 0.01 µV */
          Value /= ((R_LOW * 10) + NV.RiL);  /* 0.01 µV / 0.1 Ohms = 0.1 µA */
          Display_Value(Value, -7, 'A');     /* display current */

          /* change to low current mode when current is quite low */
          if (U1 <= 3)                       /* I <= 4.2µA */
          {
            Mode = MODE_LOW;                 /* low current mode */
            Flag |= CHANGED_MODE;            /* set flag for changed mode */
          }
          break;

        case MODE_LOW:             /* charge cap with low current (Rh) */
          /* voltage across Rh at probe-3 (ignore RiL) */
          U1 = ReadU(Probes.Ch_3);           /* read voltage at probe-3 */

          if (U1 > CAP_DISCHARGED)      /* minimum exceeded */
          {
            /* calculate current: I = U / R */
            Value = U1;                        /* in mV */
            Value *= 10000;                    /* scale to 0.1 µV */
            Value /= (R_HIGH / 1000);          /* 0.1 µV / kOhms = 0.1 nA */
            Display_Value(Value, -10, 'A');    /* display current */
          }
          else                          /* in the noise floor */
            Display_Minus();
          break;

        case MODE_DISCHARGE:       /* discharge cap */
          /* voltage at cap (probe-1) */
          U1 = ReadU(Probes.Ch_1);           /* read voltage at probe-1 */
          Display_Value(U1, -3, 'V');        /* display voltage */

          /* check if cap is discharged */
          if (U1 <= CAP_DISCHARGED)          /* < threshold */
          {
            /* start new check cycle */
            Mode = MODE_NONE;                /* show pinout */
            Flag |= CHANGED_MODE;            /* set flag for changed mode */
          }
          break;
      }

      /* common display output */
      if ((Mode == MODE_HIGH) || (Mode == MODE_LOW))
      {
        /* display voltage across current shunt (Rl or Rh) */
        Display_Space();
        Display_Char('(');
        Display_Value(U1, -3, 'V');          /* display voltage */
        Display_Char(')');
      }
    }

    /*
     *  user feedback
     *  - short key press -> next step
     *  - two short key presses -> exit tool
     */

    if (! (Flag & CHANGED_MODE))        /* skip when mode has changed */
    {
      /* wait for user feedback or timeout of 2s */
      Test = TestKey(2000, CHECK_KEY_TWICE | CHECK_BAT);
      /* also delay for next loop run */

      if (Test == KEY_SHORT)            /* short key press */
        Test = 100;                     /* next mode */
      else if (Test == KEY_TWICE)       /* two short key presses */
        Flag = 0;                       /* end processing loop */
#ifdef HW_KEYS
      else if (Test == KEY_RIGHT)       /* right key */
        Test = 100;                     /* next mode */
#endif

      if (Test == 100)                  /* next mode */
      {
        /* change mode */
        if (Mode == MODE_NONE)          /* pinout mode */
        {
          Mode = MODE_HIGH;             /* charge cap with high current */
          Flag |= CHANGED_MODE;         /* set flag for changed mode */
        }
        else                            /* any other mode */
        {
          Mode = MODE_DISCHARGE;        /* discharge cap */
          Flag |= CHANGED_MODE;         /* set flag for changed mode */
        }
      }
    }
  }

  /*
   *  clean up
   */

  /* local constants for Mode */
  #undef MODE_NONE
  #undef MODE_CHARGE
  #undef MODE_LEAK
  #undef MODE_DISCHARGE

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef CHANGED_MODE
}

#endif // SW_CAP_LEAKAGE
