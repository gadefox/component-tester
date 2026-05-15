/* ************************************************************************
 *
 *   servo tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef SW_SERVO

/*
 *  local constants
 */


/*
 *  local variables
 */

/* sweep function for servo tester */
uint8_t                  SweepStep;     /* sweep step */
volatile uint8_t         SweepDir;      /* sweep direction */


/* ************************************************************************
 *   PWM: servo check
 * ************************************************************************ */

/*
 *  Servo Check, PWM generator for testing servos
 *  - uses probe #2 (OC1B) as PWM output
 *    and probe #1 & probe #3 as ground
 *  - alternative: dedicated signal output via OC1B
 *  - requires additional keys (e.g. rotary encoder) and
 *    display with more than 2 lines
 *  - requires idle sleep mode to keep timers running when MCU is sleeping
 */

void Servo_Check(void)
{
  uint8_t           Flag;               /* loop control */
  uint8_t           Mode;               /* UI mode */
  uint8_t           Test = 0;           /* user feedback */
  uint8_t           Index;              /* PWM index */
  uint8_t           Period[4] = {200, 80, 40, 30};  /* in 0.1ms */
  uint16_t          Toggle;             /* toggle value */
  uint16_t          Step;               /* step size */
  uint16_t          Temp;               /* temporary value */
  uint32_t          Value;              /* temporary value */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG       0b00000001     /* run / otherwise end */
  #define SWEEP_MODE     0b00000010     /* sweep mode */
  #define CHANGE_PULSE   0b00000100     /* change pulse width */
  #define CHANGE_FREQ    0b00001000     /* change frequency */
  #define DISPLAY_PULSE  0b00010000     /* display pulse width */
  #define DISPLAY_FREQ   0b00100000     /* display frequency */
  #define TOGGLE_SWEEP   0b01000000     /* enter/leave sweep operation */

  /* local constants for Mode */
  #define MODE_PULSE              1     /* pulse width mode */
  #define MODE_FREQ               2     /* frequency mode */

  /*
   *  MCU clock specific value
   *  - step size for a resolution of about 0.01ms
   *  - 8MHz: 5, 16MHz: 10, 20MHz: 13
   */

#if F_CPU == 8000000
  #define PULSE_STEP        5
#elif F_CPU == 16000000
  #define PULSE_STEP        10
#elif F_CPU == 20000000
  #define PULSE_STEP        13
#else
  #error <<< No or wrong MCU type selected! >>>
#endif


  /*
   *  PWM for servos:
   *  - frequency
   *    50Hz / 20ms  analog servo
   *    125Hz / 8ms  digital servo
   *    250Hz / 4ms  high speed digital servo
   *    333Hz / 3ms  high speed digital servo
   *  - pulse 1 - 2ms (allow 0.5 - 2.5ms)
   *    left   1.0ms
   *    mid    1.5ms
   *    right  2.0ms
   *  - typical rotation is 90-120� & 180�
   *  - typical speed is 30-500ms/60�
   */

  ShortCircuit(0);                 /* make sure probes are not shorted */

  /* display info */
  LCD_Clear();

#ifdef UI_COLORED_TITLES
  /* display: Servo */
  Display_ColoredEEString_Space(Servo_str, COLOR_TITLE);
#else
  Display_EEString_Space(Servo_str);  /* display: Servo */
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
   *  calculate required values for PWM based on MCU clock
   *  - Timer1, phase correct PWM
   *  - top = f_MCU / (2 * prescaler * f_PWM)
   *        = (f_MCU * t_PWM) / (2 * prescaler)
   *  - toggle = (f_MCU * t_pulse) / (2 * prescaler)
   *  - use prescaler 1:8 for best resolution across all MCU clocks
   *    with t_pulse in 0.1ms
   *    = ((f_MCU / 10000) * t_pulse) / 16
   */

  /* PWM toggle values (t_pulse) */
  #define SERVO_LEFT_MAX      (((F_CPU / 10000) * 5) / 16)    /* 0.5ms */
  #define SERVO_LEFT_NORM     (((F_CPU / 10000) * 10) / 16)   /* 1.0ms */
  #define SERVO_MID           (((F_CPU / 10000) * 15) / 16)   /* 1.5ms */
  #define SERVO_RIGHT_NORM    (((F_CPU / 10000) * 20) / 16)   /* 2.0ms */  
  #define SERVO_RIGHT_MAX     (((F_CPU / 10000) * 25) / 16)   /* 2.5ms */

  /* sweep control */
  #define SERVO_STEP_MAX      (SERVO_LEFT_NORM / 10)   /* toggle_1ms / 10 */

  /*
   *  calculate required values for sweep timer
   *  - Timer0, CTC mode
   *  - t_step = 3ms -> f_step = 333Hz
   *  - prescaler = 1024
   *  - top = (f_MCU / (f_step * prescaler)) - 1
   *        = (t_step / (t_MCU_cycle * prescaler)) - 1
   *  - t_step = t_MCU_cycle * prescaler * (top + 1)
   *  - SERVO_STEP_TIME in 탎
   */

  #define SERVO_SWEEP_TOP     (((F_CPU / 333) / 1024) - 1)
  #define SERVO_STEP_TIME     ((MCU_CYCLE_TIME * 1024 * (SERVO_SWEEP_TOP + 1)) / 10000)

  /*
   *  set up Timer0 for sweeping
   *  - CTC mode
   *  - top value by OCR0A (double buffered)
   *  - fixed prescaler 1:1024
   */

  TCCR0B = 0;                      /* disable Timer0 */
  TCNT0 = 0;                       /* reset counter */
  OCR0A = SERVO_SWEEP_TOP;         /* set compare value */
  TCCR0A = (1 << WGM01);           /* set timer mode */
  TIMSK0 = (1 << OCIE0A);          /* enable output compare match A interrupt */

  /*
   *  set up Timer1 for PWM
   *  - phase correct PWM
   *  - top value by OCR1A (buffered)
   *  - OC1B non-inverted output
   *  - fixed prescaler 1:8
   */

  TCNT1 = 0;                       /* reset counter to 0 */
  TIMSK1 = 0;                      /* disable all interrupts for Timer1 */

  /* enable OC1B pin and set timer mode */
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1B1);
  TCCR1B = (1 << WGM13) | (1 << CS11);  /* start Timer1 by setting prescaler */

  /* set start values */
  Toggle = SERVO_MID;              /* toggle value (1.5ms) */
  Index = 0;                       /* #0 (20.0ms) */
  SweepStep = 0;                   /* no step */
  SweepDir = 0;                    /* no direction */
  Mode = MODE_PULSE;               /* pulse width mode */
  Flag = RUN_FLAG | MODE_PULSE | CHANGE_PULSE | CHANGE_FREQ | DISPLAY_PULSE | DISPLAY_FREQ;

  /*
   *  TODO:
   *  - since the pulse length is displayed with a resolution of 0.01ms
   *    a visible change might need several steps
   *  - improve UI to give visual feedback for each step
   */

  while (Flag > 0)       /* processing loop */
  {
    /*
     *  change timer settings
     */

    /* change pulse width */
    if (Flag & CHANGE_PULSE)
    {
      OCR1B = Toggle;                   /* set toggle value */

      Flag &= ~CHANGE_PULSE;            /* clear flag */
    }

    /* change frequency */
    if (Flag & CHANGE_FREQ)
    {
      /* top = ((f_MCU / 10000) * t_pulse) / 16 */
      Test = Period[Index];             /* get period */
      Value = (F_CPU / 10000);       /* MCU clock in 10kHz */
      Value *= Test;
      Value /= 16; 
      OCR1A = (uint16_t)Value;          /* set top value */

      Flag &= ~CHANGE_FREQ;             /* clear flag */
    }

    /*
     *  update display
     */

    /* display pulse duration / sweep period */
    if (Flag & DISPLAY_PULSE)
    {
      LCD_ClearLine2();                 /* line #2 */
      MarkItem(MODE_PULSE, Mode);       /* mark mode if selected */

      if (Flag & SWEEP_MODE)            /* sweep mode */
      {
        /*
         *  calculate sweep time
         *  - t_sweep = t_step * (toggle_1ms / step)
         */

        Value = SERVO_STEP_TIME;        /* step time in 탎 (around 3000) */
        Value *= SERVO_LEFT_NORM;       /* * toggle value for 1ms */
        Value /= SweepStep;             /* / step size (in 탎) */
      }
      else                              /* normal mode */
      {
        /*
         *  calculate pulse length
         *  - t = (toggle * 2 * prescaler) / f_MCU
         */

        Value = (uint32_t)Toggle;
        Value *= 16000;                 /* * (2 * prescaler) (in 0.001) */
        Value /= (F_CPU / 1000);     /* / f_MCU (in 1탎) */
      }

      /* display value */
      Display_FullValue(Value, 3, 'm');
      Display_Char('s');

      Flag &= ~DISPLAY_PULSE;           /* clear flag */
    }

    /* display PWM frequency/period */
    if (Flag & DISPLAY_FREQ)
    {
      LCD_ClearLine3();                 /* clear line #3 */
      MarkItem(MODE_FREQ, Mode);        /* mark mode if selected */

      Test = Period[Index];             /* get period */
      Value = 10000 / Test;             /* calculate frequency */
      Display_Value2(Value);            /* display frequency */
      Display_EEString(Hertz_str);      /* display: Hz */

      if (Flag & SWEEP_MODE)            /* in sweep mode */
      {
        Display_Space();
        Display_EEString(Sweep_str);    /* display: sweep */
      }

      Flag &= ~DISPLAY_FREQ;            /* clear flag */
    }

    /* smooth UI after long key press */
    if (Test == KEY_LONG)          /* long key press */
      SmoothLongKeyPress();             /* delay next key press */

    /*
     *  user feedback
     */

    /* wait for user feedback */
    Test = TestKey(0, CURSOR_BLINK | CHECK_KEY_TWICE | CHECK_BAT);

    /* consider rotary encoder's turning velocity (1-7) */
    Step = UI.KeyStep;             /* get velocity */
    if (Step > 1)                  /* larger step */
    {
      /* increase step size based on turning velocity */
      Step--;

      if (Flag & SWEEP_MODE)       /* in sweep mode */
        /*
         *  MCU clock specific value range
         *  - 8MHz: 1-50, 16MHz: 1-100, 20MHz: 1-125
         */
        Step *= (SERVO_STEP_MAX / 32) + 1;
      else                         /* in normal mode */
        /*
         *  MCU clock specific value range
         *  - 8MHz: 250-1250, 16MHz: 500-2500, 20MHz: 625-3125
         *  - use multiples of 0.01ms step size
         */
        Step *= PULSE_STEP * ((SERVO_RIGHT_MAX - SERVO_LEFT_MAX) / 500);
    }
    else                           /* single step */
    {
      if (! (Flag & SWEEP_MODE))   /* in normal mode */
      {
        /*
         *  MCU clock specific value
         *  - change step size for a resolution of about 0.01ms
         */

        Step = PULSE_STEP;
      }
    }

    /* process user input */
    if (Test == KEY_SHORT)              /* short key press */
    {
      /* toggle pulse/frequency mode */
      if (Mode == MODE_PULSE)           /* pulse width mode */
        Mode = MODE_FREQ;               /* change to frequency mode */
      else                              /* frequency mode */
        Mode = MODE_PULSE;              /* change to pulse width mode */

      Flag |= DISPLAY_PULSE | DISPLAY_FREQ;     /* update display */
    }
    else if (Test == KEY_TWICE)         /* two short key presses */
      Flag = 0;                         /* end loop */
    else if (Test == KEY_LONG)          /* long key press */
    {
      if (Mode == MODE_PULSE)           /* pulse width mode */
      {
        if (Flag & SWEEP_MODE)          /* in sweep mode */
        {
          /* return to slowest sweep speed */
          SweepStep = 1;                /* smallest step */
          Flag |= DISPLAY_PULSE;        /* set flag */
        }
        else                            /* in normal mode */
        {
          /* return to middle position (1.5ms) */
          Toggle = SERVO_MID;           /* set mid */
          Flag |= CHANGE_PULSE | DISPLAY_PULSE;   /* set flags */
        }
      }
      else                              /* frequency mode */
      {
        if (Flag & SWEEP_MODE)          /* in sweep mode */
          /* leave sweep mode */
          Flag &= ~SWEEP_MODE;          /* clear flag */
        else                            /* in normal mode */
          /* enter sweep mode */
          Flag |= SWEEP_MODE;           /* set flag */

        Flag |= DISPLAY_PULSE | DISPLAY_FREQ | TOGGLE_SWEEP;  /* set flags */
      }
    }
    else if (Test == KEY_RIGHT)         /* right key */
    {
      if (Mode == MODE_PULSE)           /* pulse width mode */
      {
        if (Flag & SWEEP_MODE)          /* in sweep mode */
        {
          /* increase sweep speed -> increase sweep step */
          Temp = SweepStep + Step;
          if (Temp > SERVO_STEP_MAX)
            Temp = SERVO_STEP_MAX;
          SweepStep = (uint8_t)Temp;
          Flag |= DISPLAY_PULSE;        /* set flag */
        }
        else                            /* in normal mode */
        {
          /* increase pulse length -> increase toggle value */
          Temp = Toggle + Step;         /* take advantage of overflow */
          if ((Temp < Toggle) || (Temp > SERVO_RIGHT_MAX))  /* overflow */
            Temp = SERVO_RIGHT_MAX;     /* upper limit */
          Toggle = Temp;                /* set new value */
          Flag |= CHANGE_PULSE | DISPLAY_PULSE;   /* set flags */
        }
      }
      else                              /* frequency mode */
      {
        /* next PWN frequency -> increase index */
        if (Index < 3)                  /* upper limit is 3 */
        {
          Index++;                      /* next one */
          Flag |= DISPLAY_FREQ | CHANGE_FREQ;     /* set flags */
        }       
      }
    }
    else if (Test == KEY_LEFT)          /* left key */
    {
      if (Mode == MODE_PULSE)           /* pulse width mode */
      {
        if (Flag & SWEEP_MODE)          /* in sweep mode */
        {
          /* decrease sweep speed -> decrease sweep step */
          Temp = SweepStep - Step;
          if (Step >= SweepStep)
            Temp = 1;
          SweepStep = (uint8_t)Temp;
          Flag |= DISPLAY_PULSE;        /* set flag */
        }
        else                            /* in normal mode */
        {
          /* decrease pulse length -> decrease toggle value */
          Temp = Toggle - Step;         /* take advantage of underflow */
          if ((Temp > Toggle) || (Temp < SERVO_LEFT_MAX))   /* underflow */
          {
            Temp = SERVO_LEFT_MAX;      /* lower limit */
          }
          Toggle = Temp;                /* set new value */
          Flag |= CHANGE_PULSE | DISPLAY_PULSE;   /* set flags */
        }
      }
      else                              /* frequency mode */
      {
        /* previous PWM frequency -> decrease index */
        if (Index > 0)                  /* lower limit is 0 */
        {
          Index--;                      /* previous one */
          Flag |= DISPLAY_FREQ | CHANGE_FREQ;     /* set flags */
        }
      }
    }

    /*
     *  enter/leave sweep operation
     *  - use Timer0 as sweep timer
     */

    if (Flag & TOGGLE_SWEEP)
    {
      if (Flag & SWEEP_MODE)            /* enter sweeping */
      {
        /* set start values */
        SweepStep = 1;                  /* forward */
        SweepDir = 1;                   /* lowest speed */

        /* start sweep timer */
        TCNT0 = 0;                      /* reset counter */
        TCCR0B = (1 << CS02) | (1 << CS00);  /* enable timer by setting prescaler */  
      }
      else                              /* exit sweeping */
        /* stop sweep timer */
        TCCR0B = 0;                     /* disable Timer0 */

      Flag &= ~TOGGLE_SWEEP;            /* clear flag */
    }
  }

  /*
   *  clean up
   */

  TCCR0B = 0;                 /* disable Timer0 */
  TIMSK0 = 0;                 /* disable all interrupts for Timer0 */
  TCCR1B = 0;                 /* disable Timer1 */
  TCCR1A = 0;                 /* reset flags (also frees PB2) */

