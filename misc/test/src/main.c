#include <avr/io.h>

#include "wait.h"


void DelayTimer(void)
{
  TCNT0 = 0;                       /* reset counter to 0 */
  TCCR0B = (1 << CS00);            /* start timer by setting prescaler */

  while (!(TIFR0 & (1 << OCF0A)));   /* wait for output compare A match flag */

  TCCR0B = 0;                      /* stop timer */
  TIFR0 = (1 << OCF0A);            /* clear flag */
}

void dummy(void)
{
}

void HandS(void)
{
  ADCSRA = 17;                 /* start conversion with next ADC clock cycle */
  wait10us();                    /* fixed pre-delay */
  DelayTimer();
  PORTC = 34;          /* pull up probe-2 via Rl */
  DDRC = 51;           /* enable resistor */
  wait2us();                     /* first half-pulse */
  /* S/H happens here */
  dummy();
}

void timer1_init(void)
{
  TCCR1A = 0;
  TCCR1B = (1<<CS10);   // prescaler = 1 → Timer1 beží na F_CPU
}

uint16_t cycles(void)
{
  uint16_t start, stop;

  timer1_init();

  start = TCNT1;
  wait1us();
  stop = TCNT1;

  return stop - start;   // počet cyklov
}

int main(void)
{
  uint16_t elapsed = cycles();

  return 0;
}

