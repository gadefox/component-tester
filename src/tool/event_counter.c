/* ************************************************************************
 *
 *   event counter tools (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef HW_EVENT_COUNTER

/*
 *  local constants
 */


/*
 *  local variables
 */

/* time counter */
volatile uint8_t         TimeTicks;     /* tick counter */
volatile uint16_t        TimeCounter;   /* time counter */


/* ************************************************************************
 *   counter: event counter
 * ************************************************************************ */

/*
 *  ISR for match of Timer1's OCR1B (Output Compare Register B)
 *  - for time ticks of event counter
 */

ISR(TIMER1_COMPB_vect, ISR_BLOCK)
{
  /*
   *  hints:
   *  - the OCF1B interrupt flag is cleared automatically
   *  - interrupt processing is disabled while this ISR runs
   *    (no nested interrupts)
   */

  /* time ticks */
  TimeTicks++;                     /* got another tick */
  if (TimeTicks >= 5)              /* 5 ticks = 1 second */
  {
    TimeTicks = 0;                 /* reset tick counter */
    TimeCounter++;                 /* got another second */
  }

  TIFR1 = (1 << OCF1A);                 /* clear output compare A match flag */

  /* break TestKey() processing */
  Cfg.OP_Control |= OP_BREAK_KEY;       /* set break signal */
}


/*
 *  event counter
 *  - counter input: T0
 *    needs to be dedicated pin (not in parallel with display!)
 *  - requires additional keys (e.g. rotary encoder) and
 *    display with more than 5 lines
 *  - requires idle sleep mode to keep timers running when MCU is sleeping
 *  - requires MCU clock of 8, 16 or 20MHz
 */