#ifndef HW_FIXED_SIGNAL_OUTPUT
  R_DDR = 0;                            /* set HiZ mode */
#endif

#ifdef HW_FIXED_SIGNAL_OUTPUT
  SIGNAL_DDR &= ~(1 << SIGNAL_OUT);     /* set HiZ mode */
#endif

  /* local constants for sweeping */
  #undef SERVO_STEP_TIME
  #undef SERVO_SWEEP_TOP
  #undef SERVO_STEP_MAX
  #undef SERVO_RIGHT_MAX
  #undef SERVO_RIGHT_NORM
  #undef SERVO_MID
  #undef SERVO_LEFT_MAX
  #undef SERVO_LEFT_NORM

  /* local constant for step size */
  #undef PULSE_STEP

  /* local constants for Mode */
  #undef MODE_FREQ
  #undef MODE_PULSE

  /* local constants for Flag */
  #undef TOGGLE_SWEEP
  #undef DISPLAY_FREQ
  #undef DISPLAY_PULSE
  #undef CHANGE_FREQ
  #undef CHANGE_PULSE
  #undef SWEEP_MODE
  #undef RUN_FLAG
}


/*
 *  ISR for match of Timer0's OCR0A (Output Compare Register A)
 *  - sweep timer for servo check  
 */

