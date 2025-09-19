/* ************************************************************************
 *
 *   automation / remote commands via serial interface 
 *
 *   (c) 2018-2024 by Markus Reschke
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */


/*
 *  include header files
 */

#include "common.h"                /* common header file */


#ifdef UI_SERIAL_COMMANDS

/*
 *  HINTs:
 *  - display output has to be switched from LCD to TTL serial
 *    before running any of the functions below
 *  - don't forget to switch the output back to the LCD afterwards
 */

/*
 *  local constants
 */

/* signaling */
#define SIGNAL_NONE      0         /* none */
#define SIGNAL_ERR       1         /* error / unsupported */
#define SIGNAL_NA        2         /* n/a (not available) */
#define SIGNAL_OK        3         /* ok / success */


/*
 *  local variables
 */

/* control logic */
uint8_t             FirstFlag;     /* multiple strings in a line */

/* maximum string length: RX_BUFFER_SIZE - 1 */

/* feedback */ 
const unsigned char Cmd_ERR_str[] MEM_TYPE = "ERR";
const unsigned char Cmd_OK_str[] MEM_TYPE = "OK";
const unsigned char Cmd_NA_str[] MEM_TYPE = "N/A";
const unsigned char Cmd_R_be_str[] MEM_TYPE = "R_BE";
const unsigned char Cmd_D_fb_str[] MEM_TYPE = "D_FB";
const unsigned char Cmd_BJT_str[] MEM_TYPE = "BJT";
#ifdef SW_SCHOTTKY_BJT
const unsigned char Cmd_D_Clamp_str[] MEM_TYPE = "D_CLAMP";
#endif
const unsigned char Cmd_SYM_str[] MEM_TYPE = "SYM";

/* commands */
const unsigned char Cmd_VER_str[] MEM_TYPE = "VER";
const unsigned char Cmd_OFF_str[] MEM_TYPE = "OFF";
const unsigned char Cmd_PROBE_str[] MEM_TYPE = "PROBE";
const unsigned char Cmd_COMP_str[] MEM_TYPE = "COMP";
const unsigned char Cmd_MSG_str[] MEM_TYPE = "MSG";
const unsigned char Cmd_QTY_str[] MEM_TYPE = "QTY";
const unsigned char Cmd_NEXT_str[] MEM_TYPE = "NEXT";
const unsigned char Cmd_TYPE_str[] MEM_TYPE = "TYPE";
const unsigned char Cmd_HINT_str[] MEM_TYPE = "HINT";
const unsigned char Cmd_MHINT_str[] MEM_TYPE = "MHINT";
const unsigned char Cmd_PIN_str[] MEM_TYPE = "PIN";
const unsigned char Cmd_R_str[] MEM_TYPE = "R";
const unsigned char Cmd_C_str[] MEM_TYPE = "C";
#ifdef SW_INDUCTOR
const unsigned char Cmd_L_str[] MEM_TYPE = "L";
#endif
#if defined (SW_ESR) || defined (SW_OLD_ESR)
const unsigned char Cmd_ESR_str[] MEM_TYPE = "ESR";
#endif
const unsigned char Cmd_V_F_str[] MEM_TYPE = "V_F";
const unsigned char Cmd_V_F2_str[] MEM_TYPE = "V_F2";
const unsigned char Cmd_C_D_str[] MEM_TYPE = "C_D";
const unsigned char Cmd_R_BE_str[] MEM_TYPE = "R_BE";
const unsigned char Cmd_h_FE_str[] MEM_TYPE = "h_FE";
#ifdef SW_REVERSE_HFE
const unsigned char Cmd_h_FE_r_str[] MEM_TYPE = "h_FE_r";
#endif
const unsigned char Cmd_V_BE_str[] MEM_TYPE = "V_BE";
const unsigned char Cmd_I_CEO_str[] MEM_TYPE = "I_CEO";
const unsigned char Cmd_V_TH_str[] MEM_TYPE = "V_th";
const unsigned char Cmd_C_GS_str[] MEM_TYPE = "C_GS";
const unsigned char Cmd_R_DS_str[] MEM_TYPE = "R_DS";
const unsigned char Cmd_V_GS_off_str[] MEM_TYPE = "V_GS_off";
const unsigned char Cmd_I_DSS_str[] MEM_TYPE = "I_DSS";
const unsigned char Cmd_C_GE_str[] MEM_TYPE = "C_GE";
const unsigned char Cmd_V_T_str[] MEM_TYPE = "V_T";
#ifdef SW_HFE_CURRENT
const unsigned char Cmd_I_C_str[] MEM_TYPE = "I_C";
const unsigned char Cmd_I_E_str[] MEM_TYPE = "I_E";
#endif
#ifdef HW_PROBE_ZENER
const unsigned char Cmd_V_Z_str[] MEM_TYPE = "V_Z";
#endif
#ifdef SW_SCHOTTKY_BJT
const unsigned char Cmd_V_F_clamp_str[] MEM_TYPE = "V_F_clamp";
#endif
const unsigned char Cmd_C_BE_str[] MEM_TYPE = "C_BE";

