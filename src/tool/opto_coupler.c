/* ************************************************************************
 *
 *   opto coupler tool (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_OPTO_COUPLER

/*
 *  local constants
 */


/*
 *  local variabless
 */

const unsigned char If_str[] MEM_TYPE = "If";
const unsigned char t_on_str[] MEM_TYPE = "t_on";
const unsigned char t_off_str[] MEM_TYPE = "t_off";


/* ************************************************************************
 *   opto coupler check
 * ************************************************************************ */

/*
 *  check for LED
 *  - simple wrapper for CheckDiode()
 *
 *  requires:
 *  - Probe1: ID of positive probe (anode)
 *  - Probe2: ID of negative probe (cathode)
 */

void Check_LED(uint8_t Probe1, uint8_t Probe2)
{
  uint16_t          U1;                 /* voltage */

  UpdateProbes2(Probe1, Probe2);        /* update probes */

  /* we assume: probe-1 = A / probe2 = C */
  /* set probes: Gnd -- Rl -- probe-2 / probe-1 -- Vcc */
  R_PORT = 0;                      /* set resistor port to Gnd */
  R_DDR = Probes.Rl_2;             /* pull down probe-2 via Rl */
  ADC_DDR = Probes.Pin_1;          /* set probe-1 to output */
  ADC_PORT = Probes.Pin_1;         /* pull-up probe-1 directly */

  U1 = ReadU_5ms(Probes.Ch_2);     /* voltage at Rl (cathode) */
  if (U1 >= 977)         /*  not just a leakage current (> 1.4mA) */
    CheckDiode();        /* run standard diode check */
}


/*
 *  check opto couplers
 *  - uses standard probes
 *  - pins which have to be connected (common Gnd):
 *    - LED's cathode and BJT's emitter 
 *    - LED's cathode and TRIAC's MT2
 *  - supports:
 *    - BJT
 *    - TRIAC (with and without zero crossing circuit)
 */

