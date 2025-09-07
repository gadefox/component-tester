/* ************************************************************************
 *
 *   common variables
 *
 *   (c) 2012-2025 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

/*
 *  include header files
 */

/* local includes */
#include "common.h"           /* common header file */


/*
 *  local constants
 */

/* configuration */
Config_Type       Cfg;                     /* tester modes, offsets and values */

/* probing */
Check_Type        Check;                   /* checking/testing */

/* configuration */
Config_Type       Cfg;                     /* tester modes, offsets and values */

#ifdef HW_TOUCH
Touch_Type      Touch;                   /* touch screen adjustment offsets */
#endif


/*
 *  NVRAM values with their defaults
 *  - stored in EEPROM
 */

#ifdef HW_TOUCH
/* touch screen adjustment offsets */
const Touch_Type    NV_Touch EEMEM = {0, 0, 0, 0, 0};
#endif

/*
 *  constant tables
 *  - stored in EEPROM/Flash
 */

#ifdef VAR_TIMER1_TABLES
/* Timer1 prescalers and corresponding register bits */
const uint16_t T1_Prescaler_table[NUM_TIMER1] MEM_TYPE = {1, 8, 64, 256, 1024};
const uint8_t T1_RegBits_table[NUM_TIMER1] MEM_TYPE = {(1 << CS10), (1 << CS11), (1 << CS11) | (1 << CS10), (1 << CS12), (1 << CS12) | (1 << CS10)};
#endif