void EventCounter(void)
{
  uint8_t           Flag;               /* loop control flag */
  uint8_t           Test;               /* user feedback */
  uint8_t           CounterMode;        /* counter mode */
  uint8_t           Item;               /* UI item */
  uint8_t           Show;               /* display control */
  uint8_t           Temp;               /* temp. value */
  uint16_t          Step;               /* step size */
  unsigned char     *String = NULL;     /* string pointer (EEPROM) */
  uint16_t          TimeTrigger;        /* time limit/trigger */
  uint32_t          EventsTrigger;      /* events limit/trigger */
  uint32_t          Events;             /* events */

  /* local constants for Flag (bitfield) */
  #define RUN_FLAG            0b00000001     /* run flag */
  #define WAIT_FLAG           0b00000010     /* wait flag */
  #define IDLE_FLAG           0b00000100     /* idle flag (not counting) */
  #define DELAY_FLAG          0b00001000     /* delay flag */
  #define START_COUNTING      0b00010000     /* start counting */
  #define MANAGE_COUNTING     0b00100000     /* manage counting */
  #define STOP_COUNTING       0b01000000     /* stop counting */

  /* local constants for CounterMode */
  #define MODE_COUNT          1         /* count events and time (start/stop) */
  #define MODE_TIME           2         /* count events during given time period */
  #define MODE_EVENTS         3         /* count time for given number of events */

  /* local constants for Item */
  #define UI_COUNTERMODE      1         /* counter mode */
  #define UI_EVENTS           2         /* events */
  #define UI_TIME             3         /* time */
  #define UI_STARTSTOP        4         /* start/stop */

  /* local constants for Show (follows UI items, bitfield) */
  #define SHOW_MODE           0b00000001     /* show mode */
  #define SHOW_EVENTS         0b00000010     /* show events */
  #define SHOW_TIME           0b00000100     /* show time */
  #define SHOW_STARTSTOP      0b00001000     /* show start/stop */

  /* local constants for defaults and maximums */
  #define DEFAULT_TIME        60             /* one minute */
  #define DEFAULT_EVENTS      100            /* ? */
  #define MAX_TIME            43200          /* 12h (in seconds) */
  #define MAX_EVENTS          4000000000     /* ? */

  /* show flags based on item number */
  uint8_t UI_Index[4] = { SHOW_MODE, SHOW_EVENTS, SHOW_TIME, SHOW_STARTSTOP };

#ifdef EVENT_COUNTER_TRIGGER_OUT
  /*
   *  init probe pins:
   *  - probe #1: Gnd
   *  - probe #2: trigger output (default: low)
   *  - probe #3: Gnd
   */

  /* set probes: probe-1 -- Gnd / probe-2 -- Rl -- Gnd / probe-3 -- Gnd */
  ADC_PORT = 0;                         /* pull down directly: */
  ADC_DDR = (1 << TP1) | (1 << TP3);    /* probe #1 and probe #3 */
  R_PORT = 0;                           /* pull down: */
  R_DDR = (1 << R_RL_2);                /* probe #2 via Rl */
#endif

  /* show info */
  LCD_Clear();                          /* clear display */

#ifdef UI_COLORED_TITLES
  /* display: Event Counter */
  Display_ColoredEEString(EventCounter_str, COLOR_TITLE);
#else
  Display_EEString(EventCounter_str); /* display: Event Counter */
#endif

  /*
   *  We use Timer1 for the time period and Timer0 to count the events.
   *  Max. event frequency for Timer0 is 1/4 of the MCU clock.
   */

  /*
   *  MCU     pre-     top     top
   *  Clock   scaler   1s      0.2s
   *   1MHz   1:64     15625    3125  (not supported)
   *   8MHz   1:256    31250    6250
   *  16MHz   1:256    62500   12500
   *  20MHz   1:256    78125   15625
   *
   *  - top = (f_MCU / (f_tick * prescaler)) - 1
   *        = (f_MCU * t_tick / prescaler) - 1
   *  - t_tick = 0.2s
   */

  #define TOP       (F_CPU / (5 * 256)) - 1

  /* set up Timer0 (event counter) */
  TCCR0A = 0;                      /* normal mode (count up) */
  TIFR0 = (1 << TOV0);             /* clear overflow flag */
  TIMSK0 = (1 << TOIE0);           /* enable overflow interrupt */

  /* set up Timer1 (time ticks) */
  TCCR1A = 0;                      /* CTC mode */
  TIFR1 = (1 << OCF1A) | (1 << OCF1B);  /* clear output compare A & B match flag */
  TIMSK1 = (1 << OCIE1B);          /* enable output compare B match interrupt */
  OCR1B = TOP;                     /* set top value for time tick */
  OCR1A = TOP;                     /* same for CTC */

  /* set up T0 as input (just in case) */
  COUNTER_DDR &= ~(1 << COUNTER_IN);    /* set to input mode */
  wait500us();                          /* settle time */

  /* set start values */
  Events = 0;                      /* make compiler happy */
  EventsTrigger = DEFAULT_EVENTS;  /* set default value */
  TimeTrigger = DEFAULT_TIME;      /* set default value */
  Step = 0;                        /* make compiler happy */
  CounterMode = MODE_COUNT;        /* default mode: count */
  Item = UI_COUNTERMODE;           /* select start item */
  Flag = RUN_FLAG | IDLE_FLAG;     /* set control flags */
  /* display everything at startup */
  Show = SHOW_MODE | SHOW_EVENTS | SHOW_TIME | SHOW_STARTSTOP;

  /*
   *  processing loop
   */

  while (Flag > 0)
  {
    /*
     *  start counting
     */

    if (Flag & START_COUNTING)
    {
      /* reset counters */
      Pulses = 0;                  /* pulse counter (ISR) */
      Events = 0;                  /* total value for events */
      TimeTicks = 0;               /* counter for ticks */
      TimeCounter = 0;             /* total value for time (s) */
      TCNT0 = 0;                   /* Timer0: reset event/pulse counter */
      TCNT1 = 0;                   /* Timer1: reset time counter */      

      /* start counters */
      /* start Timer1: prescaler 1:256, CTC mode */
      TCCR1B = (1 << CS12) | (1 << WGM12);
      /* start Timer0: clock source T0 on rising edge */
      TCCR0B = (1 << CS02) | (1 << CS01) | (1 << CS00);

#ifdef EVENT_COUNTER_TRIGGER_OUT
      /* set trigger output high */
      R_PORT = (1 << R_RL_2);      /* pull up probe #2 via Rl */
#endif

      Flag &= ~(START_COUNTING | IDLE_FLAG);      /* clear flags */

      /* update display of events and time (clear trigger values) */
      Show |= SHOW_EVENTS | SHOW_TIME;
    }

    /*
     *  manage counting
     *  - run for each time tick (Timer1)
     */

    if (Flag & MANAGE_COUNTING)
    {
      /* time counters are managed by ISR */

      /* events: get current value */
      Events = Pulses;                  /* get pulses */
      Events += TCNT0;                  /* add counter */

      /* prevent overflow */
      if ((TimeCounter >= MAX_TIME) || (Events >= MAX_EVENTS))
        /* reached maximum */
        Flag |= STOP_COUNTING;          /* stop counting */

      /* manage trigger */
      if (CounterMode == MODE_TIME)          /* time mode */
      {
        if (TimeCounter >= TimeTrigger)      /* limit exeeded */
          Flag |= STOP_COUNTING;        /* stop counting */
      }
      else if (CounterMode == MODE_EVENTS)   /* events mode */
      {
        if (Events >= EventsTrigger)    /* limit exeeded */
          Flag |= STOP_COUNTING;        /* stop counting */
      }

      Flag &= ~MANAGE_COUNTING;         /* clear flag */

      /* each second */
      if (TimeTicks == 0)               /* full second */
        Show |= SHOW_EVENTS | SHOW_TIME;     /* show events and time */
    }

    /*
     *  stop counting (part 1)
     */

    if (Flag & STOP_COUNTING)
    {
      /* stop counters */
      TCCR1B = 0;                  /* disable Timer1 */
      TCCR0B = 0;                  /* disable Timer0 */

#ifdef EVENT_COUNTER_TRIGGER_OUT
      /* set trigger output low */
      R_PORT = 0;                  /* pull down probe #2 via Rl */
#endif

      /* flags are reset later on to allow output of results */

      /* display current values for events and time */
      Show |= SHOW_EVENTS | SHOW_TIME | SHOW_STARTSTOP;
    }

    /*
     *  display counter mode
     */

    if (Show & SHOW_MODE)          /* display mode */
    {
      switch (CounterMode)              /* get mode specifics */
      {
        case MODE_COUNT:                /* count time and events */
          String = (unsigned char *)Count_str;
          break;

        case MODE_TIME:                 /* given time period */
          String = (unsigned char *)Time_str;
          break;

        case MODE_EVENTS:               /* given number of events */
          String = (unsigned char *)Events_str;
      }

      /* display mode (in line #2) */
      LCD_ClearLine2();                      /* clear line #2 */
      MarkItem(UI_COUNTERMODE, Item);        /* mark item if selected */
      Display_EEString(String);              /* display mode name */

      Show |= SHOW_TIME | SHOW_EVENTS;       /* update display of trigger values */
    }

    /*
     *  display events
     */

    if (Show & SHOW_EVENTS)        /* display events */
    {
      LCD_ClearLine3();                 /* clear line #3 */
      MarkItem(UI_EVENTS, Item);        /* mark item if selected */
      Display_Char('n');                /* display: n */
      Display_Space();

      if (Flag & IDLE_FLAG)        /* not counting */
      {
        if (CounterMode == MODE_EVENTS)      /* events mode */
          /* display trigger value */
          Display_FullValue(EventsTrigger, 0, 0);
      }
      else                         /* counting */
        /* display events counter */
        Display_FullValue(Events, 0, 0);
    }

    /*
     *  display time
     */

    if (Show & SHOW_TIME)          /* display time */
    {
      LCD_ClearLine(4);                      /* clear line #4 */
      LCD_CharPos(1, 4);                     /* go to start of line #4 */
      MarkItem(UI_TIME, Item);               /* mark item if selected */
      Display_Char('t');                     /* display: t */
      Display_Space();

      if (Flag & IDLE_FLAG)        /* not counting */
      {
        if (CounterMode == MODE_TIME)        /* time mode */
          /* display trigger value */
          Display_FullValue(TimeTrigger, 0, 's');
      }
      else                         /* counting */
        /* display time elapsed */
        Display_FullValue(TimeCounter, 0, 's');
    }

    /*
     *  stop counting (part 2)
     */

    if (Flag & STOP_COUNTING)
    {
      /* reset flags */
      Flag &= ~STOP_COUNTING;           /* clear flag */
      Flag |= IDLE_FLAG;                /* set idle flag */
    }

    /*
     *  display start/stop
     */

    if (Show & SHOW_STARTSTOP)     /* display start/stop */
    {
      if (Flag & IDLE_FLAG)        /* display: start */
        String = (unsigned char *)Start_str;
      else                         /* display: stop */
        String = (unsigned char *)Stop_str;

      LCD_ClearLine(5);                      /* clear line #5 */
      LCD_CharPos(1, 5);                     /* go to start of line #5 */
      MarkItem(UI_STARTSTOP, Item);          /* mark item if selected */
      Display_EEString(String);              /* display start/stop */
    }

    /* smooth UI after long key press */
    if (Flag & DELAY_FLAG)
    {
      SmoothLongKeyPress();        /* delay next key press */

      Flag &= ~DELAY_FLAG;         /* clear flag */
    }

    /* update display control flag */
    if (Flag & IDLE_FLAG)               /* not counting */
      /* set display control flag based on currently selected item */
      Show = UI_Index[Item - 1];
    else                                /* counting */
      /* reset display control */
      Show = 0;

    /*
     *  wait for user feedback or Timer1 (via OP_BREAK_KEY)
     */

    Flag |= WAIT_FLAG;             /* enter waiting loop */

    while (Flag & WAIT_FLAG)
    {
      /* wait for user feedback */
      Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);

      /* consider rotary encoder's turning velocity (1-7) */
      Temp = UI.KeyStep;                /* get velocity */
      Step = Temp;
      if (Temp > 1)                     /* larger step */
      {
        /* increase step size based on turning velocity */
        if (Item == UI_TIME)            /* time */
        {
          /* 16 bit value */
          if (Temp <= 3)
            Step = 10;
          else if (Temp <= 5)
            Step = 120;
          else
            Step = 1800;
        }
        else if (Item == UI_EVENTS)     /* events */
        {
          /* 32 bit value - simply too large */
          if (Temp <= 3)
            Step = 10;
          else if (Temp <= 5) Step = 100;
          else Step = 1000;
        }
      }

      if (Test == KEY_TIMEOUT)          /* timeout by OP_BREAK_KEY */
      {
        Flag |= MANAGE_COUNTING;        /* manage counting */
        Flag &= ~WAIT_FLAG;             /* end waiting loop */
      }
      else if (Test == KEY_SHORT)       /* short key press */
      {
        /* switch to next item */
        if (Flag & IDLE_FLAG)           /* when not counting */
        {
          if (Item < UI_STARTSTOP)      /* not last item */
          {
            /* go to next item */
            Item++;                     /* next one */

            /* special rules */
            if (CounterMode == MODE_COUNT)        /* counter mode */
            {
              /* skip events and time */
              if (Item < UI_STARTSTOP)
                Item = UI_STARTSTOP;
            }
            else if (CounterMode == MODE_EVENTS)  /* events mode */
            {
              /* skip time */
              if (Item == UI_TIME)
                Item = UI_STARTSTOP;
            }
            else                                  /* time mode */
            {
              /* skip events */
              if (Item == UI_EVENTS)
                Item = UI_TIME;
            }
          }
          else                          /* last item */
          {
            /* go to first item */
            Item = UI_COUNTERMODE;
          }

          /* update display flags (old and new item) */
          Test = UI_Index[Item - 1];    /* get new display flag */
          Show |= Test;                 /* add new display flag */

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
      }
      else if (Test == KEY_LONG)        /* long key press */
      {
        if (Item == UI_STARTSTOP)       /* start/stop selected */
        {
          if (Flag & IDLE_FLAG)         /* not counting */
            /* start counting */
            Flag |= START_COUNTING | DELAY_FLAG;
          else                          /* counting */
            /* stop counting */
            Flag |= STOP_COUNTING | DELAY_FLAG;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_EVENTS)     /* events selected */
        {
          /* reset to default value */
          EventsTrigger = DEFAULT_EVENTS;

          Flag |= DELAY_FLAG;           /* set delay flag */
          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_TIME)       /* time selected */
        {
          /* reset to default value */
          TimeTrigger = DEFAULT_TIME;

          Flag |= DELAY_FLAG;           /* set delay flag */
          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
      }
      else if (Test == KEY_TWICE)       /* two short key presses */
        /* exit tool */
        Flag = 0;                       /* end processing loop */
      else if (Test == KEY_RIGHT)       /* right key */
      {
        if (Item == UI_COUNTERMODE)     /* counter mode selected */
        {
          /* change to next mode */
          CounterMode++;
          /* overrun to first mode */
          if (CounterMode > MODE_EVENTS)
            CounterMode = MODE_COUNT;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_EVENTS)     /* events selected */
        {
          /* increase event trigger */
          EventsTrigger += Step;
          /* limit overflow to max. value */
          if (EventsTrigger > MAX_EVENTS) EventsTrigger = MAX_EVENTS;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_TIME)       /* time selected */
        {
          /* increase time trigger */
          TimeTrigger += Step;
          /* limit overflow to max. value */
          if (TimeTrigger > MAX_TIME)
            TimeTrigger = MAX_TIME;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
      }
      else if (Test == KEY_LEFT)        /* left key */
      {
        if (Item == UI_COUNTERMODE)     /* counter mode selected */
        {
          /* change to previous mode */
          CounterMode--;
          /* underrun to last mode */
          if (CounterMode == 0)
            CounterMode = MODE_EVENTS;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_EVENTS)     /* events selected */
        {
          /* decrease event trigger */
          EventsTrigger -= Step;
          /* limit underflow to zero */
          if (EventsTrigger > MAX_EVENTS)
            EventsTrigger = 0;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
        else if (Item == UI_TIME)       /* time selected */
        {
          /* decrease time trigger */
          TimeTrigger -= Step;
          /* limit underflow to zero */
          if (TimeTrigger > MAX_TIME)
            TimeTrigger = 0;

          Flag &= ~WAIT_FLAG;           /* end waiting loop */
        }
      }
    }
  }


  /*
   *  clean up
   */

  /* timers */
  TIMSK0 = 0;                 /* disable all interrupts for Timer0 */
  TIMSK1 = 0;                 /* disable all interrupts for Timer1 */

  /* local constant for timer1 */
  #undef TOP

  /* local constants for Flag */
  #undef RUN_FLAG
  #undef WAIT_FLAG
  #undef IDLE_FLAG
  #undef DELAY_FLAG
  #undef START_COUNTING
  #undef MANAGE_COUNTING
  #undef STOP_COUNTING

  /* local constants for CounterMode */
  #undef MODE_COUNT
  #undef MODE_TIME
  #undef MODE_EVENTS

  /* local constants for Item */
  #undef UI_COUNTERMODE
  #undef UI_EVENTS
  #undef UI_TIME
  #undef UI_STARTSTOP

  /* local constants for Show */
  #undef SHOW_MODE
  #undef SHOW_EVENTS
  #undef SHOW_TIME
  #undef SHOW_STARTSTOP

  /* local constants for defaults and maximums */
  #undef DEFAULT_TIME
  #undef DEFAULT_EVENTS
  #undef MAX_TIME
  #undef MAX_EVENTS
}

#endif // HW_EVENT_COUNTER