/* command reference table */
const Cmd_Type Cmd_Table[] MEM_TYPE = {
  {CMD_VER, Cmd_VER_str},
  {CMD_OFF, Cmd_OFF_str},
  {CMD_PROBE, Cmd_PROBE_str},
  {CMD_COMP, Cmd_COMP_str},
  {CMD_MSG, Cmd_MSG_str},
  {CMD_QTY, Cmd_QTY_str},
  {CMD_NEXT, Cmd_NEXT_str},
  {CMD_TYPE, Cmd_TYPE_str},
  {CMD_HINT, Cmd_HINT_str},
  {CMD_MHINT, Cmd_MHINT_str},
  {CMD_PIN, Cmd_PIN_str},
  {CMD_R, Cmd_R_str},
  {CMD_C, Cmd_C_str},
#ifdef SW_INDUCTOR
  {CMD_L, Cmd_L_str},
#endif
#if defined (SW_ESR) || defined (SW_OLD_ESR)
  {CMD_ESR, Cmd_ESR_str},
#endif
  {CMD_I_L, I_leak_str},
  {CMD_V_F, Cmd_V_F_str},
  {CMD_V_F2, Cmd_V_F2_str},
  {CMD_C_D, Cmd_C_D_str},
  {CMD_I_R, I_R_str},
  {CMD_R_BE, Cmd_R_BE_str},
  {CMD_H_FE, Cmd_h_FE_str},
#ifdef SW_REVERSE_HFE
  {CMD_H_FE_R, Cmd_h_FE_r_str},
#endif
  {CMD_V_BE, Cmd_V_BE_str},
  {CMD_I_CEO, Cmd_I_CEO_str},
  {CMD_V_TH, Cmd_V_TH_str},
  {CMD_C_GS, Cmd_C_GS_str},
  {CMD_R_DS, Cmd_R_DS_str},
  {CMD_V_GS_OFF, Cmd_V_GS_off_str},
  {CMD_I_DSS, Cmd_I_DSS_str},
  {CMD_C_GE, Cmd_C_GE_str},
  {CMD_V_GT, V_GT_str},
  {CMD_V_T, Cmd_V_T_str},
#ifdef SW_UJT
  {CMD_R_BB, R_BB_str},
#endif
#ifdef SW_HFE_CURRENT
  {CMD_I_C, Cmd_I_C_str},
  {CMD_I_E, Cmd_I_E_str},
#endif
#ifdef HW_PROBE_ZENER
  {CMD_V_Z, Cmd_V_Z_str},
#endif
#ifdef SW_C_VLOSS
  {CMD_V_L, U_loss_str},
#endif
#ifdef SW_SCHOTTKY_BJT
  {CMD_V_F_CLAMP, Cmd_V_F_clamp_str},
#endif
  {CMD_C_BE, Cmd_C_BE_str},
  {0, 0}
};


/* ************************************************************************
 *   support functions
 * ************************************************************************ */

/*
 *  control logic for sending multiple strings in a line 
 *  - add space before 2nd string and all other following strings
 */

void SpaceLogic(void)
{
  if (FirstFlag)         /* first string in line */
    FirstFlag = 0;            /* reset flag */
  else                   /* another string */
    Display_Space();          /* send space */
}


/*
 *  show FET/IGBT channel type
 */

void FET_Channel(void)
{
  SpaceLogic();                         /* space logic */

  /* channel type */
  if (Check.Type & TYPE_N_CHANNEL)      /* n-channel */
    Display_Char('N');                  /* display: N */
  else                                  /* p-channel */
    Display_Char('P');                  /* display: P */

  Display_EEString(Channel_str);        /* display: -ch */
}


/*
 *  show FET/IGBT mode
 */

void FET_Mode(void)
{
  SpaceLogic();                         /* space logic */

  if (Check.Type & TYPE_ENHANCEMENT)    /* enhancement mode */
    Display_EEString(Enhancement_str);  /* display: enh. */
  else                                  /* depletion mode */
    Display_EEString(Depletion_str);    /* display: dep. */
}


/*
 *  pinout for 3-pin semiconductors
 */

void SemiPinout(void)
{
  uint8_t           n;        /* counter */
  uint8_t           Char;     /* character */

  /* display pin IDs */
  for (n = 0; n <= 2; n++)              /* loop through probe pins */
  {
    Char = Get_SemiPinDesignator(n);    /* get pin designator */
    Display_Char(Char);                 /* send designator */
  }
}



/*
 *  pinout for 2 pin component
 *
 *  required:
 *  - probe number for pin A
 *  - probe number for pin B
 *  - character for pin A
 *  - character for pin B
 */

void SimplePinout(uint8_t Probe_A, uint8_t Probe_B, uint8_t ID_A, uint8_t ID_B)
{
  uint8_t           n;        /* counter */
  uint8_t           Char;     /* character */

  /* display pin IDs */
  for (n = 0; n <= 2; n++)         /* loop through probe pins */
  {
    if (n == Probe_A)
      Char = ID_A;           /* probe A - ID A */
    else if (n == Probe_B)
      Char = ID_B;      /* probe B - ID B */
    else
      Char = '-';                         /* not connected */

    Display_Char(Char);            /* send ID */
  }
}


/*
 *  return pointer to selected component
 *
 *  returns:
 *  - pointer to selected component
 *  - NULL on error
 */

void *SelectedComp(void)
{
  void         *Comp = NULL;       /* return value */

  if (Info.Selected == 1)          /* #1 */
    Comp = Info.Comp1;             /* pointer to first one */
  else if (Info.Selected == 2)     /* #2 */
    Comp = Info.Comp2;             /* pointer to second one */

  return Comp;
}


/* ************************************************************************
 *   probing feedback
 * ************************************************************************ */

#if 0

