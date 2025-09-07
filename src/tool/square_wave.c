/* ************************************************************************
 *
 *   square wave tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_SQUAREWAVE

/*
 *  local constants
 */


/*
 *  local variables
 */


/* ************************************************************************
 *   Signal Generator (just squarewave)
 * ************************************************************************ */

/*
 *  create square wave signal with variable frequency
 *  - uses probe #2 (OC1B) as output
 *    and probe #1 & probe #3 as ground
 *  - alternative: dedicated signal output via OC1B
 *  - requires additional keys (e.g. rotary encoder)
 *  - requires idle sleep mode to keep timer running when MCU is sleeping
 */

void SquareWave_SignalGenerator(void)
{
  uint8_t           Test;               /* user feedback */
  uint8_t           Index;              /* prescaler table index */
  uint8_t           Bits = 0;           /* prescaler register bits */
  uint16_t          Prescaler;          /* timer prescaler */
  uint16_t          Top;                /* counter's top value */
  uint16_t          Step;               /* step size */
  uint16_t          Temp;               /* temporary value */
  uint32_t          Value;              /* temporary value */

  /*
      fast PWM:             f_PWM = f_MCU / (prescaler * (1 + top))
      available prescalers: 1, 8, 64, 256, 1024
      top:                  (2^2 - 1) up to (2^16 - 1)

      ranges for a 8MHz MCU clock:
      prescaler  /pre       top 2^16     top 2^2
      1          8MHz       122Hz        2MHz
      8          1MHz       15.26Hz      250kHz
      64         125kHz     1.9Hz        31.25kHz
      256        31.25kHz   0.5Hz        7812.5Hz
      1024       7812.5Hz   0.12Hz       1953.125Hz 
  */

  ShortCircuit(0);                      /* make sure probes are not shorted */

  /* display info */
  LCD_Clear();

#ifdef UI_COLORED_TITLES
  /* display: Square Wave */
  Display_ColoredEEString_Space(SquareWave_str, COLOR_TITLE);
#else
  Display_EEString_Space(SquareWave_str);  /* display: Square Wave */
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
   *  set up Timer1 for PWM with 50% duty cycle 
   *  - fast PWM mode 
   *  - top value by OCR1A
   *  - OC1B non-inverted output
   */

  /* enable OC1B pin and set timer mode */
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B1) | (1 << COM1B0);
  TCCR1B = (1 << WGM13) | (1 << WGM12);

  /*
   *  processing loop
   */

  /* set values for default frequency: 1kHz */
  Index = 0;                       /* prescaler 1/1 */
  Prescaler = 1;                   /* prescaler 1/1 */
  Bits = (1 << CS10);              /* register bits for prescaler 1 */
  Top = (F_CPU / 1000) - 1;     /* top = f_MCU / (prescaler * f_PWM) - 1 */

  while (1)
  {
    /*
     *  auto-ranging
     */

    Test = Index;

    /* check if we should change the range */
    if (Top > 0x7FFF)              /* more than 15 bits */
    {
      if (Index < (NUM_TIMER1 - 1))     /* don't exceed upper prescaler limit */
        Index++;                   /* increase prescaler */
    }
    else if (Top < 0x0FFF)         /* less than 12 bits */
    {
      if (Index > 0)               /* don't exceed lower prescaler limit */
        Index--;                   /* decrease prescaler */
    }

    /* process changed range */
    if (Index != Test)             /* range has changed */
    {
      Step = Prescaler;            /* save old value */

      /* read new prescaler and register bits from table */
      Prescaler = DATA_read_word(&T1_Prescaler_table[Index]);
      Bits = DATA_read_byte(&T1_RegBits_table[Index]);

      /* adjust top value for changed prescaler */
      if (Index > Test)            /* larger prescaler */
      {
        /* decrease top value by same factor as the prescaler increased */
        Temp = Prescaler / Step;
        Top /= Temp;
      }
      else                         /* smaller prescaler */
      {
        /* increase top value by same factor as the prescaler decreased */
        Temp = Step / Prescaler;
        Top *= Temp;  
      }
    }

    /*
     *  change timer settings
     */

    /* update timer */
    TCCR1B = (1 << WGM13) | (1 << WGM12);    /* stop timer */
    TCNT1 = 0;                               /* reset counter */
    OCR1B = Top / 2;                         /* 50% duty cycle */
    OCR1A = Top;                             /* top value for frequency */
    TCCR1B = (1 << WGM13) | (1 << WGM12) | Bits;    /* (re)start timer */

    /*
     *  display frequency
     *  - f_PWM = f_MCU / (prescaler * (1 + top))
     */

    Value = F_CPU * 100;        /* scale to 0.01Hz */
    Value /= Prescaler;            /* / prescaler */
    Test = 2;                      /* 2 decimal places */

    /*
     *  optimize resolution of frequency without causing an overflow
     *  prescaler       :  1  8  64  256  1024
     *  decimal places  :  2  3   4    4     5
     */

    Temp = Prescaler;
    while (Temp >= 8)         /* loop through prescaler steps */
    {
      Value *= 10;            /* scale by factor 0.1 */
      Test++;                 /* one decimal place more */
      Temp /= 8;              /* next lower prescaler */
    }

    Value /= Top + 1;                   /* / (1 + top) */
    LCD_ClearLine2();
    Display_FullValue(Value, Test, 0);  /* display frequency */
    Display_EEString(Hertz_str);        /* display: Hz */

    /*
     *  user feedback
     */

    /* wait for user feedback */
    Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);

    /* consider rotary encoder's turning velocity */
    Step = UI.KeyStep;             /* get velocity (1-7) */

    if (Step > 1)                  /* larger step */
    {
      /* increase step size based on turning velocity */

      /* step^4: 16 81 256 625 1296 2401 */
      Step *= Step;                /* ^2 */
      Step *= Step;                /* ^2 */
    }

    /* process user input */
    if (Test == KEY_RIGHT)         /* encoder: right turn */
    {
      /* increase frequency -> decrease top value */
      Temp = Top - Step;                /* take advantage of underflow */
      if ((Temp > Top) || (Temp < 0x003))    /* underflow */
        Temp = 0x0003;                  /* lower limit */
      Top = Temp;                       /* set new value */
    }
    else if (Test == KEY_LEFT)     /* encoder: left turn */
    {
      /* decrease frequency -> increase top value */
      Temp = Top + Step;                /* take advantage of overflow */
      if ((Temp < Top)  || (Temp == 0xFFFF))      /* overflow */
        Temp = 0xFFFE;                  /* upper limit */
      Top = Temp;                       /* set new value */
    }
    else if (Test == KEY_TWICE)    /* two short key presses */
      goto quit;                        /* end loop */
    
    if (Test == KEY_LONG)     /* long key press */
    {
      /* set default frequency: 1kHz */
      Index = 0;                        /* prescaler 1/1 */
      Prescaler = 1;                    /* prescaler 1/1 */
      Bits = (1 << CS10);               /* register bits for prescaler 1 */
      Top = (F_CPU / 1000) - 1;      /* top = f_MCU / (prescaler * f) - 1 */
    }
  }

  /*
   *  clean up
   */

quit:

  TCCR1B = 0;                 /* disable timer */
  TCCR1A = 0;                 /* reset flags (also frees PB2) */

#ifndef HW_FIXED_SIGNAL_OUTPUT
  R_DDR = 0;                  /* set HiZ mode */
#endif

#ifdef HW_FIXED_SIGNAL_OUTPUT
  SIGNAL_DDR &= ~(1 << SIGNAL_OUT);     /* set HiZ mode */
#endif
}

#endif // SW_SQUAREWAVE
