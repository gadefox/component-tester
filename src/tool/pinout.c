/* ************************************************************************
 *
 *   probe tools (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef FUNC_PROBE_PINOUT

/*
 *  local constants
 */


/*
 *  local variabless
 */


/* ************************************************************************
 *   support functions
 * ************************************************************************ */

/*
 *  display probe pinout
 *
 *  required:
 *  - Mode
 *    PROBES_PWM         PWM signal
 *    PROBES_ESR         ESR measurement
 *    PROBES_RCL         monitoring RCL
 *    PROBES_RINGTESTER  ring tester
 *    PROBES_DIODE       diode
 */

void ProbePinout(uint8_t Mode)
{
  uint8_t           Char1 = 0;     /* designator for probe #1 */
  uint8_t           Char2 = 0;     /* designator for probe #2 */
  uint8_t           Char3 = 0;     /* designator for probe #3 */

  LCD_ClearLine2();                /* info goes to line #2 */

  /*
   *  set probe pinout based on mode
   */

  switch (Mode)
  {
#ifdef SW_PROBEPINOUT_PWM
    case PROBES_PWM:
      /* probe #1: Gnd / probe #2: signal / probe #3: Gnd */
      Char1 = '-';
      Char2 = 's';
      Char3 = '-';
      break;
#endif

#ifdef SW_PROBEPINOUT_ESR
    case PROBES_ESR:
      /* probe #1: + / probe #3: - */
      Char1 = '+';
      Char2 = 0;
      Char3 = '-';
      break;
#endif

#ifdef SW_PROBEPINOUT_RCL
    case PROBES_RCL:
      /* probe #1: * / probe #3: * */
      Char1 = '*';
      Char2 = 0;
      Char3 = '*';
      break;
#endif

#if defined (HW_RING_TESTER) && defined (RING_TESTER_PROBES)
    case PROBES_RINGTESTER:
      /* probe #1: Vcc / probe #2: pulse out / probe #3: Gnd */
      Char1 = '+';
      Char2 = 'p';
      Char3 = '-';
      break;
#endif

#if defined (SW_PHOTODIODE)
    case PROBES_DIODE:
      /* probe #1: Anode / probe #3: Cathode */
      Char1 = 'A';
      Char2 = 0;
      Char3 = 'C';
      break;
#endif
  }

  Show_SimplePinout(Char1, Char2, Char3);    /* display pinout */

  /* wait for any key press or 5s */
  TestKey(5000, CHECK_BAT);
  LCD_ClearLine2();                /* clear line #2 */
}


#endif // FUNC_PROBE_PINOUT
