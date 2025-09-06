/* ************************************************************************
 *
 *   probe tools functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef HW_LOGIC_PROBE

#ifndef PROBE_H
#define PROBE_H

#ifndef HW_KEYS
#error <<< Logic probe tool requires keys >>>
#endif


/* ************************************************************************
 *   constants for operation and UI
 * ************************************************************************ */

extern void LogicProbe(void);

/* logic families and voltages */
extern const uint16_t Logic_Vcc_table[];

extern unsigned char TTL_str[];
extern const unsigned char CMOS_str[];

extern const uint16_t Logic_Vcc_table[NUM_LOGIC_TYPES];

extern const unsigned char LogicProbe_str[];


#endif // PROBE_H

#endif // HW_LOGIC_PROBE