ISR(TIMER0_COMPA_vect, ISR_BLOCK)
{
  uint16_t          Temp;     /* temp. value */

  /*
   *  HINTs:
   *  - the OCF0A interrupt flag is cleared automatically
   *  - interrupt processing is disabled while this ISR runs
   *    (no nested interrupts)
   */

  /* toggle values for PWM */
  #define SERVO_LEFT_NORM     (((F_CPU / 10000) * 10) / 16)   /* 1.0ms */
  #define SERVO_RIGHT_NORM    (((F_CPU / 10000) * 20) / 16)   /* 2.0ms */

  /*
   *  update PWM pulse length (Timer1)
   */

  Temp = OCR1B;               /* get current compare value */

  if (SweepDir == 1)          /* forward */
  {
    Temp += SweepStep;        /* add step */

    if (Temp >= SERVO_RIGHT_NORM)  /* exceeded upper limit */
    {
      Temp = SERVO_RIGHT_NORM;     /* limit */
      SweepDir = 2;                /* change direction */
    }
  }
  else                        /* backward */
  {
    Temp -= SweepStep;        /* substract step */

    if (Temp <= SERVO_LEFT_NORM)   /* exceeded lower limit */
    {
      Temp = SERVO_LEFT_NORM;      /* limit */
      SweepDir = 1;                /* change direction */
    }
  }

  OCR1B = Temp;               /* set new compare value */

  #undef SERVO_LEFT_NORM
  #undef SERVO_RIGHT_NORM
}


#endif // SW_SERVO
