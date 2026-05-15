/* ************************************************************************
 *
 *   ring tester tool (hardware and software options)
 *
 *   (c) 2012-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef HW_RING_TESTER

/*
 *  local constants
 */


/*
 *  local variables
 */


/* ************************************************************************
 *   ring tester for high Q chokes and transformers
 * ************************************************************************ */

/*
 *  ring tester (LOPT/FBT tester)
 *  - uses frontend for ring detection
 *  - counter input: T0
 *  - control via probes (RING_TESTER_PROBES)
 *    probe #1: +5V
 *    probe #2: pulse output
 *    probe #3: Gnd
 *  - control via dedicated pin (RING_TESTER_PIN)
 *    RINGTESTER_OUT: pulse output
 *  - requires idle sleep mode to keep timers running when MCU is sleeping
 */

void RingTester(void)
{
  /* local constants for Mode */
#define EXIT_STATE     0         /* end loop */
#define IDLE_STATE     1         /* enter/run waiting loop */
#define GATE_STATE     2         /* gatetime flag */
#define SHOW_RINGS     3         /* display rings */

  uint8_t           State = IDLE_STATE; /* loop control flag */
  uint8_t           Test;               /* user feedback */
  uint8_t           Old_DDR;            /* old DDR state */

  /* show info */
  LCD_Clear();                          /* clear display */

#ifdef UI_COLORED_TITLES
  /* display: Ring Tester */
  Display_ColoredEEString(RingTester_str, COLOR_TITLE);
#else
  Display_EEString(RingTester_str);   /* display: Ring Tester */
#endif

#ifdef RING_TESTER_PROBES
  ProbePinout(PROBES_RINGTESTER);       /* show probes used */
#endif

  /*
   *  working priciple:
   *  - a shorted or bad inductor has a low Q value
   *  - a trigger pulse causes a damped oscillation in a tank circuit
   *  - the higher the inductor's Q the longer the oscillation lasts
   *  - so we simply count the number of rings until the oscillation
   *    fades away
   */

  /*
   *  set up pulse output
   */

#ifdef RING_TESTER_PIN
  /* use dedicated pin */
  RINGTESTER_PORT &= ~(1 << RINGTESTER_OUT);      /* low by default */
  RINGTESTER_DDR |= (1 << RINGTESTER_OUT);        /* enable output */
#endif

#ifdef RING_TESTER_PROBES
  /* set probes: probe #1 - Vcc / probe #2 - Rl - pulse out / probe #3 - Gnd */
  R_PORT = 0;                                /* pull down probe #2 */
  R_DDR = (1 << R_RL_2);                     /* enable Rl for probe #2 */
  ADC_PORT = (1 << TP1);                     /* pull up #1, pull down #3 */
  ADC_DDR = (1 << TP1) | (1 << TP3);         /* enable direct pull for #1 and #3 */
#endif

  /*
   *  We use Timer1 for the gate time and Timer0 to count rings.
   *  Max. frequency for Timer0 is 1/4 of the MCU clock.
   */

  /*
      Fixed gate time of 10ms.

      Timer1 top value (gate time)
      - top = gatetime * MCU_cycles / prescaler 
      - gate time in µs
      - MCU cycles per µs
      - top max. 2^16 - 1

      Using prescaler 8:1 - register bits: (1 << CS11)
   */

  /* set up Timer0 (ring counter) */
  TCCR0A = 0;                      /* normal mode (count up) */
  TIFR0 = (1 << TOV0);             /* clear overflow flag */
  TIMSK0 = (1 << TOIE0);           /* enable overflow interrupt */

  /* set up Timer1 (gate time) */
  TCCR1A = 0;                      /* normal mode (count up) */
  TIFR1 = (1 << OCF1A);            /* clear output compare A match flag */
  TIMSK1 = (1 << OCIE1A);          /* enable output compare A match interrupt */
  /* top = gatetime * MCU_cycles / timer prescaler */
  OCR1A = (uint16_t)((10000UL * MCU_CYCLES_PER_US) / 8);

  /*
   *  measurement loop
   */

  while (State != EXIT_STATE)
  {
    /* set up T0 as input (pin might be shared with display) */
    Old_DDR = COUNTER_DDR;              /* save current settings */
    COUNTER_DDR &= ~(1 << COUNTER_IN);  /* signal input */
    wait500us();                        /* settle time */

    /* start timers */
    Pulses = 0;                         /* reset pulse counter */
    TCNT0 = 0;                          /* Timer0: reset ring counter */
    TCNT1 = 0;                          /* Timer1: reset gate time counter */
    TCCR1B = (1 << CS11);               /* start Timer1: prescaler 8:1 */
    TCCR0B = (1 << CS02) | (1 << CS01); /* start Timer0: clock source T0 on falling edge */

    /*
     *  create trigger pulse (2 ms)
     *  - will create one pseudo ring since timers are already running
     */

#ifdef RING_TESTER_PIN
    RINGTESTER_PORT |= (1 << RINGTESTER_OUT);     /* set pin high */
    wait2ms();                                    /* wait 2 ms */
    RINGTESTER_PORT &= ~(1 << RINGTESTER_OUT);    /* set pin low */
#endif

#ifdef RING_TESTER_PROBES
    R_PORT = (1 << R_RL_2);             /* pull up probe #2 via Rl */
    wait2ms();                          /* wait 2 ms */
    R_PORT = 0;                         /* pull down probe #2 */
#endif

    /* wait for timer1 or key press */
    while (State == IDLE_STATE)
    {
      if (TCCR1B == 0)                  /* Timer1 stopped by ISR */
        State = GATE_STATE;             /* end loop and signal Timer1 event */
      else                              /* Timer1 still running */
      {
        /* wait for user feedback */
        Test = TestKey(0, CHECK_KEY_TWICE | CHECK_BAT);

        if (Test == KEY_TWICE)          /* two short key presses */
          State = EXIT_STATE;           /* end processing loop */
      }
    }

    /* T0 pin might be shared with display */
    COUNTER_DDR = Old_DDR;              /* restore old settings */

    Cfg.OP_Control &= ~OP_BREAK_KEY;    /* clear break signal (just in case) */

    /*
     *  process measurement
     */

    if (State == GATE_STATE)            /* got measurement */
    {
      /* total sum of rings during gate period */
      Pulses += TCNT0;                  /* add counter of Timer0 */

      /* consider first/pseudo ring created by trigger pulse */
      if (Pulses > 0)                   /* sanity check */
        Pulses--;                       /* subtract one ring */

      State = SHOW_RINGS;               /* display rings */
    }

    /*
     *  display number of rings (in line #2)
     */

    LCD_ClearLine2();                   /* clear line #2 */
    Display_Char('n');                  /* display: n */
    Display_Space();

    if (State == SHOW_RINGS)             /* valid number of rings */
    {
      Display_Value2(Pulses);           /* display rings */
      State = IDLE_STATE;               /* clear flag */
    }
    else                                /* invalid number of rings */
      Display_Minus();                  /* display: no value */

    /*
     *  add some delay to slow down the update rate
     *  and to smooth the UI
     */

    /* check test button using a timeout of 400 ms */
    Test = TestKey(400, CHECK_KEY_TWICE | CHECK_BAT);

    /* catch double press for exit */
    if (Test == KEY_TWICE)              /* two short key presses */
      State = EXIT_STATE;               /* end processing loop */
  }

  /*
   *  clean up
   */

  TIMSK0 = 0;                 /* disable all interrupts for Timer0 */
  TIMSK1 = 0;                 /* disable all interrupts for Timer1 */

  /* local constants for Flag */
  #undef EXIT
  #undef IDLE
  #undef GATE
  #undef SHOW_RINGS
}

#endif // HW_RING_TESTER
