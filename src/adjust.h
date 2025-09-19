/* ************************************************************************
 *
 *   adjust functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef ADJUST_H
#define ADJUST_H


/* manage RZero */
#ifdef R_MULTIOFFSET
  #define NV_R_ZERO       {R_ZERO, R_ZERO, R_ZERO}
#else
  #define NV_R_ZERO       R_ZERO
#endif

/* manage CapZero */
#ifdef CAP_MULTIOFFSET
#define NV_C_ZERO         {C_ZERO, C_ZERO, C_ZERO}
#else
#define NV_C_ZERO         C_ZERO
#endif // CAP_MULTIOFFSET


/* storage modes (ID and bitfield) */
#define STORAGE_LOAD          0b00000001     /* load adjustment values (ID) */
#define STORAGE_SAVE          0b00000010     /* save adjustment values (ID) */
#define STORAGE_SHORT         0b00000100     /* short menu (flag) */ 


/* basic adjustment offsets and values (stored in EEPROM) */
typedef struct
{
  uint16_t          RiL;           /* internal pin resistance of MCU in low mode (0.1 Ohms) */
  uint16_t          RiH;           /* internal pin resistance of MCU in high mode (0.1 Ohms) */

  #ifdef R_MULTIOFFSET
    uint16_t        RZero[3];      /* resistance of probe leads (2 in series) (0.01 Ohms) */
  #else
    uint16_t        RZero;         /* resistance of probe leads (2 in series) (0.01 Ohms) */
  #endif

  #ifdef CAP_MULTIOFFSET
    uint8_t         CapZero[3];    /* capacitance zero offsets (PCB+leads) (pF) */
  #else
    uint8_t         CapZero;       /* capacitance zero offset (PCP+leads) (pF) */
  #endif

  int8_t            RefOffset;     /* voltage offset of bandgap reference (mV) */
  int8_t            CompOffset;    /* voltage offset of analog comparator (mV) */
  uint8_t           Contrast;      /* contrast value of display */
  uint8_t           CheckSum;      /* checksum for stored values */
} Adjust_Type;


#if defined (CAP_MULTIOFFSET) || defined (R_MULTIOFFSET)
extern uint8_t GetOffsetIndex(uint8_t Probe1, uint8_t Probe2);
#endif

extern void SetAdjustmentDefaults(void);
extern void ManageAdjustmentStorage(uint8_t Mode, uint8_t ID);

extern void ShowAdjustmentValues(void);
extern uint8_t SelfAdjustment(void);

#ifdef SW_SELFTEST
extern uint8_t SelfTest(void);
#endif

extern Adjust_Type     NV;                 /* basic adjustment offsets and values */

/* basic adjustment values: profile #1 */
extern const Adjust_Type    NV_Adjust_1;

/* basic adjustment values: profile #2 */
extern const Adjust_Type    NV_Adjust_2;

#ifdef UI_THREE_PROFILES
/* basic adjustment values: profile #3 */
extern const Adjust_Type  NV_Adjust_3;
#endif // UI_THREE_PROFILES

extern const unsigned char URef_str[];
extern const unsigned char RhLow_str[];
extern const unsigned char RhHigh_str[];
extern const unsigned char RiLow_str[];
extern const unsigned char RiHigh_str[];
extern const unsigned char Rl_str[];
extern const unsigned char Rh_str[];
extern const unsigned char ProbeComb_str[];
extern const unsigned char CapOffset_str[];
extern const unsigned char ROffset_str[];
extern const unsigned char Vcc_str[];
extern const unsigned char CompOffset_str[];


#endif // ADJUST_H