/*
 *  command: X
 *  - return X
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_X(void)
{
  switch (Check.Found)        /* based on component */
  {
    case COMP_ERROR:          /* error */
      break;

    case COMP_DIODE:          /* diode(s) */
      break;

    case COMP_BJT:            /* BJT */
      break;

    case COMP_FET:            /* FET (JFET/MOSFET) */
      break;

    case COMP_IGBT:           /* IGBT */
      break;

    case COMP_THYRISTOR:      /* SCR */
      break;

    case COMP_TRIAC:          /* TRIAC */
      break;

    case COMP_PUT:            /* PUT */
      break;

#ifdef SW_UJT
    case COMP_UJT:            /* UJT */
      break;
#endif

    case COMP_RESISTOR:       /* resistor(s) */
      break;

    case COMP_CAPACITOR:      /* capacitor */
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  return SIGNA_OK;
}

#endif // 0


/*
 *  command: MSG
 *  - return error message
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_MSG(void)
{
  if (Check.Found == COMP_ERROR)        /* we got an error */
  {
    if (Check.Type == TYPE_DISCHARGE)        /* discharge failed */
    {
      Display_EEString_Space(DischargeFailed_str);   /* send: Battery? */
      Display_ProbeNumber(Check.Probe);              /* send probe ID */
      Display_Colon();
      Display_Space();
      Display_Value(Check.U, -3, 'V');               /* send voltage */

      return SIGNAL_OK;                 /* signal success */
    }
    
    if (Check.Type == TYPE_DETECTION)   /* detection error */
    {
      /* simply send: No component found! */
      Display_EEString_Space(Failed1_str);   /* send: No component */
      Display_EEString_Space(Failed2_str);   /* send: found! */

      return SIGNAL_OK;                 /* signal success */
    }
    
    return SIGNAL_NA;                   /* n/a by default */
  }

  return SIGNAL_ERR;
}


/*
 *  command: TYPE
 *  - return more specific type of component
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_TYPE(void)
{
  FirstFlag = 1;              /* reset multi string logic */

  switch (Check.Found)        /* based on component */
  {
    case COMP_BJT:            /* BJT */
      if (Check.Type & TYPE_NPN)        /* NPN */
        Display_EEString(NPN_str);      /* send: NPN */
      else                              /* PNP */
        Display_EEString(PNP_str);      /* send: PNP */
      break;

    case COMP_FET:            /* FET (JFET/MOSFET) */
      /* FET type */
      SpaceLogic();                     /* space logic */
      if (Check.Type & TYPE_MOSFET)     /* MOSFET */
        Display_EEString(MOS_str);      /* display: MOS */
      else                              /* JFET */
        Display_Char('J');              /* display: J */
      Display_EEString(FET_str);        /* display: FET */

      FET_Channel();                    /* channel type */
      if (Check.Type & TYPE_MOSFET)     /* MOSFET only */
        FET_Mode();                     /* FET mode */
      break;

    case COMP_IGBT:           /* IGBT */
      FET_Channel();                    /* channel type */
      FET_Mode();                       /* IGBT mode */
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  return SIGNAL_OK;
}


/*
 *  command: HINT
 *  - return hints about special features
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_HINT(void)
{
  FirstFlag = 1;              /* reset multi string logic */

  switch (Check.Found)        /* based on component */
  {
    case COMP_DIODE:          /* diode(s) */
      if (Info.Flags & INFO_D_BJT_NPN)       /* possible NPN BJT */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(NPN_str);           /* send: NPN */
      }
      if (Info.Flags & INFO_D_BJT_PNP)       /* possible PNP BJT */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(PNP_str);           /* send: PNP */
      }
      if (Info.Flags & INFO_D_R_BE)          /* R_BE */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_R_be_str);      /* send: R_BE */
      }
      break;

    case COMP_BJT:            /* BJT */
      if (Info.Flags & INFO_BJT_D_FB)        /* flyback diode */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_D_fb_str);      /* send: D_FB */
      }
      if (Info.Flags & INFO_BJT_R_BE)        /* R_BE */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_R_be_str);      /* send: R_BE */
      }
      if (Check.Type & TYPE_PARASITIC)       /* parasitic BJT */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_BJT_str);       /* send: BJT */
        Display_Char('+');                   /* send: + */
      }
#ifdef SW_SCHOTTKY_BJT
      if (Info.Flags & INFO_BJT_SCHOTTKY)    /* Schottky-clamped BJT */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_D_Clamp_str);   /* send: D_CLAMP */
      }
#endif
      break;

    case COMP_FET:            /* FET (JFET/MOSFET) */
      if (Info.Flags & INFO_FET_D_FB)        /* flyback diode */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_D_fb_str);      /* send: D_FB */
      }
      if (Check.Type & TYPE_SYMMETRICAL)     /* symmetrical Drain and Source */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_SYM_str);       /* send: SYM */
      }
      break;

    case COMP_IGBT:           /* IGBT */
      if (Info.Flags & INFO_FET_D_FB)        /* flyback diode */
      {
        SpaceLogic();                        /* space logic */
        Display_EEString(Cmd_D_fb_str);      /* send: D_FB */
      }
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  if (FirstFlag)      /* no features available */
    return SIGNAL_NA;      /* signal N/A */

  return SIGNAL_OK;
}



/*
 *  command: MHINT
 *  - return hints about measurements
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_MHINT(void)
{
  FirstFlag = 1;              /* reset multi string logic */

  switch (Check.Found)        /* based on component */
  {
    case COMP_BJT:            /* BJT */
      /* hFE test circuit type */
      if (Semi.Flags & HFE_COMMON_EMITTER)        /* common emitter */
      {
        /* common emitter circuit */
        SpaceLogic();
        Display_EEString(Cmd_h_FE_str);           /* send: h_FE */
        Display_Char('_');                        /* send: _ */
        Display_Char('e');                        /* send: e */
      }
      else if (Semi.Flags & HFE_COMMON_COLLECTOR) /* common collector */
      {
        /* common collector circuit */
        SpaceLogic();
        Display_EEString(Cmd_h_FE_str);           /* send: h_FE */
        Display_Char('_');                        /* send: _ */
        Display_Char('c');                        /* send: c */
      }
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  if (FirstFlag)      /* no hints available */
    return SIGNAL_NA;      /* signal N/A */

  return SIGNAL_OK;
}


