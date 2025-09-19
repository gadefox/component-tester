/* ************************************************************************
 *
 *   cap functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef CAP_H
#define CAP_H


#if defined(SW_ESR) && defined(SW_OLD_ESR)
#error <<< select either ESR or OLD_ESR! >>>
#endif


/* number of entries in data tables */
#define NUM_LARGE_CAP         46        /* large cap factors */


#define NUM_SMALL_CAP         9         /* small cap factors */
/* multiplicator table IDs */
#define TABLE_SMALL_CAP       1              /* table for small caps */
#define TABLE_LARGE_CAP       2              /* table for large caps */


/* capacitor */
typedef struct
{
  uint8_t           A;             /* probe pin #1 */
  uint8_t           B;             /* probe pin #2 */
  int8_t            Scale;         /* exponent of factor (value * 10^x) */
  unsigned long     Value;         /* capacitance incl. zero offset */
  unsigned long     Raw;           /* capacitance excl. zero offset */
  uint16_t          I_leak_Value;  /* leakage current: value (in A) */
  int8_t            I_leak_Scale;  /* leakage current: exponent (10^x) */
#ifdef SW_C_VLOSS
  uint16_t          U_loss;        /* voltage loss (in 0.1%) */
#endif
} Capacitor_Type;


#if defined (SW_ESR) || defined (SW_OLD_ESR)
extern uint16_t MeasureESR(Capacitor_Type *Cap);
#endif

extern void MeasureCap(uint8_t Probe1, uint8_t Probe2, uint8_t ID);

#ifdef HW_ADJUST_CAP
extern uint8_t RefCap(void);
#endif

#ifdef SW_C_VLOSS
extern const unsigned char U_loss_str[];
#endif

/* voltage based factors for large caps (using Rl) */
extern const uint16_t LargeCap_table[];

/* voltage based factors for small caps (using Rh) */
extern const uint16_t SmallCap_table[];

extern Capacitor_Type  Caps[];             /* capacitors */


#endif // CAP_H
