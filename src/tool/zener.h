/* ************************************************************************
 *
 *   zener tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef ZENER_H
#define ZENER_H


/* Zener check: can't have unswitched and switched mode */
#if defined (ZENER_UNSWITCHED) && defined (ZENER_SWITCHED)
#error <<< Zener check: select either unswitched or switched mode! >>>
#endif

/* Zener check, switched mode: boost converter drive methods */
#ifdef ZENER_SWITCHED
/* can't have high and low active */
#if defined (ZENER_BOOST_HIGH) && defined (ZENER_BOOST_LOW)
#error <<< Zener check: select either high or low active for boost converter! >>>
#endif

/* must have one drive method */
#if ! defined (ZENER_BOOST_HIGH) && ! defined (ZENER_BOOST_LOW)
#error <<< Zener check: select drive method for boost converter! >>>
#endif
#endif

#ifdef UI_ZENER_DIODE
#ifndef SW_SYMBOLS
#error <<< zener: no symbols specified! >>>
#endif

#undef SYMBOLS_EXTRA
#endif // UI_ZENER_DIORE

#if ! defined (ZENER_UNSWITCHED) && ! defined (ZENER_SWITCHED)
#undef HW_PROBE_ZENER
#endif

/* ************************************************************************
 *   constants for operation and UI
 * ************************************************************************ */

#ifdef HW_ZENER

extern void Zener_Tool(void);


#ifndef ZENER_UNSWITCHED
extern const unsigned char Min_str[];
#endif

#ifdef HW_PROBE_ZENER
extern const unsigned char Zener_str[];
#endif

#endif // HW_ZENER


#ifdef HW_PROBE_ZENER

extern void CheckZener(void);

#endif // HW_PROBE_ZENER


#endif // ZENER_H