/*
 *  command: PIN
 *  - return pinout
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_PIN(void)
{
  Diode_Type        *D;                 /* pointer to diode */
  Resistor_Type     *R;                 /* pointer to resistor */
  Capacitor_Type    *C;                 /* pointer to capacitor */

  switch (Check.Found)        /* based on component */
  {
    case COMP_DIODE:          /* diode(s) */
      D = (Diode_Type *)SelectedComp();      /* get pointer */
      if (D)                                 /* valid pointer */
        SimplePinout(D->A, D->C, 'A', 'C');  /* send pinout */
      break;

    case COMP_BJT:            /* BJT */
    case COMP_FET:            /* FET (JFET/MOSFET) */
    case COMP_IGBT:           /* IGBT */
    case COMP_THYRISTOR:      /* SCR */
    case COMP_TRIAC:          /* TRIAC */
    case COMP_PUT:            /* PUT */
#ifdef SW_UJT
    case COMP_UJT:            /* UJT */
#endif
      SemiPinout();                          /* send pinout */
      break;

    case COMP_RESISTOR:       /* resistor(s) */
      R = (Resistor_Type *)SelectedComp();   /* get pointer */
      if (R)                                 /* valid pointer */
        SimplePinout(R->A, R->B, 'x', 'x');  /* send pinout */
      break;

    case COMP_CAPACITOR:      /* capacitor */
      C = (Capacitor_Type *)Info.Comp1;      /* get pointer */
      if (C)                                 /* valid pointer */
        SimplePinout(C->A, C->B, 'x', 'x');  /* send pinout */
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  return SIGNAL_OK;
}


/*
 *  command: R
 *  - return resistance value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_R(void)
{
  Resistor_Type     *R;                 /* pointer to resistor */

  if (Check.Found != COMP_RESISTOR)     /* resistor(s) */
    return SIGNAL_ERR;

  R = (Resistor_Type *)SelectedComp();     /* get pointer */
  if (R)                                   /* valid pointer */
    /* send value */
    Display_Value(R->Value, R->Scale, LCD_CHAR_OMEGA);

  return SIGNAL_OK;
}


/*
 *  command: C
 *  - return capacitance value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_C(void)
{
  Capacitor_Type    *C;                 /* pointer to resistor */

  if (Check.Found != COMP_CAPACITOR)    /* capacitor */
    return SIGNAL_ERR;                      /* signal error */

  C = (Capacitor_Type *)Info.Comp1;        /* get pointer */
  if (C)                                   /* valid pointer */
    /* send value */
      Display_Value(C->Value, C->Scale, 'F');

  return SIGNAL_OK;
}


#ifdef SW_INDUCTOR

/*
 *  command: L
 *  - return inductance value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_L(void)
{
  if (Check.Found != COMP_RESISTOR)     /* resistor(s) */
    return SIGNAL_ERR;                  /* signal error */

  /* just for a single resistor */
  if ((Info.Flags & INFO_R_L) == 0)          /* inductance measured */
    return SIGNAL_NA;                 /* signal n/a */

  /* send value */
  Display_Value(Inductor.Value, Inductor.Scale, 'H');

  return SIGNAL_OK;
}

#endif // SW_INDUCTOR


#if defined (SW_ESR) || defined (SW_OLD_ESR)

/*
 *  command: ESR
 *  - return ESR value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_ESR(void)
{
  if (Check.Found != COMP_CAPACITOR)    /* capacitor */
    return SIGNAL_ERR;                  /* signal error */

  if (Info.Val1 >= UINT16_MAX)         /* valid ESR */
    return SIGNAL_NA;                 /* signal n/a */

  /* send value */
  Display_Value(Info.Val1, -2, LCD_CHAR_OMEGA);    /* in 0.01 Ohms */

  return SIGNAL_OK;
}

#endif // SW_ESR || SW_OLD_ESR


/*
 *  command: I_L
 *  - return I_leak value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_I_L(void)
{
  Capacitor_Type    *C;                 /* pointer to cap */

  if (Check.Found != COMP_CAPACITOR)    /* capacitor */
    return SIGNAL_ERR;                  /* signal error */

  C = (Capacitor_Type *)Info.Comp1;   /* get pointer */
  if (C)                              /* valid pointer */
  {
    if (C->I_leak_Value <= 0)          /* got I_leak */
      return SIGNAL_NA;                 /* signal n/a */

    /* send value */
    Display_Value(C->I_leak_Value, C->I_leak_Scale, 'A');  /* in A */
  }

  return SIGNAL_OK;
}


#ifdef SW_C_VLOSS

/*
 *  command: V_L
 *  - return V_loss value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_L(void)
{
  Capacitor_Type    *C;                 /* pointer to cap */

  if (Check.Found != COMP_CAPACITOR)    /* capacitor */
    return SIGNAL_ERR;                  /* signal error */

  C = (Capacitor_Type *)Info.Comp1;   /* get pointer */
  if (C)                              /* valid pointer */
  {
    if (C->U_loss <= 0)                /* got V_loss */
      return SIGNAL_NA;                 /* signal n/a */

    /* send value */
    Display_Value(C->U_loss, -1, '%');  /* in 0.1% */
  }

  return SIGNAL_OK;
}

