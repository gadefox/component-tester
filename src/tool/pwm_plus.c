/* ************************************************************************
 *
 *   pwm plus tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_PWM_PLUS

/*
 *  local constants
 */


/*
 *  local variables
 */


/* ************************************************************************
 *   PWM: extended PWM generator
 * ************************************************************************ */

/*
 *  PWM generator with improved UI
 *  - uses probe #2 (OC1B) as PWM output
 *    and probe #1 & probe #3 as ground
 *  - alternative: dedicated signal output via OC1B
 *  - max. reasonable PWM frequency for 8MHz MCU clock is 40kHz
 *  - requires additional keys (e.g. rotary encoder) and
 *    display with more than 2 text lines
 *  - requires idle sleep mode to keep timer running when MCU is sleeping
 */

void PWM_Tool(void)
{
  uint8_t           Status;             /* loop control */
  uint8_t           Mode;               /* UI */
  uint8_t           Test = 0;           /* user feedback */
  uint8_t           Step;               /* step size */
  uint8_t           Ratio;              /* PWM ratio (in %) */
  uint8_t           Index;              /* prescaler table index */
  uint8_t           Bits;               /* prescaler register bits */
  uint16_t          Prescaler;          /* timer prescaler */
  uint16_t          Top;                /* top value */
  uint16_t          Step2;              /* step size */
  uint16_t          Temp;               /* temporary value */
  uint32_t          Value;              /* temporary value */
#ifdef PWM_SHOW_DURATION
  uint16_t          TimeValue;          /* duration/resolution of timer step */
  int8_t            TimeScale;          /* scale of duration */
#endif

  /* local constants for status (bitfield) */
  #define CHANGE_FREQ    0b00000001     /* change frequency */
  #define CHANGE_RATIO   0b00000010     /* change ratio */
  #define DISPLAY_FREQ   0b00000100     /* display frequency */
  #define DISPLAY_RATIO  0b00001000     /* display ratio */

  /* local constants for Mode */
  #define MODE_FREQ               1     /* frequency mode */
  #define MODE_RATIO              2     /* ratio mode */

  /*
   *  Timer1:
   *  - phase & frequency correct PWM:  f_PWM = f_MCU / (2 * prescaler * top)
   *  - available prescalers:           1, 8, 64, 256, 1024
   *  - range of top:                   (2^2 - 1) up to (2^16 - 1)
   *  - ranges for a 8MHz MCU clock:
   *    prescaler  /2pre       top 2^16   top 2^2    top 100
   *    1          4MHz        61Hz       1MHz       40kHz
   *    8          500kHz      7.6Hz      125kHz     5kHz
   *    64         62.5kHz     0.95Hz     15.625kHz  625Hz
   *    256        15625Hz     0.24Hz     3906.25Hz  156.25Hz
   *    1024       3906.25Hz   0.06Hz     976.5Hz    39Hz
   *  - to support a PWM ratio of 1% top should be at least 100
   */

  ShortCircuit(0);                    /* make sure probes are not shorted */

  /* display info */
  LCD_Clear();

#ifdef UI_COLORED_TITLES
  /* display: PWM */
  Display_ColoredEEString_Space(PWM_str, COLOR_TITLE);
#else
  Display_EEString_Space(PWM_str);    /* display: PWM */
#endif

#ifndef HW_FIXED_SIGNAL_OUTPUT
  ProbePinout(PROBES_PWM);              /* show probes used */
#endif

#ifndef HW_FIXED_SIGNAL_OUTPUT
  /* set up probes: #1 and #3 are signal ground, #2 is signal output */
  ADC_PORT = 0;                         /* pull down directly: */
  ADC_DDR = (1 << TP1) | (1 << TP3);    /* probe 1 & 3 */
  R_DDR = (1 << R_RL_2);                /* enable Rl for probe 2 */
  R_PORT = 0;                           /* pull down probe 2 initially */
#endif

#ifdef HW_FIXED_SIGNAL_OUTPUT
  /* dedicated output via OC1B */
  SIGNAL_PORT &= ~(1 << SIGNAL_OUT);    /* low by default */
  SIGNAL_DDR |= (1 << SIGNAL_OUT);      /* enable output */
#endif

  /*
   *  set up Timer1 for PWM
   *  - phase and frequency correct PWM
   *  - top value by OCR1A
   *  - OC1B non-inverted output
   */

  TCNT1 = 0;                            /* set counter to 0 */

  /* enable OC1B pin and set timer mode */
  TCCR1A = (1 << WGM10) | (1 << COM1B1);
  TCCR1B = (1 << WGM13);

  /*
   *  processing loop
   */

  /* start values */
  /* top = f_MCU / (2 * prescaler * f_PWM) */
  Top = (F_CPU / 2000);         /* 1kHz */
  Ratio = 50;                      /* 50% PWM ratio */
  Prescaler = 1;                   /* prescaler 1:1 */
  Index = 0;                       /* first index of prescaler tables */
  Bits = (1 << CS10);              /* register bits for prescaler 1:1 */
  Status = CHANGE_FREQ | CHANGE_RATIO | DISPLAY_FREQ | DISPLAY_RATIO;
  Mode = MODE_FREQ;                /* frequency mode */

  while (1)       /* processing loop */
  {
    /*
     *  change timer settings
     */

    if (Status & CHANGE_FREQ)        /* change frequency */
    {
      /*
       *  auto-ranging
       */

      Step = Index;                /* save old index */

      /* check if we should change the range */
      if (Top > 0x7FFF)            /* more than 15 bits */
      {
        if (Index < (NUM_TIMER1 - 1))   /* don't exceed upper prescaler limit */
          Index++;                 /* increase prescaler */
      }
      else if (Top < 0x0FFF)       /* less than 12 bits */
      {
        if (Index > 0)             /* don't exceed lower prescaler limit */
          Index--;                 /* decrease prescaler */
      }

      /* process changed range */
      if (Index != Step)           /* range has changed */
      {
        Step2 = Prescaler;         /* save old value */

        /* read new prescaler and register bits from table */
        Prescaler = DATA_read_word(&T1_Prescaler_table[Index]);
        Bits = DATA_read_byte(&T1_RegBits_table[Index]);

        if (Index > Step)          /* larger prescaler */
        {
          /* decrease top value by same factor as the prescaler increased */
          Temp = Prescaler / Step2;
          Top /= Temp;
        }
        else                       /* smaller prescaler */
        {
          /* increase top value by same factor as the prescaler decreased */
          Temp = Step2 / Prescaler;
          Top *= Temp;
        }
      }

      /* set frequency */
      OCR1A = Top;                      /* set top value */
      TCCR1B = (1 << WGM13) | Bits;     /* (re)start timer */

#ifdef PWM_SHOW_DURATION
      /* calculate duration of timer step */
      TimeScale = -9;                        /* ns */
      /* t = (1 / f_MCU) * 2 * prescaler = 2 * prescaler / f_MCU */
      Value = 2000000000 / F_CPU;         /* 2/f_MCU in ns */
      Value *= Prescaler;                    /* * prescaler */
      while (Value > UINT16_MAX)             /* rescale */
      {
        Value /= 1000;                       /* /1000 */
        TimeScale += 3;                      /* 10^3 */
      } 
      TimeValue = (uint16_t)Value;           /* keep lower 16 bits */
#endif // PWM_SHOW_DURATION

      Status &= ~CHANGE_FREQ;             /* clear flag */
      /* a frequency change implies a ratio change */
    }

    if (Status & CHANGE_RATIO)       /* change ratio */
    {
      /* toggle = top * (ratio / 100) */
      Value = (uint32_t)Top * Ratio;
      Value /= 100;
      OCR1B = (uint16_t)Value;          /* set compare/toggle value */

#ifdef PWM_SHOW_DURATION
      /* also update the display of the pulse duration */
      Status |= DISPLAY_RATIO;            /* display ratio */
#endif

      Status &= ~CHANGE_RATIO;            /* clear flag */
    }

    /*
     *  update display
     */

    if (Status & DISPLAY_FREQ)       /* display frequency in line #2 */
    {
      LCD_ClearLine2();
      MarkItem(MODE_FREQ, Mode);        /* mark mode if selected */

      /* f_PWM = f_MCU / (2 * prescaler * top) */
      Value = F_CPU * 50;            /* scale to 0.01Hz and /2 */
      Value /= Prescaler;               /* /prescaler */
      Step = 2;                         /* 2 decimal places */

      /*
       *  optimize resolution of frequency without causing an overflow
       *  prescaler       :  1  8  64  256  1024
       *  decimal places  :  2  3   4    4     5
       */

      Temp = Prescaler;
      while (Temp >= 8)         /* loop through prescaler steps */
      {
        Value *= 10;            /* scale by factor 0.1 */
        Step++;                 /* one decimal place more */
        Temp /= 8;              /* next lower prescaler */
      }

      Value /= Top;             /* /top */

      Display_FullValue(Value, Step, 0);  /* display frequency */
      Display_EEString(Hertz_str);        /* display: Hz */

      Status &= ~DISPLAY_FREQ;            /* clear flag */
    }

    if (Status & DISPLAY_RATIO)      /* display ratio in line #3 */
    {
      LCD_ClearLine3();                 /* clear line #3 */
      MarkItem(MODE_RATIO, Mode);       /* mark mode if selected */

      Display_Value(Ratio, 0, '%');     /* show ratio in % */

#ifdef PWM_SHOW_DURATION
      /* and also pulse duration */
      Display_Space();
      /* pulse duration = duration of timer step * toggle value */ 
      Value = (uint32_t)TimeValue * OCR1B;
      Display_Value(Value, TimeScale, 's');
#endif

      Status &= ~DISPLAY_RATIO;           /* clear flag */
    }

    /* smooth UI after long key press */
    if (Test == KEY_LONG)          /* long key press */
      SmoothLongKeyPress();             /* delay next key press */

    /*
     *  user feedback
     */

    /* wait for user feedback */
    Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);
    
    if (Test == KEY_TWICE)              /* two short key presses */
      goto quit;                        /* end loop */
 
    /* consider rotary encoder's turning velocity */
    Step = UI.KeyStep;             /* get velocity (1-7) */
    Step2 = Step;
    if (Step > 1)                  /* larger step */
    {
      /* increase step size based on turning velocity */
      if (Mode == MODE_FREQ)       /* frequency mode */
      {
        /*
         *  value ranges for each prescaler:
         *  -    1:  100 -> 32767 /  100 <- 32760
         *  -    8: 4095 -> 32767 / 4095 <- 32760
         *  -   64: 4095 -> 32767 / 4095 <- 16380
         *  -  256: 8191 -> 32767 / 4095 <- 16380
         *  - 1024: 8191 -> 65635 / 4095 <- 65635
         */

        /* step^4: 16 81 256 625 1296 2401 */
        Step2 *= Step;             /* ^2 */
        Step2 *= Step2;            /* ^2 */
      }
      else                         /* ratio mode */
        /* 0-100% */
        Step *= 100 / 32;
    }

    /* process user input */
    switch (Test )
    {
      case KEY_SHORT:              /* short key press */
        /* toggle frequency/ratio mode */
        if (Mode == MODE_FREQ)     /* frequency mode */
          Mode = MODE_RATIO;       /* change to ratio mode */
        else                       /* ratio mode */
          Mode = MODE_FREQ;        /* change to frequency mode */

        Status |= DISPLAY_FREQ | DISPLAY_RATIO;  /* update display */
        break;
   
      case KEY_LONG:               /* long key press */
        if (Mode == MODE_FREQ)       /* frequency mode */
        {
          /* set 1kHz */
          Prescaler = 1;
          Index = 0;
          Bits = (1 << CS10);        /* prescaler register bits for 1 */
          Top = (F_CPU / 2000);   /* 1kHz */
          Status |= CHANGE_FREQ | DISPLAY_FREQ | CHANGE_RATIO;   /* set flags */
        }
        else                         /* ratio mode */
        {
          /* set 50% */
          Ratio = 50;
          Status |= CHANGE_RATIO | DISPLAY_RATIO;     /* set flags */
        }
        break;

      case KEY_RIGHT:              /* right key */
        if (Mode == MODE_FREQ)       /* frequency mode */
        {
          /* increase frequency -> decrease top */
          Temp = Top - Step2;        /* take advantage of underflow */
          if ((Temp > Top) || (Temp < 0x0064))      /* underflow */
            Temp = 0x0064;           /* lower limit */
          Top = Temp;                /* set new value */

          Status |= CHANGE_FREQ | DISPLAY_FREQ | CHANGE_RATIO;  /* set flags */
        }
        else                         /* ratio mode */
        {
          /* increase ratio */
          Ratio += Step;             /* add (max 200) */
          if (Ratio > 100)           /* limit exceeded */
            Ratio = 100;             /* max. is 100 */

          Status |= CHANGE_RATIO | DISPLAY_RATIO;     /* set flags */
        }
        break;

      case KEY_LEFT:               /* left key */
        if (Mode == MODE_FREQ)       /* frequency mode */
        {
          /* decrease frequency -> increase top */
          Temp = Top + Step2;        /* take advantage of overflow */
          if (Temp < Top)            /* overflow */
            Temp = 0xFFFF;           /* upper limit */
          Top = Temp;                /* set new value */

          Status |= CHANGE_FREQ | DISPLAY_FREQ | CHANGE_RATIO;  /* set flags */
        }
        else                         /* ratio mode */
        {
          /* decrease ratio */
          if (Ratio > Step)          /* no underflow */
            Ratio -= Step;           /* new ratio */
          else                       /* underflow */
            Ratio = 0;               /* lower limit is 0 */

          Status |= CHANGE_RATIO | DISPLAY_RATIO;     /* set flags */
        }
    }
  }

  /*
   *  clean up
   */

quit:
  TCCR1B = 0;                 /* disable timer */
  TCCR1A = 0;                 /* reset flags (also frees PB2) */

#ifndef HW_FIXED_SIGNAL_OUTPUT
  R_DDR = 0;                            /* set HiZ mode */
#endif

#ifdef HW_FIXED_SIGNAL_OUTPUT
  SIGNAL_DDR &= ~(1 << SIGNAL_OUT);     /* set HiZ mode */
#endif

  /* local constants for Mode */
#undef MODE_RATIO
#undef MODE_FREQ

  /* local constants for Flag */
#undef DISPLAY_RATIO
#undef DISPLAY_FREQ
#undef CHANGE_RATIO
#undef CHANGE_FREQ
}


#endif // SW_PWM_PLUS
