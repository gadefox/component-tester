/* ************************************************************************
 *
 *   serial functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined(SERIAL_BITBANG) || defined(SERIAL_HARDWARE)

#ifndef SERIAL_H
#define SERIAL_H


/* TTL serial: either bit-bang or hardware */
#if defined(SERIAL_BITBANG) && defined(SERIAL_HARDWARE)
#error <<< Serial: select either bitbang or hardware serial interface! >>>
#endif


/* ************************************************************************
 *   constants
 * ************************************************************************ */

/* string buffer sizes */
#define RX_BUFFER_SIZE        11        /* 10 chars + terminating 0 */


/*
 *  flags for Info_Type
 */

#define INFO_NONE             0b00000000     /* no flags set */

/* resistor */
#define INFO_R_L              0b00000001     /* measured inductance */

/* diode */
#define INFO_D_R_BE           0b00000001     /* detected B-E resistor */
#define INFO_D_BJT_NPN        0b00000010     /* possible NPN BJT */
#define INFO_D_BJT_PNP        0b00000100     /* possible PNP BJT */
#define INFO_D_I_R            0b00001000     /* measured reverse leakage current */
#define INFO_D_CAP1           0b00010000     /* diode #1: measured capacitance */
#define INFO_D_CAP2           0b00100000     /* diode #2: measured capacitance */

/* BJT */
#define INFO_BJT_D_FB         0b00000001     /* detected flyback diode */
#define INFO_BJT_R_BE         0b00000010     /* detected B-E resistor */
#define INFO_BJT_SCHOTTKY     0b00000100     /* detected Schottky-clamped BJT */

/* FET/IGBT */
#define INFO_FET_D_FB         0b00000001     /* detected body/flyback diode */
#define INFO_FET_SYM          0b00000010     /* symmetrical drain and source */
#define INFO_FET_V_TH         0b00000100     /* measured Vth */
#define INFO_FET_C_GS         0b00001000     /* measured C_GS */
#define INFO_FET_R_DS         0b00010000     /* measured R_DS_on */


/* ************************************************************************
 *   constants for remote commands
 * ************************************************************************ */

/*
 *  command IDs
 */

/* basic commands */
#define CMD_NONE              0    /* no command */
#define CMD_VER               1    /* print firmware version */
#define CMD_OFF               2    /* power off */

/* probing commands */
#define CMD_PROBE             10   /* probe component */
#define CMD_COMP              11   /* return component type ID */
#define CMD_MSG               12   /* return error message */
#define CMD_QTY               13   /* return component quantity */
#define CMD_NEXT              14   /* select next component */
#define CMD_TYPE              15   /* return more sepcific type */
#define CMD_HINT              16   /* return hints on special features */
#define CMD_MHINT             17   /* return hints on measurements */
#define CMD_PIN               18   /* return pinout */
#define CMD_R                 20   /* return resistance */
#define CMD_C                 21   /* return capacitance */
#define CMD_L                 22   /* return inductance */
#define CMD_ESR               23   /* return ESR */
#define CMD_I_L               24   /* return I_leak */
#define CMD_V_F               25   /* return V_f */
#define CMD_V_F2              26   /* return V_f of low current measurement */
#define CMD_C_D               27   /* return C_D */
#define CMD_I_R               28   /* return I_R */
#define CMD_R_BE              29   /* return R_BE */
#define CMD_H_FE              30   /* return hFE */
#define CMD_H_FE_R            31   /* return reverse hFE */
#define CMD_V_BE              32   /* return V_BE */
#define CMD_I_CEO             33   /* return I_CEO */
#define CMD_V_TH              34   /* return V_th */
#define CMD_C_GS              35   /* return C_GS */
#define CMD_R_DS              36   /* return R_DS */
#define CMD_V_GS_OFF          37   /* return V_GS(off) */
#define CMD_I_DSS             38   /* return I_DSS */
#define CMD_C_GE              39   /* return C_GE */
#define CMD_V_GT              40   /* return V_GT */
#define CMD_V_T               41   /* return V_T */
#define CMD_R_BB              42   /* return R_BB */
#define CMD_I_C               43   /* return I_C */
#define CMD_I_E               44   /* return I_E */
#define CMD_V_Z               45   /* return V_Z */
#define CMD_V_L               46   /* return V_loss */
#define CMD_V_F_CLAMP         47   /* return V_f of clamping diode */
#define CMD_C_BE              48   /* return C_BE */


/* TTL serial */
/* control */
#define SER_RX_PAUSE          1         /* pause RX */
#define SER_RX_RESUME         2         /* resume RX */


/* additional component data for remote commands */
typedef struct
{
  /* common stuff */
  uint8_t           Quantity;      /* component quantity */
  uint8_t           Selected;      /* selected component */
  uint8_t           Flags;         /* misc flags */
  void              *Comp1;        /* pointer to component #1 */
  void              *Comp2;        /* pointer to component #2 */
  uint16_t          Val1;          /* value #1 */
} Info_Type;

/* 
  Mapping

          R   C           D    BJT        FET    IGBT
  -----------------------------------------------------------------
  Comp1   R1  C           D1   D_FB       D_FB   D_FB
  Comp2   R2              D2
  Val1        ESR (0.01)       V_BE (mV)
*/


extern void Serial_Setup(void);
extern void Serial_WriteByte(uint8_t Byte);

#ifdef SERIAL_RW


extern void Serial_Ctrl(uint8_t Control);


extern char          RX_Buffer[];        /* serial RX buffer */
extern uint8_t       RX_Pos;             /* position in buffer */

#ifdef SERIAL_BITBANG
extern uint8_t     RX_Char;            /* RX char (bit buffer) */
extern uint8_t     RX_Bits;            /* bit counter for RX char */
#endif


#endif // SERIAL_RW

extern void Serial_Char(unsigned char Char);

#if defined (UI_SERIAL_COPY) || defined (UI_SERIAL_COMMANDS)
extern void Serial_NewLine(void);
#endif


#ifdef UI_SERIAL_COMMANDS
extern Info_Type     Info;               /* additional component data */
#endif


#endif // SERIAL_H

#endif // SERIAL_BITBANG || SERIAL_HARDWARE