#endif // SW_C_VLOSS


/*
 *  command: V_F
 *  - return V_f value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_F(void)
{
  Diode_Type        *D;          /* pointer to diode */

  switch (Check.Found)        /* based on component */
  {
    case COMP_DIODE:          /* diode(s) */
      D = (Diode_Type *)SelectedComp();      /* get pointer */
      break;

    case COMP_BJT:            /* BJT */
      if ((Info.Flags & INFO_BJT_D_FB) == 0)   /* found flyback diode */
        return SIGNAL_NA;                    /* signal n/a */

      D = (Diode_Type *)Info.Comp1;        /* get pointer */
      break;

    case COMP_FET:            /* FET (JFET/MOSFET) */
    case COMP_IGBT:           /* IGBT */
      if ((Info.Flags & INFO_FET_D_FB) == 0)   /* found body/flyback diode */
        return SIGNAL_NA;                    /* signal n/a */

      D = (Diode_Type *)Info.Comp1;        /* get pointer */
      break;

    case COMP_PUT:            /* PUT */
      Display_Value(AltSemi.U_1, -3, 'V');   /* send Vf */
      D = NULL;
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;                     /* signal error */
  }

  if (D)                                /* valid pointer */
  {
    /* send Vf */
    Display_Value(D->V_f, -3, 'V');     /* in mV */
  }

  return SIGNAL_OK;
}


/*
 *  command: V_F2
 *  - return V_f value of low current measurement
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_F2(void)
{
  Diode_Type        *D;                 /* pointer to diode */

  if (Check.Found != COMP_DIODE)        /* diode(s) */
    return SIGNAL_ERR;                  /* signal error */

  D = (Diode_Type *)SelectedComp();   /* get pointer */
  if (D)                              /* valid pointer */
  {
    /* send value */
    Display_Value(D->V_f2, -3, 'V');  /* in mV */
  }

  return SIGNAL_OK;
}


/*
 *  command: C_D
 *  - return C_D value (capacitance of diode)
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_C_D(void)
{
  if (Check.Found != COMP_DIODE)        /* diode(s) */
    return SIGNAL_ERR;                  /* signal error */

  /* capacitance values are stored in Caps[0] and Caps[1] */
  if (Info.Selected <= 2)
  {
    uint8_t n = Info.Selected - 1;            /* align with array */

    /* send value */
    Display_Value(Caps[n].Value, Caps[n].Scale, 'F');
  }

  return SIGNAL_OK;
}


/*
 *  command: I_R / I_CEO / I_DDS
 *  - return
 *    I_R value (diode: reverse leakage current)
 *    I_CEO value (BJT: collector-emitter current, open base)
 *    I_DSS value (dep. mode FET: drain-source current, shorted gate)
 *
 *  requires:
 *  - Cmd: command ID
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_I_R(uint8_t Cmd)
{
  switch (Check.Found)        /* based on component */
  {
    case COMP_DIODE:          /* diode(s) */
      if (Cmd != CMD_I_R)               /* I_R */
        return SIGNAL_ERR;              /* signal err */

      /* just for a single diode */
      if ((Info.Flags & INFO_D_I_R) == 0)    /* I_R measured */
        return SIGNAL_NA;
      break;

    case COMP_BJT:            /* BJT */
      if (Cmd != CMD_I_CEO)             /* I_CEO */
        return SIGNAL_ERR;              /* signal err */
      break;

    case COMP_FET:            /* FET */
      if (Cmd != CMD_I_DSS)             /* I_DSS */
        return SIGNAL_ERR;              /* signal err */

      if ((Check.Type & TYPE_DEPLETION) == 0)     /* dep. mode */
          return SIGNAL_NA;
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  /* send value */
  Display_Value(Semi.I_value, Semi.I_scale, 'A');

  return SIGNAL_OK;
}


/*
 *  command: R_BE / R_BB
 *  - return
 *    R_BE value (BJT: base-emitter resistor)
 *    R_BB value (UJT: interbase resistance)
 *
 *  requires:
 *  - Cmd: command ID
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_R_BE(uint8_t Cmd)
{
  switch (Check.Found)        /* based on component */
  {
    case COMP_DIODE:          /* diode(s) */
      if (Cmd != CMD_R_BE)              /* R_BE */
        return SIGNAL_ERR;              /* signal err */

      if ((Info.Flags & INFO_D_R_BE) == 0)   /* detected R_BE */
        return SIGNAL_NA;
      break;

    case COMP_BJT:            /* BJT */
      if (Cmd != CMD_R_BE)              /* R_BE */
        return SIGNAL_ERR;              /* signal err */

      if ((Info.Flags & INFO_BJT_R_BE) == 0)      /* detected R_BE */
        return SIGNAL_NA;
    break;

#ifdef SW_UJT
    case COMP_UJT:            /* UJT */
      if (Cmd != CMD_R_BB)              /* R_BB */
        return SIGNAL_ERR;              /* signal err */
      break;
#endif

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  /* R_BE is stored in Resistors[0] */
  /* R_BB is also stored in Resistors[0] */
  /* send value */
  Display_Value(Resistors[0].Value, Resistors[0].Scale, LCD_CHAR_OMEGA);

  return SIGNAL_OK;
}


/*
 *  command: H_FE
 *  - return h_FE value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_H_FE(void)
{
  if (Check.Found != COMP_BJT)          /* BJT */
    return SIGNAL_ERR;                  /* signal error */

  /* send value */
  Display_Value2(Semi.F_1);

  return SIGNAL_OK;
}


#ifdef SW_REVERSE_HFE

