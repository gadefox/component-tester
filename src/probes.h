/* ************************************************************************
 *
 *   probes functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef PROBES_H
#define PROBES_H


/* number of entries in data tables */
#define NUM_E6                6         /* E6 norm values */
#define NUM_E12              12         /* E12 norm values */
#define NUM_E24              24         /* E24 norm values */
#define NUM_E96              96         /* E24 norm values */


/* E series */
#define E6                    6         /* E6 */
#define E12                  12         /* E12 */
#define E24                  24         /* E24 */
#define E48                  48         /* E48 */
#define E96                  96         /* E96 */
#define E192                192         /* E192 */

/* probe IDs */
#define PROBE_1               0    /* probe #1 */
#define PROBE_2               1    /* probe #2 */
#define PROBE_3               2    /* probe #3 */

/* bit flags for PullProbe() (bitfield) */
#define PULL_DOWN             0b00000000     /* pull down */
#define PULL_UP               0b00000001     /* pull up */
#define PULL_1MS              0b00001000     /* pull for 1ms */
#define PULL_10MS             0b00010000     /* pull for 10ms */


/* probes */
typedef struct
{
  /* probe IDs */
  uint8_t           ID_1;          /* probe-1 */
  uint8_t           ID_2;          /* probe-2 */
  uint8_t           ID_3;          /* probe-3 */

  /* backup probe IDs */
  uint8_t           ID2_1;         /* probe-1 */
  uint8_t           ID2_2;         /* probe-2 */
  uint8_t           ID2_3;         /* probe-3 */

  /* register bits for switching probes and test resistors */
  uint8_t           Rl_1;          /* Rl mask for probe-1 */
  uint8_t           Rl_2;          /* Rl mask for probe-2 */
  uint8_t           Rl_3;          /* Rl mask for probe-3 */
  uint8_t           Rh_1;          /* Rh mask for probe-1 */
  uint8_t           Rh_2;          /* Rh mask for probe-2 */
  uint8_t           Rh_3;          /* Rh mask for probe-3 */
  uint8_t           Pin_1;         /* pin mask for probe-1 */
  uint8_t           Pin_2;         /* pin mask for probe-2 */
  uint8_t           Pin_3;         /* pin mask for probe-3 */
  uint8_t           Ch_1;          /* ADC MUX input channel for probe-1 */
  uint8_t           Ch_2;          /* ADC MUX input channel for probe-2 */
  uint8_t           Ch_3;          /* ADC MUX input channel for probe-3 */
} Probe_Type;


/*
 *  tables of register bits for probe settings
 *  - they save some bytes in the firmware
 */

/* register bits for Rl probe resistors based on probe ID */
extern const uint8_t Rl_table[];

/* register bits for Rh probe resistors based on probe ID */
extern const uint8_t Rh_table[];

/* register bits for ADC port pins based on probe ID */
extern const uint8_t Pin_table[];

/* register bits for ADC MUX input channels based on probe ID (ADC0-7 only) */
extern const uint8_t Channel_table[];

#ifdef SW_E6
/* E6 (in 0.01) */
extern const uint16_t E6_table[];
#endif

#ifdef SW_E12
/* E12 (in 0.01) */
extern const uint16_t E12_table[];
#endif

#ifdef SW_E24
/* E24 (in 0.01) */
extern const uint16_t E24_table[];
#endif

#ifdef SW_E96
/* E96 (in 0.01) */
extern const uint16_t E96_table[];
#endif


extern void UpdateProbes(uint8_t Probe1, uint8_t Probe2, uint8_t Probe3);
extern void UpdateProbes2(uint8_t Probe1, uint8_t Probe2);
extern void RestoreProbes(void);
extern void BackupProbes(void);
extern uint8_t ShortedProbes(void);

#if defined (SW_ESR) || defined (SW_OLD_ESR)
extern void DischargeCap(uint8_t Probe1, uint8_t Probe2);
#endif

extern void DischargeProbes(void);
extern void PullProbe(uint8_t Probe, uint8_t Mode);

extern uint16_t GetFactor(uint16_t U_in, uint8_t ID);

#if defined (FUNC_EVALUE) || defined (FUNC_COLORCODE) || defined (FUNC_EIA96)
extern uint8_t GetENormValue(uint32_t Value, int8_t Scale, uint8_t E_Series, uint8_t Tolerance);
#endif

extern void CheckProbes(uint8_t Probe1, uint8_t Probe2, uint8_t Probe3);
extern void CheckAlternatives(void);


/* probing */
extern Probe_Type      Probes;             /* test probes */


#endif // PROBES_H
