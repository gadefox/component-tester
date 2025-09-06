/* ************************************************************************
 *
 *   inductor functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_INDUCTOR

#ifndef INDUCTOR_H
#define INDUCTOR_H


#define TABLE_INDUCTOR        3              /* table for inductors */

/* inductor */
typedef struct
{
  int8_t            Scale;         /* exponent of factor (value * 10^x) */
  unsigned long     Value;         /* inductance */  
} Inductor_Type;


/* voltage based factors for inductors */
extern const uint16_t Inductor_table[];


extern uint8_t MeasureInductor(Resistor_Type *Resistor);

// TODO: check HW_LC_METER
extern Inductor_Type Inductor;           /* inductor */


#endif // INDUCTOR_H


#endif // SW_INDUCTOR
