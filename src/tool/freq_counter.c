/* ************************************************************************
 *
 *   freq counter tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
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
 *  local variables
 */


/* ************************************************************************
 *   simple frequency counter
 * ************************************************************************ */

#ifdef HW_FREQ_COUNTER_BASIC

/*
 *  basic frequency counter
 *  - frequency input: T0
 *  - requires idle sleep mode to keep timers running when MCU is sleeping
 */

void FrequencyCounter(void)
{
  uint8_t           Flag;               /* loop control flag */
  uint8_t           Test;               /* user feedback */
  uint8_t           Old_DDR;            /* old DDR state */
  uint8_t           Index;              /* prescaler table index */
  uint8_t           Bits;               /* prescaler register bits */
  uint16_t          GateTime;           /* gate time in ms */
  uint16_t          Top;                /* top value for timer */
  uint32_t          Value;              /* temporary value */

  /* local constants for Flag */
  #define RUN_FLAG       1         /* run flag */
  #define WAIT_FLAG      2         /* enter/run waiting loop */
  #define GATE_FLAG      3         /* gatetime flag */
  #define SHOW_FREQ      4         /* display frequency */

  /* show info */
  LCD_Clear();                          /* clear display */

#ifdef UI_COLORED_TITLES
  /* display: Freq. Counter */
  Display_ColoredEEString(FreqCounter_str, COLOR_TITLE);
#else
  Display_EEString(FreqCounter_str);  /* display: Freq. Counter */
#endif

  /*
   *  We use Timer1 for the gate time and Timer0 to count pulses of the
   *  unknown signal. Max. frequency for Timer0 is 1/4 of the MCU clock.
   */

  Flag = RUN_FLAG;            /* enter measurement loop */

  /*
      auto ranging

      Timer1 top value (gate time)
      - top = gatetime * MCU_cycles / prescaler 
      - gate time in 탎
      - MCU cycles per 탎
      - top max. 2^16 - 1

      Frequency
      - f = pulses / gatetime
      - pulses = f * gatetime

      range         gate time  prescaler  MCU clock  pulses
      ----------------------------------------------------------
      <10kHz           1000ms       1024  > 16MHz    <10k
                       1000ms        256  <= 16MHz   <10k      
      10kHz-100kHz      100ms         64  all        1k-10k
      >100kHz            10ms          8  all        >1k (<50k)
   */

  /* start values for autoranging (assuming high frequency) */
  GateTime = 10;                   /* gate time 10ms */
  Index = 1;                       /* prescaler table index (prescaler 8:1) */

  /* set up Timer0 (pulse counter) */
  TCCR0A = 0;                      /* normal mode (count up) */
  TIFR0 = (1 << TOV0);             /* clear overflow flag */
  TIMSK0 = (1 << TOIE0);           /* enable overflow interrupt */

  /* set up Timer1 (gate time) */
  TCCR1A = 0;                      /* normal mode (count up) */
  TIFR1 = (1 << OCF1A);            /* clear output compare A match flag */
  TIMSK1 = (1 << OCIE1A);          /* enable output compare A match interrupt */

  /*
   *  measurement loop
   */

  while (Flag > 0)
  {
    /* set up T0 as input (pin might be shared with display) */
    Old_DDR = COUNTER_DDR;              /* save current settings */
    COUNTER_DDR &= ~(1 << COUNTER_IN);  /* signal input */
    wait500us();                        /* settle time */

    /* update prescaler */
    Top = DATA_read_word(&T1_Prescaler_table[Index]);  /* prescaler value */
    Bits = DATA_read_byte(&T1_RegBits_table[Index]);   /* prescaler bits */

    /* calculate compare value for Timer1 (gate time) */
    /* top = gatetime * MCU_cycles / timer prescaler */
    Value = GateTime;                   /* gatetime (in ms) */
    /* * MCU cycles per 탎 and scale gatetime to 탎 */
    Value *= (MCU_CYCLES_PER_US * 1000);
    Value /= Top;                       /* divide by timer prescaler */
    Top = (uint16_t)Value;              /* use lower 16 bit */

    /* start timers */
    Pulses = 0;                         /* reset pulse counter */
    Flag = WAIT_FLAG;                   /* enter waiting loop */
    TCNT0 = 0;                          /* Timer0: reset pulse counter */
    TCNT1 = 0;                          /* Timer1: reset gate time counter */
    OCR1A = Top;                        /* Timer1: set gate time */
    TCCR1B = Bits;                      /* start Timer1: prescaler */
    TCCR0B = (1 << CS02) | (1 << CS01); /* start Timer0: clock source T0 on falling edge */

    /* wait for timer1 or key press */
    while (Flag == WAIT_FLAG)
    {
      if (TCCR1B == 0)                  /* Timer1 stopped by ISR */
        Flag = GATE_FLAG;               /* end loop and signal Timer1 event */
      else                              /* Timer1 still running */
      {
        /* wait for user feedback */
        Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);

        if (Test == KEY_TWICE)          /* two short key presses */
          Flag = 0;                     /* end processing loop */
      }
    }

    /* T0 pin might be shared with display */
    COUNTER_DDR = Old_DDR;              /* restore old settings */

    Cfg.OP_Control &= ~OP_BREAK_KEY;    /* clear break signal (just in case) */

    /*
     *  process measurement
     */

    if (Flag == GATE_FLAG)              /* got measurement */
    {
      /* total sum of pulses during gate period */
      Pulses += TCNT0;                  /* add counter of Timer0 */

      /*
       *  calculate frequency
       *  - f = pulses / gatetime
       *  - 20MHz MCU: 5M pulses per second at maximum
       *    with 10ms gate time max. 50k pulses
       */

      Value = Pulses;                   /* number of pulses */
      Value *= 1000;                    /* scale to ms */
      Value /= GateTime;                /* divide by gatetime (in ms) */
      Flag = SHOW_FREQ;                 /* display frequency */

      /* autoranging */
      if (Pulses > 10000)               /* range overrun */
      {
        if (GateTime > 10)              /* upper range limit not reached yet */
        {
          GateTime /= 10;               /* 1000ms -> 100ms -> 10ms */
          Index--;                      /* one prescaler step down */
#if F_CPU > 16000000 
          if (Index == 3) Index--;      /* skip 256, use 64 */
#endif
          Flag = RUN_FLAG;              /* don't display frequency */
        }
      }
      else if (Pulses < 1000)           /* range underrun */
      {
        if (GateTime < 1000)            /* lower range limit not reached yet */
        {
          GateTime *= 10;               /* 1ms -> 10ms -> 100ms -> 1000ms */
          Index++;                      /* one prescaler step up */
#if F_CPU > 16000000 
          if (Index == 3) Index++;      /* skip 256, use 1024 */
#endif
          Flag = RUN_FLAG;              /* don't display frequency */
        }
      }

      /* prevent display of "0 Hz" */
      if (Pulses == 0)                  /* no signal or f too low */
        Flag = RUN_FLAG;                /* don't display frequency */
    }

    /*
     *  display frequency (in line #2)
     */

    LCD_ClearLine2();                   /* clear line #2 */
    Display_Char('f');                  /* display: f */
    Display_Space();

    if (Flag == SHOW_FREQ)              /* valid frequency */
    {
      Display_Value2(Value);            /* display frequency */
      Display_EEString(Hertz_str);      /* display: Hz */
      Flag = RUN_FLAG;                  /* clear flag */
    }
    else                                /* invalid frequency */
      Display_Minus();                  /* display: no value */
  }

  /*
   *  clean up
   */

  TIMSK0 = 0;                 /* disable all interrupts for Timer0 */
  TIMSK1 = 0;                 /* disable all interrupts for Timer1 */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef WAIT_FLAG
  #undef GATE_FLAG
  #undef SHOW_FREQ
}

