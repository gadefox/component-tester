/* ************************************************************************
 *
 *   USER functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef USER_H
#define USER_H


#if defined(UI_COLORED_CURSOR) && !defined(LCD_COLOR)
#error <<< Cursor requires color display >>>
#endif

#ifdef HW_BUZZER
/* buzzer type: either active or passive */
#if defined(BUZZER_ACTIVE) && defined(BUZZER_PASSIVE)
#error <<< Buzzer: select either active or passive buzzer! >>>
#elif !defined(BUZZER_ACTIVE) && !defined(BUZZER_PASSIVE)
#error <<< Buzzer: select buzzer type! >>>
#endif
#endif // HW_BUZZER


/* UI feedback mode for TestKey() (bitfield) */
#define CURSOR_NONE           0b00000000     /* no cursor */
#define CURSOR_STEADY         0b00000001     /* steady cursor */
#define CURSOR_BLINK          0b00000010     /* blinking cursor */
#define CHECK_OP_MODE         0b00000100     /* consider operation mode */
#define CHECK_KEY_TWICE       0b00001000     /* check for two short presses of the test key */
#define CHECK_BAT             0b00010000     /* check battery */
#define CURSOR_TEXT           0b00100000     /* show hint instead of cursor */

/* keys (test push button etc.) */
#define KEY_NONE              0    /* no key or error */
#define KEY_TIMEOUT           0    /* timeout */
#define KEY_SHORT             1    /* test push button: short key press */
#define KEY_LONG              2    /* test push button: long key press */
#define KEY_TWICE             3    /* test push button: two short key presses */
#define KEY_RIGHT             4    /* rotary encoder: right turn */
                                   /* push buttons: increase */
#define KEY_LEFT              5    /* rotary encoder: left turn */
                                   /* push buttons: decrease */
#define KEY_INCDEC            6    /* push buttons: increase and decrease */

/* virtual keys */
#define KEY_COMMAND           100  /* remote command (from serial interface) */
#define KEY_MAINMENU          101  /* main menu */
#define KEY_POWER_ON          102  /* just powered up */
#define KEY_POWER_OFF         103  /* power off */
#define KEY_PROBE             104  /* probe component */
#define KEY_EXIT              105  /* exit (menu) */
#define KEY_DEFAULTS          106  /* set defaults (adjustments) */


/* ************************************************************************
 *   constants for additional hardware
 * ************************************************************************ */

/* passive buzzer */
#define BUZZER_FREQ_LOW       0              /* 2.5 kHz */
#define BUZZER_FREQ_HIGH      1              /* 5 kHz */


/* user interface */
typedef struct
{
  /* display */
  uint8_t           LineMode;      /* line mode for LCD_NextLine() */
  uint8_t           CharPos_X;     /* current character x position */
  uint8_t           CharPos_Y;     /* current character y position */
                                   /* top left is 1/1 */
  uint8_t           CharMax_X;     /* max. characters per line */
  uint8_t           CharMax_Y;     /* max. number of lines */
  uint8_t           MaxContrast;   /* maximum contrast */

  /* color support */
#ifdef LCD_COLOR
  uint16_t          PenColor;      /* pen color */
#if defined (UI_COLORED_TITLES) || defined (UI_COLORED_VALUES)
  uint16_t          OldColor;      /* old color */
  #endif
#endif // LCD_COLOR

  /* fancy pinout with symbols */
#ifdef SW_SYMBOLS
  uint8_t           SymbolLine;    /* line for output */
  uint8_t           SymbolSize_X;  /* x size in characters */
  uint8_t           SymbolSize_Y;  /* y size in characters */
  uint8_t           SymbolPos_X;   /* x char position (left) */
  uint8_t           SymbolPos_Y;   /* y char position (top) */
#endif

  /* additional keys (push buttons etc.) */
#if defined(HW_ENCODER) || defined(HW_INCDEC_KEYS) || defined(HW_TOUCH)
  uint8_t           KeyOld;        /* former key */
  uint8_t           KeyStep;       /* step size (1-7) */
  uint8_t           KeyStepOld;    /* former step size */
#endif

  /* rotary encoder */
#ifdef HW_ENCODER
  uint8_t           EncState;      /* last AB status */
  uint8_t           EncDir;        /* turning direction */
  uint8_t           EncPulses;     /* number of Gray code pulses */
  uint8_t           EncTicks;      /* time counter */
#endif

  /* increase/decrease push buttons */
#ifdef HW_INCDEC_KEYS
  /* no additional variables needed */
#endif

  /* touch screen */
#ifdef HW_TOUCH
  uint16_t          TouchRaw_X;    /* raw touch screen x position */
  uint16_t          TouchRaw_Y;    /* raw touch screen y position */
  uint8_t           TouchPos_X;    /* charater x position */
  uint8_t           TouchPos_Y;    /* charater y position */
#endif

  /* key hints */
#ifdef UI_KEY_HINTS
  unsigned char     *KeyHint;      /* string pointer (EEPROM) */
#endif
} UI_Type;


extern int8_t CmpValue(uint32_t Value1, int8_t Scale1, uint32_t Value2, int8_t Scale2);
extern uint32_t RescaleValue(uint32_t Value, int8_t Scale, int8_t NewScale);

#ifdef SW_R_TRIMMER

extern const unsigned char R_t_str[];
extern const unsigned char R1_str[];
extern const unsigned char R2_str[];


extern int8_t NormalizeValue(uint32_t Value1, int8_t Scale1, uint32_t Value2, int8_t Scale2);

#endif // SW_R_TRIMMER


#ifdef FUNC_ROUNDSIGNEDVALUE
extern int32_t RoundSignedValue(int32_t Value, uint8_t Scale, uint8_t RoundScale);
#endif


#ifdef FUNC_CELSIUS2FAHRENHEIT
extern int32_t Celsius2Fahrenheit(int32_t Value, uint8_t Scale);
#endif


#if defined (HW_BUZZER) && defined (BUZZER_PASSIVE)
extern void PassiveBuzzer(uint8_t Mode);
#endif


extern uint8_t TestKey(uint16_t Timeout, uint8_t Mode);
extern void WaitKey(void);


#ifdef FUNC_SMOOTHLONGKEYPRESS
extern void SmoothLongKeyPress(void);
#endif


extern uint8_t ShortCircuit(uint8_t Mode);

extern void MarkItem(uint8_t Item, uint8_t Selected);

extern void AdjustmentMenu(uint8_t Mode);
extern uint8_t MainMenu(void);


#ifdef SW_SELFTEST

#define FUNC_DISPLAY_SIGNEDFULLVALUE

extern const unsigned char Selftest_str[];

#endif

/* common terms and texts */
extern const unsigned char Profile1_str[];
extern const unsigned char Profile2_str[];
#ifdef UI_THREE_PROFILES
extern const unsigned char Profile3_str[];
#endif

/* configuration */
extern UI_Type           UI;                      /* user interface */

#ifdef SW_POWER_OFF
extern const unsigned char PowerOff_str[];
#endif


#endif // USER_H
