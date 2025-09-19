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

  while (1)
  {
    /*
     *  short or long key press -> measure
     *  two short key presses -> exit tool
     */

    /* wait for user feedback */
    Test = TestKey(0, CURSOR_BLINK | CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)              /* two short key presses */
    {
#ifdef HW_DISCHARGE_RELAY
      ADC_DDR = 0;                     /* remove short circuit */
#endif
      return;
    }

    /* measure cap */
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
      if (ESR != UINT16_MAX)           /* got valid ESR */
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
  uint8_t           Test;               /* user feedback */
  uint8_t           Mode;               /* mode */
  uint16_t          U1;                 /* voltage #1 */
  uint32_t          Value;              /* temp. value */

  /* local constants for Mode */
  #define MODE_PINOUT         0         /* show pinout */
  #define MODE_HIGH           1         /* charge cap: high current */
  #define MODE_LOW            2         /* charge cap: low current */
  #define MODE_DISCHARGE      3         /* discharge cap */
  #define MODE_CHANGED   0b10000000     /* mode has changed */

  /* show info */
  LCD_Clear();                          /* clear display */
#ifdef UI_COLORED_TITLES
  /* display: cap leakage */
  Display_ColoredEEString(CapLeak_str, COLOR_TITLE);
#else
  Display_EEString(CapLeak_str);      /* display: cap leakage */
#endif

  /* set start values */
  Mode = MODE_CHANGED;

  UpdateProbes(PROBE_1, 0, PROBE_3);    /* update register bits and probes */

  while (1)       /* processing loop */
  {
    /*
     *  display mode and set probes
     */

    if (Mode & MODE_CHANGED)       /* mode has changed */
    {
      Mode &= ~MODE_CHANGED;       /* clear flag */
      LCD_ClearLine2();            /* clear line #2 */

      switch (Mode)                /* based on mode */
      {
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

        default:            /* display pinout */
          /* probe-1: Vcc / probe-3: Gnd */
          Show_SimplePinout('+', 0, '-');
          LCD_ClearLine(3);             /* clear line #3 */
      }
    }
    else
    {
      if (Mode != MODE_PINOUT)
      {
        LCD_ClearLine3();            /* clear line #3 */

        switch (Mode)                /* based on mode */
        {
          case MODE_DISCHARGE:       /* discharge cap */
            /* voltage at cap (probe-1) */
            U1 = ReadU(Probes.Ch_1);           /* read voltage at probe-1 */
            Display_Value(U1, -3, 'V');        /* display voltage */

            /* check if cap is discharged */
            if (U1 <= CAP_DISCHARGED)          /* < threshold */
              /* start new check cycle */
              Mode = MODE_PINOUT | MODE_CHANGED; /* show pinout + set flag for changed mode */

            goto skip;  /* optimization: avoid extra branching/variable by jumping past common display code */

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
              Mode = MODE_LOW | MODE_CHANGED;  /* low current mode + set flag for changed mode */
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
        }

        /* common display output */
        /* display voltage across current shunt (Rl or Rh) */
        Display_Space();
        Display_Char('(');
        Display_Value(U1, -3, 'V');          /* display voltage */
        Display_Char(')');
      }

      /* wait for user feedback or timeout of 2s */
skip:
      Test = TestKey(2000, CHECK_KEY_TWICE | CHECK_BAT);
      /* also delay for next loop run */

      if (Test == KEY_TWICE)
        return;

      if (Test == KEY_SHORT            /* short key press */
#if defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH)
        || Test == KEY_RIGHT           /* right key */
#endif
      )
      {
        /* change mode */
        if (Mode == MODE_PINOUT)                 /* pinout mode */
          Mode = MODE_HIGH | MODE_CHANGED;       /* charge cap with high current */
        else if (Mode != MODE_DISCHARGE)         /* MODE_HIGH and MODE_LOW modes */
          Mode = MODE_DISCHARGE | MODE_CHANGED;  /* discharge cap */
      }
    }
  }

  /*
   *  clean up
   */

  /* local constants for Mode */
  #undef MODE_PINOUT
  #undef MODE_HIGH
  #undef MODE_LOW
  #undef MODE_DISCHARGE
}


#endif // SW_CAP_LEAKAGE
