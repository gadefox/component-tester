/* ************************************************************************
 *
 *   resistor functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef RESISTOR_H
#define RESISTOR_H


#if defined(SW_R_E24_5_CC) || defined(SW_R_E24_1_CC) || defined(SW_R_E96_CC)
#ifndef LCD_GRAPHIC
#error <<< E24 and E96 feature requires graphic display >>>
#endif

#ifndef LCD_COLOR
#error <<< E24 and E96 feature requires color display >>>
#endif
#endif // SW_R_E24_5_CC || SW_R_E24_1_CC || SW_R_E96_CC


/*
 *  norm values
 */

/* option: E6 norm values */
#if defined (SW_C_E6_T) || defined (SW_L_E6_T)
#define SW_E6
#endif

/* option: E12 norm values */
#if defined (SW_C_E12_T) || defined (SW_L_E12_T)
#define SW_E12
#endif

/* option: E24 norm values */
#if defined (SW_R_E24_5_T) || defined (SW_R_E24_5_CC) || defined (SW_R_E24_1_T) || defined (SW_R_E24_1_CC)
#define SW_E24
#endif

/* option: E96 norm values */
#if defined (SW_R_E96_T) || defined (SW_R_E96_CC) || defined (SW_R_E96_EIA96)
#define SW_E96
#endif

/* functions: Show_ENormValues(), Display_EValue() */
#if defined (SW_R_E24_5_T) || defined (SW_R_E24_1_T) || defined (SW_R_E96_T)
#define FUNC_EVALUE
#define SW_R_EXX
#endif

#if defined (SW_C_E6_T) || defined (SW_C_E12_T) || defined (SW_L_E6_T) || defined (SW_L_E12_T)
#define FUNC_EVALUE
#endif

/* functions: Show_ENormCodes(), Display_ColorCode() */
#if defined (SW_R_E24_5_CC) || defined (SW_R_E24_1_CC) || defined (SW_R_E96_CC)
#define FUNC_COLORCODE
#define SW_R_EXX
#endif

/* functions: Show_ENormEIA96(), Display_EIA96() */
#ifdef SW_R_E96_EIA96
#define FUNC_EIA96
#define SW_R_EXX
#endif


/* resistor */
typedef struct
{
  uint8_t           A;             /* probe pin #1 */
  uint8_t           B;             /* probe pin #2 */
  int8_t            Scale;         /* exponent of factor (value * 10^x) */
  unsigned long     Value;         /* resistance */
} Resistor_Type;


extern uint16_t SmallResistor(uint8_t ZeroFlag);
extern void CheckResistor(void);
extern uint8_t CheckSingleResistor(uint8_t HighPin, uint8_t LowPin, uint8_t Max);


extern Resistor_Type   Resistors[];        /* resistors */


#endif // RESISTOR_H
