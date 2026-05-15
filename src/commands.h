/* ************************************************************************
 *
 *   commands functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef UI_SERIAL_COMMANDS

#ifndef COMMANDS_H
#define COMMANDS_H


/* remote command */
typedef struct
{
  uint8_t                ID;       /* command ID */
  const unsigned char    *Cmd;     /* storage address of command string */
} Cmd_Type;


extern uint8_t GetCommand(void);
extern uint8_t RunCommand(uint8_t ID);


/* feedback */
extern const unsigned char Cmd_ERR_str[];
extern const unsigned char Cmd_OK_str[];
extern const unsigned char Cmd_NA_str[];
extern const unsigned char Cmd_R_be_str[];
extern const unsigned char Cmd_D_fb_str[];
extern const unsigned char Cmd_BJT_str[];

#ifdef SW_SCHOTTKY_BJT
extern const unsigned char Cmd_D_Clamp_str[];
#endif

extern const unsigned char Cmd_SYM_str[];

/* commands */
extern const unsigned char Cmd_VER_str[];
extern const unsigned char Cmd_OFF_str[];
extern const unsigned char Cmd_PROBE_str[];
extern const unsigned char Cmd_COMP_str[];
extern const unsigned char Cmd_MSG_str[];
extern const unsigned char Cmd_QTY_str[];
extern const unsigned char Cmd_NEXT_str[];
extern const unsigned char Cmd_TYPE_str[];
extern const unsigned char Cmd_HINT_str[];
extern const unsigned char Cmd_MHINT_str[];
extern const unsigned char Cmd_PIN_str[];
extern const unsigned char Cmd_R_str[];
extern const unsigned char Cmd_C_str[];

#ifdef SW_INDUCTOR
extern const unsigned char Cmd_L_str[];
#endif

#if defined (SW_ESR) || defined (SW_OLD_ESR)
extern const unsigned char Cmd_ESR_str[];
#endif

extern const unsigned char Cmd_V_F_str[];
extern const unsigned char Cmd_V_F2_str[];
extern const unsigned char Cmd_C_D_str[];
extern const unsigned char Cmd_R_BE_str[];
extern const unsigned char Cmd_h_FE_str[];

#ifdef SW_REVERSE_HFE
extern const unsigned char Cmd_h_FE_r_str[];
#endif

extern const unsigned char Cmd_V_BE_str[];
extern const unsigned char Cmd_I_CEO_str[];
extern const unsigned char Cmd_V_TH_str[];
extern const unsigned char Cmd_C_GS_str[];
extern const unsigned char Cmd_R_DS_str[];
extern const unsigned char Cmd_V_GS_off_str[];
extern const unsigned char Cmd_I_DSS_str[];
extern const unsigned char Cmd_C_GE_str[];
extern const unsigned char Cmd_V_T_str[];

#ifdef SW_HFE_CURRENT
extern const unsigned char Cmd_I_C_str[];
extern const unsigned char Cmd_I_E_str[];
#endif

#ifdef HW_PROBE_ZENER
extern const unsigned char Cmd_V_Z_str[];
#endif

extern const unsigned char Cmd_C_BE_str[];

/* command reference table */
extern const Cmd_Type Cmd_Table[];


#endif // COMMANDS_H

#endif // UI_SERIAL_COMMANDS