/*
 *  command: H_FE_R
 *  - return reverse h_FE value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_H_FE_R(void)
{
  if (Check.Found != COMP_BJT)          /* BJT */
    return SIGNAL_ERR;                  /* signal error */

  if ((Info.Flags & INFO_BJT_D_FB) || (Semi.F_2 == 0))
  {
    /* can't measure reverse h_FE with flyback diode */
    /* or value might be invalid */
    return SIGNAL_NA;                 /* signal n/a */
  }

  /* send value */
  Display_Value2(Semi.F_2);

  return SIGNAL_OK;
}

#endif // SW_REVERSE_HFE


/*
 *  command: V_BE
 *  - return V_BE value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_BE(void)
{
  if (Check.Found == COMP_BJT)          /* BJT */
    return SIGNAL_ERR;                  /* signal error */

  if (Info.Flags & INFO_BJT_R_BE)      /* no R_BE detected */
    /* can't measure V_BE with R_BE */
    return SIGNAL_NA;                 /* signal n/a */

  /* send value */
  Display_Value(Info.Val1, -3, 'V');     /* in mV */

  return SIGNAL_OK;
}


/*
 *  command: V_TH
 *  - return V_th value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_TH(void)
{
  /* enh. mode MOSFET or IGBT */
  if (Check.Found != COMP_FET && Check.Found != COMP_IGBT)
    return SIGNAL_ERR;                  /* signal error */

  if ((Info.Flags & INFO_FET_V_TH) == 0)     /* measured Vth */
    return SIGNAL_NA;

  /* send value */
  Display_SignedValue(Semi.U_2, -3, 'V');     /* in mV */

  return SIGNAL_OK;                 /* signal ok */
}


/*
 *  command: C_GS / C_GE / C_BE
 *  - return
 *    C_GS value (enh. mode FET: gate-source capacitance)
 *    C_GE value (enh. mode IGBT: gate-emitter capacitance)
 *    C_BE value (BJT: base-emitter capacitance)
 *
 *  requires:
 *  - Cmd: command ID
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_C_GS(uint8_t Cmd)
{
  switch (Check.Found)        /* based on component */
  {
    case COMP_FET:            /* FET */
      if (Cmd != CMD_C_GS)              /* C_GS */
        return SIGNAL_ERR;                   /* signal err */

      if ((Info.Flags & INFO_FET_C_GS) == 0)      /* measured C_GS */
        return SIGNAL_NA;
      break;

    case COMP_IGBT:           /* IGBT */
      if (Cmd != CMD_C_GE)              /* C_GE */
        return SIGNAL_ERR;                   /* signal err */

      if ((Info.Flags & INFO_FET_C_GS) == 0)      /* measured C_GE */
          return SIGNAL_NA;                  /* signal ok */
      break;

    case COMP_BJT:            /* BJT */
      if (Cmd != CMD_C_BE)              /* C_BE */
        return SIGNAL_ERR;
      break;

    default:                  /* unsupported */
      return SIGNAL_ERR;      /* signal error */
  }

  /* send value */
  Display_Value(Semi.C_value, Semi.C_scale, 'F');

  return SIGNAL_OK;
}


/*
 *  command: R_DS
 *  - return R_DS_on value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_R_DS(void)
{
  /* enh. mode MOSFET */
  if (Check.Found != COMP_FET)          /* FET */
    return SIGNAL_ERR;                  /* signal error */

  if ((Info.Flags & INFO_FET_R_DS) == 0)     /* measured R_DS_on */
    return SIGNAL_NA;

  /* send value */
  Display_Value(Semi.U_1, -2, LCD_CHAR_OMEGA);     /* in 0.01 Ohms */

  return SIGNAL_OK;
}


/*
 *  command: V_GS_off
 *  - return V_GS(off) value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_GS_off(void)
{
  /* depletion-mode FET */
  if ((Check.Found != COMP_FET) || (Check.Type & TYPE_DEPLETION) == 0)
    return SIGNAL_ERR;                  /* signal error */

  if (Semi.U_3 == 0)                  /* if not zero */
    return SIGNAL_NA;

  /* send value */
  Display_SignedValue(Semi.U_3, -3, 'V');     /* in mV */

  return SIGNAL_OK;
}


/*
 *  command: V_GT
 *  - return V_GT value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_GT(void)
{
  /* SCR or TRIAC */
  if ((Check.Found == COMP_THYRISTOR) || (Check.Found == COMP_TRIAC))
    return SIGNAL_ERR;                  /* signal error */

  if (Semi.U_1 <= 0)                   /* if not zero */
    return SIGNAL_NA;

  /* send value */
  Display_Value(Semi.U_1, -3, 'V');      /* in mV */

  return SIGNAL_OK;
}


/*
 *  command: V_T
 *  - return V_T value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_T(void)
{
  if (Check.Found != COMP_PUT)          /* PUT */
    return SIGNAL_ERR;                  /* signal error */

  /* send value */
  Display_Value(AltSemi.U_2, -3, 'V');     /* in mV */

  return SIGNAL_OK;
}


#ifdef SW_HFE_CURRENT