void OptoCoupler_Tool(void)
{
  uint8_t           Run = 1;            /* loop control */
  uint8_t           Test;               /* user input */
  uint16_t          U1, U2;             /* voltages */
  uint16_t          U3, U4;             /* voltages */
  uint32_t          CTR = 0;            /* CTR in % */

  /* local constants for status */
  #define DETECTED_LED        50
  #define DETECTED_BJT       100
  #define DETECTED_TRIAC     101

  /* init */
  /* next-line mode: keep first line and wait for key/timeout */
  UI.LineMode = LINE_KEEP | LINE_KEY;

  /* display info */
  LCD_Clear();
#ifdef UI_COLORED_TITLES
  /* display: Opto Coupler */
  Display_ColoredEEString(OptoCoupler_str, COLOR_TITLE);
#else
  Display_EEString(OptoCoupler_str);  /* display: Opto Coupler */
#endif
  Display_NL_EEString(Start_str);       /* display: Start */

  /*
   *  processing loop
   */

  while (Run)
  {
    /* user input */

    /* wait for user feedback */
    Test = TestKey(0, CURSOR_BLINK | CHECK_KEY_TWICE | CHECK_BAT);

    if (Test == KEY_TWICE)         /* two short key presses */
      Run = 0;                     /* end loop */

    if (Run)                       /* check opto coupler */
    {
      /* update display */
      LCD_Clear();
#ifdef UI_SERIAL_COPY
      Display_Serial_On();         /* enable serial output & NL */
#endif

#ifdef UI_COLORED_TITLES
        /* display: Opto Coupler */
        Display_ColoredEEString(OptoCoupler_str, COLOR_TITLE);
#else
        Display_EEString(OptoCoupler_str);   /* display: Opto Coupler */
#endif
      Display_NextLine();
      Test = 0;                    /* reset status */

      /*
       *  scan for LED
       */

      Check.Found = COMP_NONE;          /* reset component search */
      Check.Diodes = 0;                 /* reset number of diodes */

      /* check all possible probe combinations */
      Check_LED(PROBE_1, PROBE_2);
      Check_LED(PROBE_2, PROBE_1);
      Check_LED(PROBE_1, PROBE_3);
      Check_LED(PROBE_3, PROBE_1);
      Check_LED(PROBE_2, PROBE_3);
      Check_LED(PROBE_3, PROBE_2);

      if (Check.Diodes == 1)       /* got one */
      {
        /* update probes for remaining checks (3rd probe is done automatically) */
        UpdateProbes2(Diodes[0].A, Diodes[0].C);

        Test = DETECTED_LED;            /* proceed with other checks */
      }

      /*
       *  we assume:
       *  probe-1 = LED's anode
       *  probe-2 = LED's cathode & BJT's emitter or TRIAC's MT2
       *  probe-3 = BJT's collector or TRIAC's MT1
       */

      /*
       *  check for BJT and TRIAC
       *  - BJT conducts only while LED is lit.
       *  - TRIAC keeps conducting as long as load current flows.
       *    Some types with zero crossing circuit got an inhibit voltage
       *    of about 5V.
       */

      if (Test == DETECTED_LED)         /* LED detected */
      {
        /* set probes: probe-2 -- Gnd / probe-3 -- Rl -- Vcc */
        ADC_DDR = Probes.Pin_2;              /* set probe-2 to output */
        ADC_PORT = 0;                        /* pull down probe-2 directly */
        R_DDR = Probes.Rl_1 | Probes.Rl_3;   /* select Rl for probe-1 & Rl for probe-3 */
        R_PORT = Probes.Rl_3;                /* pull up collector via Rl */
        U1 = ReadU_5ms(Probes.Ch_3);         /* voltage at collector when LED is off */

        /* make sure we have no conduction without the LED lit */
        if (U1 > 4000)        /* allow a leakage current of 1.5mA */
        {
          /* simulate zero crossing in case of a TRIAC with zero crossing circuit */
          R_PORT = Probes.Rl_1;                /* turn on LED */
          wait1ms();                           /* wait a tad */
          R_PORT = Probes.Rl_1 | Probes.Rl_3;  /* also pull up collector via Rl */
          U1 = ReadU_5ms(Probes.Ch_3);         /* voltage at collector when LED is on */

          R_PORT = Probes.Rl_3;                /* turn off LED */
          U2 = ReadU_5ms(Probes.Ch_3);         /* voltage at collector when LED is off */

          /* we should have conduction when the LED is lit */
          if (U1 <= 4000)          /* more than 1.5mA */
          {
            if (U2 >= 4000)        /* no conduction, allow some leakage current */
              Test = DETECTED_BJT;      /* BJT type */
            else                   /* conduction */
            {
              /* check if both voltages are about the same */
              U3 = U1;
              U3 /= 8;             /* 12.5% */
              U4 = U1 - U3;        /* lower threshold */
              U3 += U1;            /* upper threshold */
              if ((U2 > U4) && (U2 < U3))
                Test = DETECTED_TRIAC;  /* TRIAC type */
            }
          }
        }

        R_DDR = Probes.Rl_1;                 /* set probe-3 to HiZ */
      }

      /*
       *  measure CRT for BJT type
       */

      if (Test == DETECTED_BJT)         /* BJT type */
      {
        /* change probes: probe-3 -- Vcc */
        ADC_DDR = Probes.Pin_2 | Probes.Pin_3;    /* set probe-3 to output */
        ADC_PORT = Probes.Pin_3;                  /* pull up probe-3 directly */

        /* get voltages at current shunts */
        Cfg.Samples = 10;               /* just a few samples for 1ms runtime */
        R_PORT = Probes.Rl_1;           /* turn LED on */
        wait1ms();                      /* time for propagation delay */
        U1 = ReadU(Probes.Ch_1);        /* voltage at LED's anode (Rl) */
        U2 = ReadU(Probes.Ch_2);        /* voltage at emitter (RiL) */
        R_PORT = 0;                     /* turn LED off */
        Cfg.Samples = ADC_SAMPLES;      /* reset samples to default */

        /* calculate LED's If */
        /* If = (Vcc - U1) / (RiH + Rl) */
        U3 = Cfg.Vcc - U1;              /* Vcc - U1 (mV) */
        CTR = (uint32_t)U3;
        CTR *= 10000;                   /* scale to 0.0001 mV */
        U4 = NV.RiH + (R_LOW * 10);     /* RiH + Rl (0.1 Ohms) */
        CTR /= U4;                      /* If = U/R in µA */
        U3 = (uint16_t)CTR;             /* If in µA */

        /* calculate BJT's Ie */
        /* Ie = I_total - If = (U2 / RiL) - If */
        CTR = (uint32_t)U2;             /* U2 (mV) */
        CTR *= 10000;                   /* scale to 0.0001 mV */
        CTR /= NV.RiL;                  /* /RiL in 0.1 Ohms -> I_total (µA) */ 
        CTR -= U3;                      /* Ie = I_total - If (µA) */

        /* calculate CTR */
        /* CTR = Ie / If */
        CTR *= 100;                     /* scale up to % */
        CTR /= U3;                      /* Ie / If (%) */
      }

      /*
       *  Measure turn-on and turn-off times
       *  - Unfortunately we can't use the analog comparator in conjunction
       *    with Timer1, because the 1.1V bandgap reference would limit the
       *    time measurement to opto couplers with a CTR > 200%.
       */

      if (Test == DETECTED_BJT)    /* BJT type */
      {
        U1 = UINT16_MAX;           /* reset value */
        U2 = UINT16_MAX;

        ADC_DDR = Probes.Pin_2;              /* set probe-2 to output */
        ADC_PORT = 0;                        /* pull down probe-2 directly */
        R_DDR = Probes.Rl_1 | Probes.Rl_3;   /* select Rl for probe-1 & Rl for probe-3 */
        R_PORT = Probes.Rl_3;                /* pull up collector via Rl */

        U1 = ReadU_5ms(Probes.Ch_3);         /* voltage at collector when LED is off */

        /* make sure we have no conduction without the LED lit */
        if (U1 > 4000)        /* allow a leakage current of 1.5mA */
        {
          Test = Probes.Pin_3;     /* port pin mask for probe-3 */

          /*
           *  turn-on delay
           */

          Run = 0;                                /* zero counter */
          R_PORT = Probes.Rl_1 | Probes.Rl_3;     /* turn on LED */

          /*
           *  wait for logic low level (<2.0V)
           *  - MCU cycles for full loop run: 7
           */

          while (ADC_PIN & Test)
          {
            Run++;                      /* increase counter */
            if (Run > 250) break;       /* check for overflow */
          }

          if (Run <= 250)          /* no overrun */
          {
            U1 = Run * 70;                   /* delay (0.1 MCU cycles) */
            U1 /= MCU_CYCLES_PER_US;         /* delay (0.1 µs) */
          }

          /*
           *  turn-off delay
           */

          Run = 0;                                /* zero counter */
          R_PORT = Probes.Rl_3;                   /* turn off LED */

          /*
           *  wait for logic high level (>2.5V)
           *  - MCU cycles for full loop run: 7
           */

          while (!(ADC_PIN & Test))
          {
            Run++;                      /* increase counter */
            if (Run > 250) break;       /* check for overflow */
          }

          if (Run <= 250)          /* no overrun */
          {
            U2 = Run * 70;                   /* delay (0.1 MCU cycles) */
            U2 /= MCU_CYCLES_PER_US;         /* delay (0.1 µs) */
          }

          Run = 1;            /* reset value */
          Test = 100;         /* reset value */
        }
      }

      /*
       *  display result
       */

      if (Test == DETECTED_BJT)         /* BJT type */
      {
        Display_EEString(BJT_str);      /* display: BJT */

        Display_NL_EEString_Space(CTR_str);       /* display: CTR */
        Display_Value(CTR, 0, '%');               /* display CTR */

        Display_NL_EEString_Space(If_str);        /* display: If */
        Display_Value(U3, -6, 'A');               /* display If */

        if (U1 < UINT16_MAX)       /* valid t_on */
        {
          Display_NL_EEString_Space(t_on_str);    /* display: t_on */
          if (U1 < 10)        /* < 1µs */
          {
            Display_Char('<');
            U1 = 10;          /* 1µs */
          }
          Display_Value(U1, -7, 's');
        }

        if (U2 < UINT16_MAX)       /* valid t_off */
        {
          Display_NL_EEString_Space(t_off_str);   /* display: t_off */
          if (U2 < 10)        /* < 1µs */
          {
            Display_Char('<');
            U2 = 10;          /* 1µs */
          }
          Display_Value(U2, -7, 's');
        }

        Display_NL_EEString_Space(Vf_str);        /* display: Vf */
        Display_Value(Diodes[0].V_f, -3, 'V');    /* display Vf */
      }
      else if (Test == DETECTED_TRIAC)  /* TRIAC type */
      {
        Display_EEString(Triac_str);    /* display: TRIAC */

        Display_NL_EEString_Space(Vf_str);        /* display: Vf */
        Display_Value(Diodes[0].V_f, -3, 'V');    /* display Vf */
      }
      else                              /* none found */
        Display_EEString(None_str);     /* display: None */

#ifdef UI_SERIAL_COPY
      Display_Serial_Off();        /* disable serial output & NL */
#endif
    }
  }

  /* clean up local constants for status */
  #undef DETECTED_LED
  #undef DETECTED_BJT
  #undef DETECTED_TRIAC
}

#endif // SW_OPTO_COUPLER
