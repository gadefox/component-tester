/* ************************************************************************
 *
 *   pwm simple tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_PWM_SIMPLE

/*
 *  local constants
 */

/* PWM menu: frequencies */    
const uint16_t PWM_Freq_table[NUM_PWM_FREQ] MEM_TYPE = {100, 250, 500, 1000, 2500, 5000, 10000, 25000};


/*
 *  local variables
 */


/* ************************************************************************
 *   PWM: simple PWM generator
 * ************************************************************************ */

/*
 *  PWM generator with simple UI
 *  - uses probe #2 (OC1B) as PWM output
 *    and probe #1 & probe #3 as ground
 *  - alternative: dedicated signal output via OC1B
 *  - max. reasonable PWM frequency for 8MHz MCU clock is 40kHz
 *  - requires idle sleep mode to keep timer running when MCU is sleeping
 *
 *  requires:
 *  - Freqency in Hz
 */

void PWM_Tool(uint16_t Frequency)
{
  uint8_t           Test;               /* loop control and user feedback */
  uint8_t           Ratio;              /* PWM ratio (in %) */
  uint8_t           Bits;               /* bits for timer prescaler */
  uint16_t          Top;                /* top value */
  uint16_t          Toggle;             /* counter value to toggle output */
  uint32_t          Value;              /* temporary value */
#ifdef PWM_SHOW_DURATION
  uint16_t          Time;               /* duration/resolution of timer step */
#endif

  /*
   *  Timer1:
   *  - phase correct PWM:    f_PWM = f_MCU / (2 * prescaler * top)
   *  - available prescalers: 1, 8, 64, 256, 1024
   *  - range of top:         (2^2 - 1) up to (2^16 - 1)
   *
   *  - ranges for a 8MHz MCU clock:
   *    prescaler  /2pre     top 2^16   top 2^2    top 100
   *    1          4MHz      61Hz       1MHz       40kHz
   *    8          500kHz    7.6Hz      125kHz     5kHz
   *    64         62.5kHz   0.95Hz     15.625kHz  625Hz
   *    256        15625Hz   0.24Hz     3906.25Hz  156.25Hz
   *  - to support a PWM ratio of 1% top should be at least 100
   */

  ShortCircuit(0);                      /* make sure probes are not shorted */

  /* display info */
  LCD_Clear();

#ifdef UI_COLORED_TITLES
  /* display: PWM */
  Display_ColoredEEString_Space(PWM_str, COLOR_TITLE);
#else
  Display_EEString_Space(PWM_str);    /* display: PWM */
#endif

  Display_Value2(Frequency);            /* display frequency */
  Display_EEString(Hertz_str);          /* display: Hz */

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
   *  calculate required prescaler and top value based on MCU clock
   *  - top = f_MCU / (2 * prescaler * f_PWM)
   */

  Value = F_CPU / 2;                 /* /2 */
  Value /= Frequency;                   /* /f_PWM */

  if (Value > 2000000)        /* low frequency (<20Hz @8MHz) */
  {
#ifdef PWM_SHOW_DURATION
    Toggle = 256;                     /* prescaler 256 */
#else
    Value /= 256;                     /* /prescaler */
#endif
    Bits = (1 << CS12);                 /* prescaler bits for 256:1 */
  }
  else if (Value > 16000)     /* mid-range frequency (<250Hz @8MHz) */
  {
#ifdef PWM_SHOW_DURATION
    Toggle = 64;                      /* prescaler 64 */
#else
    Value /= 64;                      /* /prescaler */
#endif
    Bits = (1 << CS11) | (1 << CS10);   /* prescaler bits for 64:1 */
  }
  else                        /* high frequency */
  {
#ifdef PWM_SHOW_DURATION
    Toggle = 1;                       /* prescaler 1 */
#endif
    Bits = (1 << CS10);                 /* prescaler bits for 1:1 */
  }

#ifdef PWM_SHOW_DURATION
  Value /= Toggle;                 /* /prescaler */
#endif
 
  Top = (uint16_t)Value;           /* keep lower 16 bits */

#ifdef PWM_SHOW_DURATION
  /* calculate duration of timer step */
  /* t = (1 / f_MCU) * 2 * prescaler = 2 * prescaler / f_MCU */
  Value = 2000000000 / F_CPU;        /* 2/f_MCU in ns */
  Value *= Toggle;                      /* * prescaler */
  Time = (uint16_t)Value;               /* keep lower 16 bits */
#endif

  /* set start values */
  Ratio = 50;                 /* default ratio is 50% */

#if 0
  /* calculate toggle value: top * (ratio / 100) */
  Value = (uint32_t)Top * Ratio;
  Value /= 100;
  Toggle = (uint16_t)Value;
#endif

  Toggle = Top / 2;           /* compare value for 50% */

  /*
   *  set up Timer1 for PWM
   *  - phase correct PWM
   *  - top value by OCR1A
   *  - OC1B non-inverted output
   */

  TCCR1B = 0;                                /* stop timer */
  /* enable OC1B pin and set timer mode */
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
  TCCR1B = (1 << WGM13);
  TCNT1 = 0;                                 /* set counter to 0 */
  OCR1A = Top;                               /* set top value (-1) */
  OCR1B = Toggle;                            /* set value to compare with */

  /* start counter by setting clock prescaler */
  TCCR1B = (1 << WGM13) | Bits;

  /*
   *  ratio control
   */

  while (1)
  {
    /* show PWM ratio in line #2 */
    LCD_ClearLine2();
    Display_Value(Ratio, 0, '%');       /* show ratio in % */

#ifdef PWM_SHOW_DURATION
    /* and also pulse duration */
    Display_Space();
    /* pulse duration = duration of timer step * toggle value */ 
    Value = (uint32_t)Time * OCR1B;          /* in ns */
    Display_Value(Value, -9, 's');
#endif

    /*
     *  user interface
     *  - short key press -> increase ratio
     *    long key press -> decrease ratio
     *    two short key presses -> exit tool
     */

    /* wait for user feedback */
    Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);
    
    if (Test == KEY_TWICE)              /* two short key presses */
      goto quit;                        /* end loop */

    if (Test == KEY_SHORT)              /* short key press */
    {
      if (Ratio <= 95)
        Ratio += 5;      /* +5% and limit to 100% */
    }
#if defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH)
    else if (Test == KEY_RIGHT)         /* right key */
    {
      if (Ratio <= 99)
        Ratio += 1;      /* +1% and limit to 100% */
    }
    else if (Test == KEY_LEFT)          /* left key */
    {
      if (Ratio >= 1)
        Ratio -= 1;       /* -1% and limit to 0% */
    }
#endif
    else                                /* long key press */
    {
      if (Ratio >= 5)
        Ratio -= 5;       /* -5% and limit to 0% */
    }

    /* calculate toggle value: top * (ratio / 100) */
    Value = (uint32_t)Top * Ratio;
    Value /= 100;
    OCR1B = (uint16_t)Value;            /* update compare value */

#if defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH)
    if (Test <= KEY_LONG)               /* just for test button usage */
      MilliSleep(500);                  /* smooth UI */
#else
    MilliSleep(500);                    /* smooth UI */
#endif
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
}


#endif // SW_PWM_SIMPLE