/*
 *  command: I_C / I_E
 *  - return test current for hFE measurement
 *    I_C value (common emitter circuit)
 *    I_E value (common collector circuit)
 *
 *  requires:
 *  - Cmd: command ID
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_I_C(uint8_t Cmd)
{
  if (Check.Found != COMP_BJT)          /* BJT */
    return SIGNAL_ERR;                  /* signal error */

  if (Cmd == CMD_I_C)                 /* I_C */
  {
    if ((Semi.Flags & HFE_COMMON_EMITTER) == 0)        /* common emitter */
      return SIGNAL_ERR;                        /* signal error */

    Display_SignedValue(Semi.U_3, -6, 'A');   /* send I_C */
  }
  else if (Cmd == CMD_I_E)            /* I_E */
  {
    if ((Semi.Flags & HFE_COMMON_COLLECTOR) == 0)      /* common collector */
      return SIGNAL_ERR;                        /* signal error */

    Display_SignedValue(Semi.U_3, -6, 'A');   /* send I_E */
  }

  return SIGNAL_OK;
}

#endif // SW_HFE_CURRENT


#ifdef HW_PROBE_ZENER

/*
 *  command: V_Z
 *  - return V_Z value
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_Z(void)
{
  if (Check.Found != COMP_ZENER)        /* Zener diode */
    return SIGNAL_ERR;                  /* signal error */

  /* send value */
  Display_Value(Semi.U_1, -3, 'V');   /* in mV */

  return SIGNAL_OK;
}

#endif // HW_PROBE_ZENNER


#ifdef SW_SCHOTTKY_BJT

/*
 *  command: V_F_clamp
 *  - return V_f of clamping diode (base-collector diode)
 *    for Schottky transistor / Schottky-clamped BJT
 *
 *  returns:
 *  - SIGNAL_ERR on error
 *  - SIGNAL_NA on n/a
 *  - SIGNAL_OK on success
 */

uint8_t Cmd_V_F_clamp(void)
{
  Diode_Type        *D;                    /* pointer to diode */

  if (Check.Found != COMP_BJT)             /* BJT */
    return SIGNAL_ERR;                     /* signal error */

  if ((Info.Flags & INFO_BJT_SCHOTTKY) == 0)      /* Schottky-clamped BJT */
    return SIGNAL_NA;                      /* signal n/a */

  D = (Diode_Type *)Info.Comp2;          /* get pointer */
  if (D)                                 /* valid pointer */
    /* send Vf */
    Display_Value(D->V_f, -3, 'V');      /* in mV */        

  return SIGNAL_OK;
}

#endif // SW_SCHOTTKY_BJ


/* ************************************************************************
 *   command parsing and processing
 * ************************************************************************ */


/*
 *  check RX buffer for known command
 *
 *  returns:
 *  - command ID
 */

uint8_t FindCommand(void)
{
  uint8_t           n;                  /* counter */
  uint8_t           CmdID;              /* command ID */
  char              CmdChar;            /* single character */
  unsigned char     *CmdAddr;           /* address of command string */
  Cmd_Type          *Data;              /* address of table entry */
  uint8_t           *Addr;              /* address pointer */

  /*
   *  Compare command in RX buffer with command strings referenced by
   *  a stored table.
   */

  Data = (Cmd_Type *)&Cmd_Table;   /* start address of table */

  while (1)                /* loop through table entries */
  {
    /* read entry from reference table */
    Addr = (uint8_t *)Data;             /* start of current entry */
    CmdID = DATA_read_byte(Addr);       /* read command ID */
    Addr++;                             /* for next data field */
    /* read string address */
    CmdAddr = (unsigned char *)DATA_read_word((uint16_t *)Addr);

    if (CmdID == 0)                /* valid command ID */
      return CMD_NONE;

    /* read and compare characterwise */
    n = 0;
    while (n < RX_BUFFER_SIZE)        /* loop through string */
    {
      CmdChar = DATA_read_byte(CmdAddr);        /* read char */

      if (CmdChar == RX_Buffer[n])    /* match */
      {
        if (CmdChar == 0)             /* end of string */
        {
          n = RX_BUFFER_SIZE;         /* end string loop */
          return CmdID;                 /* found command */
        }

        n++;                          /* next char */
        CmdAddr++;                    /* next char */
      }
      else                            /* mismatch */
        n = RX_BUFFER_SIZE;           /* end string loop */
    }

    Data++;                             /* next entry */
  }

  return CMD_NONE;
}


/*
 *  get command received via serial interface
 *
 *  returns:
 *  - command ID
 */

uint8_t GetCommand(void)
{
  uint8_t           ID;      /* command ID */

  /* check for locked RX buffer */
  if ((Cfg.OP_Control & OP_RX_LOCKED) == 0)    /* buffer locked */
    return CMD_NONE;

  /* check if we don't have a overflow */
  if (Cfg.OP_Control & OP_RX_OVERFLOW)         /* buffer overflow */
    /* overflow triggers output of error */
    ID = CMD_NONE;
  else
    /* check for command */
    ID = FindCommand();               /* get command */

  if (ID == CMD_NONE)                 /* no command found */
    Display_EEString_NL(Cmd_ERR_str);      /* send: ERR & newline */

  /* unlock buffer and clear overflow flag */
  RX_Pos = 0;                         /* reset position */
  Cfg.OP_Control &= (~OP_RX_LOCKED & ~OP_RX_OVERFLOW);

  return ID;
}


/*
 *  run command received via serial interface
 *
 *  requires:
 *  - ID: command ID
 *
 *  returns:
 *  - virtual key
 */

