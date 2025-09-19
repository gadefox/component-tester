/* ************************************************************************
 *
 *   semi functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef SEMI_H
#define SEMI_H


/* FET types, also used for IGBTs (bitfield) */
#define TYPE_N_CHANNEL        0b00000001     /* n channel */
#define TYPE_P_CHANNEL        0b00000010     /* p channel */
#define TYPE_ENHANCEMENT      0b00000100     /* enhancement mode */
#define TYPE_DEPLETION        0b00001000     /* depletion mode */
#define TYPE_MOSFET           0b00010000     /* MOSFET */
#define TYPE_JFET             0b00100000     /* JFET */
#define TYPE_SYMMETRICAL      0b01000000     /* symmetrical drain/source */


/* BJT types (bitfield) */ 
#define TYPE_NPN              0b00000001     /* NPN */
#define TYPE_PNP              0b00000010     /* PNP */
#define TYPE_PARASITIC        0b00000100     /* parasitic BJT */


/* diode types (bitfield) */
#define TYPE_STANDARD         0b00000001     /* standard diode */


/* semiconductor flags (bitfield) */
#define HFE_COMMON_EMITTER    0b00000001     /* hFE: common emitter circuit */
#define HFE_COMMON_COLLECTOR  0b00000010     /* hFE: common collector circuit */
#define HFE_CIRCUIT_MASK      0b00000011     /* mask for hFE circuit flags */


/* flags for semicondutor detection logic (bitfield) */
#define DONE_NONE             0b00000000     /* detected nothing / not sure yet */
#define DONE_SEMI             0b00000001     /* detected semi */
#define DONE_ALTSEMI          0b00000010     /* detected alternative semi */


/* diode */
typedef struct
{
  uint8_t           A;             /* probe pin connected to anode */
  uint8_t           C;             /* probe pin connected to cathode */
  uint16_t          V_f;           /* forward voltage in mV (high current) */
  uint16_t          V_f2;          /* forward voltage in mV (low current) */
} Diode_Type;

/* common semiconductors */
typedef struct
{
  uint8_t           A;             /* probe connected to pin A [0-2] */
  uint8_t           B;             /* probe connected to pin B [0-2] */
  uint8_t           C;             /* probe connected to pin C [0-2] */
  uint8_t           DesA;          /* designator for pin A (char) */
  uint8_t           DesB;          /* designator for pin B (char) */
  uint8_t           DesC;          /* designator for pin C (char) */
  uint8_t           Flags;         /* misc flags */
  uint16_t          U_1;           /* voltage #1 */
  int16_t           U_2;           /* voltage #2 (+/-) */
  int16_t           U_3;           /* voltage #3 (+/-) */
  uint32_t          F_1;           /* factor #1 */
  #ifdef SW_REVERSE_HFE
  uint32_t          F_2;           /* factor #2 */
  #endif
  uint32_t          I_value;       /* current */
  int8_t            I_scale;       /* exponent of factor (value * 10^x) */
  uint32_t          C_value;       /* capacitance */
  int8_t            C_scale;       /* exponent of factor (value * 10^x) */
} Semi_Type;

/* 
  Mapping

           BJT          FET          SCR          Triac        IGBT
  ----------------------------------------------------------------------
  A        Base         Gate         Gate         Gate         Gate
  B        Collector    Drain        Anode        MT2          Collector
  C        Emitter      Source       Cathode      MT1          Emitter
  U_1      V_BE (mV)    R_DS (0.01)  V_GT (mV)    V_GT (mV)
  U_2      I_E (µA)     V_th (mV)                              V_th (mV)
  U_3      I_C/E (µA)   V_GS(off)
  F_1      hFE                                    MT2 (mV)
  F_2      hFEr
  I_value  I_CEO        I_DSS
  I_scale  I_CEO        I_DSS
  C_value  C_EB/BE
  C_scale  C_EB/BE

           Zener
  ----------------------------------------------------------------------
  U_1      V_Z (mV)
*/

/* special semiconductors */
typedef struct
{
  uint8_t           A;             /* probe connected to pin A [0-2] */
  uint8_t           B;             /* probe connected to pin B [0-2] */
  uint8_t           C;             /* probe connected to pin C [0-2] */
  uint16_t          U_1;           /* voltage #1 */
  uint16_t          U_2;           /* voltage #2 */
} AltSemi_Type;

/* 
  Mapping

          PUT         UJT
  ------------------------------------------------------------------
  A       Gate        Emitter
  B       Anode       B2
  C       Cathode     B1
  U_1     V_f
  U_2     V_T
*/


extern void SemiPinDesignators(void);
extern void GetGateThreshold(uint8_t Type);
extern uint32_t Get_hfe_c(uint8_t Type);
extern void GetLeakageCurrent(uint8_t Mode);

extern Diode_Type *SearchDiode(uint8_t A, uint8_t C);
extern void CheckDiode(void);

extern void VerifyMOSFET(void);
extern void CheckTransistor(uint8_t BJT_Type, uint16_t U_Rl);
extern void CheckDepletionModeFET(uint16_t U_Rl);

extern uint8_t CheckThyristorTriac(void);
extern void CheckPUT(void);


#ifdef SW_UJT

extern const unsigned char UJT_str[];

extern void CheckUJT(void);

extern const unsigned char R_BB_str[];
extern const unsigned char UJT_str[];

#endif // SW_UJT


extern Diode_Type      Diodes[];           /* diodes */
extern Semi_Type       Semi;               /* common semiconductor */
extern AltSemi_Type    AltSemi;            /* special semiconductor */

#ifdef SW_HFE_CURRENT
extern const unsigned char I_str[]; 
#endif

#ifdef SW_REVERSE_HFE
extern const unsigned char h_FE_r_str[]; 
#endif

#ifdef SW_C_BE
extern const unsigned char C_be_str[]; 
#endif


#endif // SEMI_H
