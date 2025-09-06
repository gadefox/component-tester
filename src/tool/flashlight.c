/* ************************************************************************
 *
 *   flashlight tool (hardware and software options)
 *
 *   (c) 2012-2025 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#ifdef HW_FLASHLIGHT

/*
 *  local constants
 */


/*
 *  local variabless
 */

/* ************************************************************************
 *   flashlight / general purpose switched output
 * ************************************************************************ */

/*
 *  flashlight / general purpose switched output
 *  - toggles output pin between high and low
 */

void Flashlight(void)
{
  uint8_t           Flag;          /* status flag */

  /*
   *  toggle output
   */

  /* get current state */
  Flag = FLASHLIGHT_PORT;          /* read port register */
  Flag &= (1 << FLASHLIGHT_CTRL);  /* filter pin */

  /* set output based on current state */
  if (Flag)                   /* pin high */
    /* toggle off */
    FLASHLIGHT_PORT &= ~(1 << FLASHLIGHT_CTRL);   /* set pin low */
  else                        /* pin low */
    /* toggle on */
    FLASHLIGHT_PORT |= (1 << FLASHLIGHT_CTRL);    /* set pin high */
}


#endif // HW_FLASHLIGHT