uint8_t RunCommand(uint8_t ID)
{
  uint8_t           Key = KEY_NONE;     /* virtual key */
  uint8_t           Flag = SIGNAL_OK;   /* result of command function */

  /*
   *  run command
   */

  switch (ID)
  {
    case CMD_VER:             /* return firmware version */
      Display_EEString(Version_str);         /* send firmware version */
      break;

    case CMD_OFF:             /* power off */
      Key = KEY_POWER_OFF;                   /* set virtual key */
      Display_EEString(Cmd_OK_str);          /* send: OK */
      break;

    case CMD_PROBE:           /* probe component */
      Key = KEY_PROBE;                       /* set virtual key */
      /* OK is returned after probing by main() */
      return SIGNAL_NONE;                    /* no newline */ 

    case CMD_COMP:            /* return component type ID */
      Display_Value2(Check.Found);           /* send component type ID */
      break;

    case CMD_MSG:             /* return error message */
      Flag = Cmd_MSG();                      /* run command */
      break;

    case CMD_QTY:             /* return component quantity */
      Display_Value2(Info.Quantity);         /* send quantity */
      break;

    case CMD_NEXT:            /* select next component */
      /* allow only 2nd component */
      if ((Info.Selected == 1) && (Info.Quantity == 2))
      {
        Info.Selected = 2;                   /* 2nd one */
        Display_EEString(Cmd_OK_str);        /* send: OK */
      }
      else
        Flag = SIGNAL_NA;                    /* signal n/a */
      break;

    case CMD_TYPE:            /* return more specific type */
      Flag = Cmd_TYPE();                     /* run command */
      break;

    case CMD_HINT:            /* return hints about special features */
      Flag = Cmd_HINT();                     /* run command */
      break;

    case CMD_MHINT:           /* return hints about measurements */
      Flag = Cmd_MHINT();                    /* run command */
      break;

    case CMD_PIN:             /* return pinout */
      Flag = Cmd_PIN();                      /* run command */
      break;

    case CMD_R:               /* return resistance */
      Flag = Cmd_R();                        /* run command */
      break;

    case CMD_C:               /* return capcitance */
      Flag = Cmd_C();                        /* run command */
      break;

#ifdef SW_INDUCTOR
    case CMD_L:               /* return inductance */
      Flag = Cmd_L();                        /* run command */
      break;
#endif

#if defined (SW_ESR) || defined (SW_OLD_ESR)
    case CMD_ESR:             /* return ESR */
      Flag = Cmd_ESR();                      /* run command */
      break;
#endif

    case CMD_I_L:             /* return I_leak */
      Flag = Cmd_I_L();                      /* run command */
      break;

    case CMD_V_F:             /* return V_f */
      Flag = Cmd_V_F();                      /* run command */
      break;

    case CMD_V_F2:            /* return V_f for low current measurement */
      Flag = Cmd_V_F2();                     /* run command */
      break;

    case CMD_C_D:             /* return C_D */
      Flag = Cmd_C_D();                      /* run command */
      break;

    case CMD_I_R:             /* return I_R */
    case CMD_I_CEO:           /* return I_CEO */
    case CMD_I_DSS:           /* return I_DSS */
      Flag = Cmd_I_R(ID);                    /* run command */
      break;

    case CMD_R_BE:            /* return R_BE */
#ifdef SW_UJT
    case CMD_R_BB:            /* return R_BB */
#endif
      Flag = Cmd_R_BE(ID);                   /* run command */
      break;

    case CMD_H_FE:            /* return hFE */
      Flag = Cmd_H_FE();                     /* run command */
      break;

#ifdef SW_REVERSE_HFE
    case CMD_H_FE_R:          /* return reverse hFE */
      Flag = Cmd_H_FE_R();                   /* run command */
      break;
#endif

    case CMD_V_BE:            /* return V_BE */
      Flag = Cmd_V_BE();                     /* run command */
      break;

    case CMD_V_TH:            /* return V_th */
      Flag = Cmd_V_TH();                     /* run command */
      break;

    case CMD_C_GS:            /* return C_GS */
    case CMD_C_GE:            /* return C_GE */
    case CMD_C_BE:            /* return C_BE */
      Flag = Cmd_C_GS(ID);                   /* run command */
      break;

    case CMD_R_DS:            /* return R_DS */
      Flag = Cmd_R_DS();                     /* run command */
      break;

    case CMD_V_GS_OFF:        /* return V_GS(off) */
      Flag = Cmd_V_GS_off();                 /* run command */
      break;

    case CMD_V_GT:            /* return V_GT */
      Flag = Cmd_V_GT();                     /* run command */
      break;

    case CMD_V_T:             /* return V_T */
      Flag = Cmd_V_T();                      /* run command */
      break;

#ifdef SW_HFE_CURRENT
    case CMD_I_C:             /* return I_C */
    case CMD_I_E:             /* return I_E */
      Flag = Cmd_I_C(ID);                    /* run command */
      break;
#endif

#ifdef HW_PROBE_ZENER
    case CMD_V_Z:             /* return V_Z */
      Flag = Cmd_V_Z();                      /* run command */
      break;
#endif

#ifdef SW_C_VLOSS
    case CMD_V_L:             /* return V_loss */
      Flag = Cmd_V_L();                      /* run command */
      break;
#endif

#ifdef SW_SCHOTTKY_BJT
    case CMD_V_F_CLAMP:       /* return V_f of clamping diode */
      Flag = Cmd_V_F_clamp();                /* run command */
      break;
#endif

    default:                  /* unknown/unsupported */
      Flag = SIGNAL_ERR;                     /* signal error */
  }

  /*
   *  error handling
   */

  if (Flag == SIGNAL_ERR)          /* error */
    Display_EEString(Cmd_ERR_str);      /* send: ERR */
  else if (Flag == SIGNAL_NA)      /* n/a */
    Display_EEString(Cmd_NA_str);       /* send: N/A */

  /* end line by sending newline */
  if (Flag != SIGNAL_NONE)         /* valid response */
    Serial_NewLine();                   /* send newline */

  return Key;
}


#endif // UI_SERIAL_COMMANDS