#endif // HW_FREQ_COUNTER_BASIC


/* ************************************************************************
 *   extended frequency counter
 * ************************************************************************ */

#ifdef HW_FREQ_COUNTER_EXT

/*
 *  extended frequency counter
 *  - uses frontend with buffer, prescaler and crystal oscillators
 *  - frequency input: T0 
 *  - control signals
 *    prescaler       - COUNTER_CTRL_DIV
 *    channel addr #0 - COUNTER_CTRL_CH0
 *    channel addr #1 - COUNTER_CTRL_CH1
 *  - prescaler
 *    0 - 1:1
 *    1 - 16:1 (or 32:1)
 *  - source channel address
 *    00 - buffered frequency input
 *    01 - unused
 *    10 - HF crystal oscillator
 *    11 - LF crystal oscillator
 *  - requires idle sleep mode to keep timers running when MCU is sleeping
 */

void FrequencyCounter(void)
{
  uint8_t           Flag;               /* loop control flag */
  uint8_t           Test = 0;           /* user feedback */
  uint8_t           InDir;              /* input DDR state */
  uint8_t           CtrlDir;            /* control DDR state */
  uint8_t           Channel;            /* input channel */
  uint8_t           Range;              /* range ID */
  uint8_t           Div = 0;            /* frequency prescaler */
  uint8_t           Index;              /* prescaler table index */
  uint8_t           Bits = 0;           /* prescaler register bits */
  uint16_t          GateTime = 0;       /* gate time in ms */
  uint16_t          Top = 0;            /* top value for timer */
  unsigned char     *String = NULL;     /* string pointer (EEPROM) */
  uint32_t          MinPulses = 0;      /* minimim pulses for range */
//  uint32_t          MaxPulses = 0;      /* maximum pulses for range */
  uint32_t          Value;              /* temporary value */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run flag */
  #define WAIT_FLAG           0b00000010     /* wait flag */
  #define GATE_FLAG           0b00000100     /* gatetime flag */
  #define UPDATE_CHANNEL      0b00001000     /* update source channel */
  #define UPDATE_RANGE        0b00010000     /* update measurement range */
  #define SHOW_FREQ           0b00100000     /* display frequency */
  #define RESCAN_FLAG         0b01000000     /* rescan */
  #define SKIP_FREQ           0b10000000     /* skip display of f */

  /* show info */
  LCD_Clear();                          /* clear display */

#ifdef UI_COLORED_TITLES
  /* display: Freq. Counter */
  Display_ColoredEEString(FreqCounter_str, COLOR_TITLE);
#else
  Display_EEString(FreqCounter_str);  /* display: Freq. Counter */
#endif

  /*
   *  We use Timer1 for the gate time and Timer0 to count pulses of the
   *  unknown signal. Max. frequency for Timer0 is 1/4 of the MCU clock.
   */

  /*
      auto ranging

      Timer1 top value (gate time)
      - top = gatetime * MCU_cycles / timer prescaler
      - gate time in 탎
      - MCU cycles per 탎
      - top max. 2^16 - 1

      Frequency
      - f = f-prescaler * pulses / gatetime
      - pulses = f * gatetime / f-prescaler

                    gate    timer      MCU       frequency
      range         time    prescaler  clock     prescaler  pulses
      ----------------------------------------------------------------
      n/a           3000ms       1024  all
      <100kHz       1000ms       1024  > 16MHz         1:1  <100k
                    1000ms        256  <= 16MHz        1:1  <100k
      100kHz-1MHz    100ms         64  all             1:1  10k-100k
      >1MHz          100ms         64  all            16:1  >6250 (<500k)
                     100ms         64  all            32:1  >3125 (<500k)
   */

  /* set up control lines */
  CtrlDir = COUNTER_CTRL_DDR;      /* get current direction */
  /* set to output mode */
  COUNTER_CTRL_DDR |= (1 << COUNTER_CTRL_DIV) | (1 << COUNTER_CTRL_CH0) | (1 << COUNTER_CTRL_CH1);

  /* set up Timer0 (pulse counter) */
  TCCR0A = 0;                      /* normal mode (count up) */
  TIFR0 = (1 << TOV0);             /* clear overflow flag */
  TIMSK0 = (1 << TOIE0);           /* enable overflow interrupt */

  /* set up Timer1 (gate time) */
  TCCR1A = 0;                      /* normal mode (count up) */
  TIFR1 = (1 << OCF1A);            /* clear output compare A match flag */
  TIMSK1 = (1 << OCIE1A);          /* enable output compare A match interrupt */

  /* set start values */
  Channel = 0;                     /* source channel: ext. frequency */
  Range = 2;                       /* start with highest range */
  Flag = RUN_FLAG | UPDATE_CHANNEL | UPDATE_RANGE;     /* set control flags */

#ifdef UI_QUARTZ_CRYSTAL
  Check.Symbol = SYMBOL_CRYSTAL;   /* set symbol ID */
#endif

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  set and display source channel
     */

    if (Flag & UPDATE_CHANNEL)          /* update channel settings */
    {
      switch (Channel)             /* get channel specifics */
      {
        case 0:     /* buffered frequency input */
          String = (unsigned char *)FreqInput_str;
          Test = 0;
          break;

        case 1:     /* high frequency crystal oscillator */
          String = (unsigned char *)HF_Crystal_str;
          Test = (1 << COUNTER_CTRL_CH1);
          break;

        case 2:     /* low frequency crystal oscillator */
          String = (unsigned char *)LF_Crystal_str;
          Test = (1 << COUNTER_CTRL_CH1) | (1 << COUNTER_CTRL_CH0);
      }

      /* set source channel */
      InDir = COUNTER_CTRL_PORT;        /* get current state */
      InDir &= ~((1 << COUNTER_CTRL_CH1) | (1 << COUNTER_CTRL_CH0));  /* clear channel lines */
      InDir |= Test;                    /* set channel lines */
      COUNTER_CTRL_PORT = InDir;        /* update port */

      /* display source channel (in line #3) */ 
      LCD_ClearLine3();                      /* clear line #3 */
      Display_EEString(CounterChannel_str);  /* display: "Ch" */
      Display_Space();
      Display_EEString(String);              /* display channel name */

#ifdef UI_QUARTZ_CRYSTAL
      if (Channel == 0)                 /* buffered frequency input */
        Clear_Symbol(4);                /* clear symbol in line #4 */
      else                              /* HF or LF crystal oscillator */
        Display_FancySemiPinout(4);     /* display crystal symbol in line #4 */
#endif

      /* restart scan in top range */
      Range = 2;                             /* select top range */
      Flag |= UPDATE_RANGE;                  /* update range */
      Flag &= ~(RESCAN_FLAG | SKIP_FREQ);    /* reset rescan */

      Flag &= ~UPDATE_CHANNEL;          /* clear flag */
    }

    /*
     *  settings for ranges
     */

    if (Flag & UPDATE_RANGE)       /* update range settings */
    {
      switch (Range)               /* get range specifics */
      {
        case 0:     /* <100kHz */
          Div = 1;                 /* frequency prescaler 1:1 */
#if F_CPU <= 16000000
            Index = 3;             /* table index 3: 256:1 */
#else
            Index = 4;             /* table index 4: 1024:1 */
#endif
          GateTime = 1000;         /* gate time: 1000ms */
          MinPulses = 0;           /* lower limit: none */
//          MaxPulses = 100000;      /* upper limit: 100k */
          break;

        case 1:     /* 100kHz-1MHz */
          Div = 1;                 /* frequency prescaler 1:1 */
          Index = 2;               /* table index 2: 64:1 */
          GateTime = 100;          /* gate time: 100ms */
          MinPulses = 10000;       /* lower limit: 10k */
//          MaxPulses = 100000;      /* upper limit: 100k */
          break;

        case 2:     /* >1MHz */
          Div = FREQ_COUNTER_PRESCALER; /* frequency prescaler 16:1 or 32:1 */
          Index = 2;               /* table index 2: 64:1 */
          GateTime = 100;          /* gate time: 100ms */
#if FREQ_COUNTER_PRESCALER == 16
            MinPulses = 6250;      /* lower limit: 6250 */
#elif FREQ_COUNTER_PRESCALER == 32
            MinPulses = 3125;      /* lower limit: 3125 */
#endif
//          MaxPulses = 0;           /* upper limit: none */
          break;
      }

      /* update Timer1 prescaler */
      Top = DATA_read_word(&T1_Prescaler_table[Index]);     /* prescaler value */
      Bits = DATA_read_byte(&T1_RegBits_table[Index]);      /* prescaler bits */

      /* calculate compare value for Timer1 (gate time) */
      /* top = gatetime * MCU_cycles / timer prescaler */
      Value = GateTime;                 /* gatetime (in ms) */
      /* * MCU cycles per 탎 and scale gatetime to 탎 */
      Value *= (MCU_CYCLES_PER_US * 1000);
      Value /= Top;                     /* divide by timer prescaler */
      Top = (uint16_t)Value;            /* use lower 16 bit */

      /* update frequency counter prescaler */
      if (Div == FREQ_COUNTER_PRESCALER)     /* 16:1 / 32:1 */
        /* enable frequency prescaler */
        COUNTER_CTRL_PORT |= (1 << COUNTER_CTRL_DIV);       /* set pin high */
      else                                   /* 1:1 */
        /* disable frequency prescaler */
        COUNTER_CTRL_PORT &= ~(1 << COUNTER_CTRL_DIV);      /* set pin low */

      Flag &= ~UPDATE_RANGE;            /* clear flag */
    }  

    /* set up T0 as input */
    InDir = COUNTER_DDR & (1 << COUNTER_IN);      /* get current direction */
    COUNTER_DDR &= ~(1 << COUNTER_IN);  /* set to input mode */
    wait500us();                        /* settle time */

    /* start timers */
    Flag |= WAIT_FLAG;                  /* enter waiting loop */
    Pulses = 0;                         /* reset pulse counter */
    TCNT0 = 0;                          /* Timer0: reset pulse counter */
    TCNT1 = 0;                          /* Timer1: reset gate time counter */
    OCR1A = Top;                        /* Timer1: set gate time */
    TCCR1B = Bits;                      /* start Timer1: prescaler */
    TCCR0B = (1 << CS02) | (1 << CS01); /* start Timer0: clock source T0 on falling edge */

    /*
     *  wait for timer1 or user feedback
     */

    while (Flag & WAIT_FLAG)
    {
      if (TCCR1B == 0)                  /* Timer1 stopped by ISR */
      {
        Flag |= GATE_FLAG;                   /* signal Timer1 event */
        Flag &= ~WAIT_FLAG;                  /* end waiting loop */
      }
      else                              /* Timer1 still running */
      {
        /* wait for user feedback */
        Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);

        if (Test == KEY_SHORT)          /* short key press */
        {
          /* select next source channel */
          if (Channel < 2)
            Channel++;        /* next channel */
          else
            Channel = 0;                  /* overrun */

          Flag |= UPDATE_CHANNEL;            /* update channel */
          Flag &= ~WAIT_FLAG;                /* end waiting loop */
        }
        else if (Test == KEY_TWICE)     /* two short key presses */
          Flag = 0;                          /* end processing loop */
#if defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH)
        else if (Test == KEY_RIGHT)     /* right key */
        {
          if (Channel < 2)
            Channel++;        /* next channel */
          else
            Channel = 0;                  /* overrun */

          Flag |= UPDATE_CHANNEL;            /* update channel */
          Flag &= ~WAIT_FLAG;                /* end waiting loop */
        }
        else if (Test == KEY_LEFT)      /* left key */
        {
          if (Channel > 0)
            Channel--;        /* previous channel */
          else
            Channel = 2;                  /* underrun */

          Flag |= UPDATE_CHANNEL;            /* update channel */
          Flag &= ~WAIT_FLAG;                /* end waiting loop */
        }
#endif // HW_KEYS
      }
    }

    if (InDir)                     /* restore old setting for T0 */
      COUNTER_DDR |= (1 << COUNTER_IN);      /* set to output mode */

    Cfg.OP_Control &= ~OP_BREAK_KEY;    /* clear break signal (just in case) */

    /*
     *  process measurement
     */

    if (Flag & GATE_FLAG)               /* got measurement */
    {
      /* total sum of pulses during gate period */
      Pulses += TCNT0;                  /* add counter of Timer0 */

      /*
       *  autoranging
       */

      if (Pulses < MinPulses)           /* range underrun */
      {
        if (Range > 0)                  /* not lowest range yet */
        {
          Range--;                      /* change to lower range */
          Flag |= UPDATE_RANGE;         /* update range */
        }
      }

#if 0
      else if (Pulses > MaxPulses)      /* range overrun */
      {
        if (Range < 2)                  /* not highest range yet */
        {
          Range++;                      /* change to higher range */
          Flag |= UPDATE_RANGE;         /* update range */
        }
      }
#endif

      /* show frequency only when not switching ranges */
      if (! (Flag & UPDATE_RANGE))      /* no change of range */
      {
        /* prevent display of "0 Hz" (no signal or f too low) */
        if (Pulses)                     /* got signal */
          Flag |= SHOW_FREQ;            /* show frequency */

        /* manage rescan */
        Flag &= ~(RESCAN_FLAG | SKIP_FREQ);       /* reset flags */

        if (Range < 2)                            /* not top range */
        {
          Range = 2;                              /* change to top range */
          Flag |= (UPDATE_RANGE | RESCAN_FLAG);   /* update range and rescan */
        }
      }

      /*
       *  calculate frequency
       *  - f = pulses * f-prescaler / gatetime
       *  - 20MHz MCU: 5M pulses per second at maximum
       *    with 100ms gate time max. 500k pulses
       */

      if (Div == 1)                     /* f-prescaler 1:1 */
      {
        /* no overflow possible */
        Pulses *= 1000;                 /* scale to ms */
        Pulses /= GateTime;             /* divide by gatetime (in ms) */
      }
      else                              /* f-prescaler 16:1 or 32:1 */
      {
        /* prevent overflow */
        Pulses *= 100;                  /* scale to 10ms */
        Pulses *= Div;                  /* * f-prescaler */
        Pulses /= (GateTime / 10);      /* divide by gatetime (in 10ms) */
      }

      Flag &= ~GATE_FLAG;          /* clear flag */
    }

    /*
     *  display frequency (in line #2)
     */

    if (! (Flag & SKIP_FREQ))      /* update frequency display */
    {
      LCD_ClearLine2();            /* clear line #2 */
      Display_Char('f');           /* display: f */
      Display_Space();

      if (Flag & SHOW_FREQ)        /* valid frequency */
      {
        /* determine prefix */
        Test = 0;                  /* dot position */
        Index = 0;                 /* unit char */

        if (Pulses >= 1000000)     /* f >= 1MHz */
        {
          Test = 6;                     /* 10^6 */
          Index = 'M';                  /* M for mega */
        }
        else if (Pulses >= 1000)   /* f >= 1kHz */
        {
          Test = 3;                     /* 10^3 */
          Index = 'k';                  /* k for kilo */
        }

        /* display frequency */
        Display_FullValue(Pulses, Test, Index);
        Display_EEString(Hertz_str);    /* display: "Hz" */

        Flag &= ~SHOW_FREQ;             /* clear flag */
      }
      else                         /* invalid frequency */
        Display_Minus();           /* display: no value */

      /* manage rescan */
      if (Flag & RESCAN_FLAG)      /* in rescan mode */
        /* prevent any updates while in rescan mode */
        Flag |= SKIP_FREQ;         /* skip frequency display */
    }
  }

  /*
   *  clean up
   */

  TIMSK0 = 0;                 /* disable all interrupts for Timer0 */
  TIMSK1 = 0;                 /* disable all interrupts for Timer1 */

  /* filter control lines which were in input mode */ 
  CtrlDir ^= (1 << COUNTER_CTRL_DIV) | (1 << COUNTER_CTRL_CH0) | (1 << COUNTER_CTRL_CH1);
  CtrlDir &= (1 << COUNTER_CTRL_DIV) | (1 << COUNTER_CTRL_CH0) | (1 << COUNTER_CTRL_CH1);
  COUNTER_CTRL_DDR &= ~CtrlDir;         /* set former direction */

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef WAIT_FLAG
  #undef GATE_FLAG
  #undef UPDATE_CHANNEL
  #undef UPDATE_RANGE
  #undef SHOW_FREQ
  #undef RESCAN_FLAG
  #undef SKIP_FREQ
}

#endif // HW_FREQ_COUNTER_EXT
