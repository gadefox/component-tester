/* ************************************************************************
 *
 *   display functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef DISPLAY_H
#define DISPLAY_H


#if defined(UI_KEY_HINTS) && (!defined(HW_ENCODER) && !defined(HW_INCDEC_KEYS) && !defined(HW_TOUCH))
#error <<< Key hints requires keys >>>
#endif


/* additional font characters: probe numbers with reversed colors */
#ifdef UI_PROBE_REVERSED
#define FONT_EXTRA
#endif

#ifdef UI_PREFIX
#define FUNC_DISPLAY_SIGNEDFULLVALUE
#endif

/* LCD module: contrast */
#ifdef LCD_CONTRAST
#define SW_CONTRAST
#else
#define LCD_CONTRAST  0
#endif

/* string buffer sizes */
#define OUT_BUFFER_SIZE       12        /* 11 chars + terminating 0 */
#define NUM_PROBE_COLORS      3         /* probe colors */
#define NUM_PREFIXES          8         /* unit prefixes */
#define NUM_COLOR_CODES      10         /* color codes */
#define NUM_EIA96_MULT        9         /* EIA-96 multiplier codes */


/* UI line modes (bitfield) */
#define LINE_STD              0b00000000     /* standard mode */
#define LINE_KEY              0b00000001     /* wait for key press */
#define LINE_KEEP             0b00000010     /* keep first line */

/* alignment */
#define ALIGN_LEFT            3         /* align left */
#define ALIGN_RIGHT           4         /* align right */

/* custom chars/symbols */
#define LCD_CHAR_ZERO         0    /* unused */
#define LCD_CHAR_DIODE_AC     1    /* diode icon '>|' */
#define LCD_CHAR_DIODE_CA     2	   /* diode icon '|<' */
#define LCD_CHAR_CAP          3    /* capacitor icon '||' */
#define LCD_CHAR_OMEGA        4    /* omega */
#define LCD_CHAR_MICRO        5    /* µ (micro) */
#define LCD_CHAR_RESISTOR_L   6    /* resistor icon left part '[' */
#define LCD_CHAR_RESISTOR_R   7    /* resistor icon right part ']' */

/* optional custom chars */
#define LCD_CHAR_1_INV        8    /* 1 (reversed color) */
#define LCD_CHAR_2_INV        9    /* 2 (reversed color) */
#define LCD_CHAR_3_INV       10    /* 3 (reversed color) */
#define LCD_CHAR_X_INV       11    /* x (reversed color) */
#define LCD_CHAR_BAT_LL      12    /* battery icon left part: low */
#define LCD_CHAR_BAT_LH      13    /* battery icon left part: high */
#define LCD_CHAR_BAT_RL      14    /* battery icon right part: low */
#define LCD_CHAR_BAT_RH      15    /* battery icon right part: high */


/* basic component symbols */
#define SYMBOL_BJT_NPN        0    /* BJT npn */
#define SYMBOL_BJT_PNP        1    /* BJT pnp */
#define SYMBOL_MOSFET_ENH_N   2    /* MOSFET enhancement mode, n-channel */
#define SYMBOL_MOSFET_ENH_P   3    /* MOSFET enhancement mode, p-channel */
#define SYMBOL_MOSFET_DEP_N   4    /* MOSFET depletion mode, n-channel */
#define SYMBOL_MOSFET_DEP_P   5    /* MOSFET depletion mode, p-channel */
#define SYMBOL_JFET_N         6    /* JFET n-channel */
#define SYMBOL_JFET_P         7    /* JFET p-channel */
#define SYMBOL_IGBT_ENH_N     8    /* IGBT enhancement mode, n-channel */
#define SYMBOL_IGBT_ENH_P     9    /* IGBT enhancement mode, p-channel */
#define SYMBOL_SCR           10    /* SCR / thyristor */
#define SYMBOL_TRIAC         11    /* TRIAC */
#define SYMBOL_PUT           12    /* PUT */
#define SYMBOL_UJT           13    /* UJT */

/* additional component symbols */
#define SYMBOL_QUESTIONMARK  14    /* question mark */
#define SYMBOL_DIODE_ZENER   15    /* Zener diode */
#define SYMBOL_CRYSTAL       16    /* quartz crystal */
#define SYMBOL_ONEWIRE       17    /* OneWire device */

/* number of component symbols */
#ifdef SYMBOLS_EXTRA
  #define NUM_SYMBOLS        18    /* basic plus additional symbols */
#else
  #define NUM_SYMBOLS        14    /* basic symbols */
#endif


/* pinout positions (bitfield) */
#define PIN_NONE              0b00000000     /* no output */
#define PIN_LEFT              0b00000001     /* left */
#define PIN_RIGHT             0b00000010     /* right */
#define PIN_BOTTOM            0b00000100     /* bottom */
#define PIN_TOP               0b00001000     /* top */
#define PIN_CENTER            0b00010000     /* center (vertical) */
#define PIN_ALT_CENTER        0b00100000     /* UI_PINOUT_ALT: center (horizontal) */


#define NUM_EIA96_MULT        9         /* EIA-96 multiplier codes */


extern void Display_NextLine(void);


#if defined (UI_KEY_HINTS) || defined (UI_BATTERY_LASTLINE)
extern void Display_LastLine(void);
#endif


#if defined(UI_SERIAL_COPY) || defined(UI_SERIAL_COMMANDS)
extern void Display_Char(unsigned char Char);
#endif


extern void Display_EEString(const unsigned char *String);


#ifdef UI_CENTER_ALIGN
extern void Display_EEString_Center(const unsigned char *String);
#endif


extern void Display_ProbeNumber(uint8_t Probe);
extern void Display_SemiPinDesignator(uint8_t Probe);
extern void Display_Space(void);
extern void Display_Minus(void);
extern void Display_Colon(void);
extern void Display_EEString_Space(const unsigned char *String);
extern void Display_NL_EEString(const unsigned char *String);


#ifdef UI_CENTER_ALIGN
extern void Display_NL_EEString_Center(const unsigned char *String);
#endif


extern void Display_NL_EEString_Space(const unsigned char *String);

extern void LCD_ClearLine2(void);
extern void LCD_ClearLine3(void);


#ifdef UI_SERIAL_COPY

#if !defined(SERIAL_BITBANG) && !defined(SERIAL_HARDWARE)
#error <<< Serial copy requires TTL serial >>>
#endif

extern void Display_Serial_On(void);
extern void Display_Serial_Off(void);

#endif


#ifdef SW_DISPLAY_REG

#ifdef UI_SERIAL_COPY
#error <<< Display reg requires serial copy >>>
#endif

#define FUNC_DISPLAY_HEXBYTE

extern void Display_Serial_Only(void);
extern void Display_LCD_Only(void);

#endif


#ifdef UI_SERIAL_COMMANDS

#if (!defined(SERIAL_BITBANG) && !defined(SERIAL_HARDWARE)) || !defined(SERIAL_RW)
#error <<< Serial copy requires TTL serial >>>
#endif

extern void Display_Serial_Only(void);
extern void Display_LCD_Only(void);

extern void Display_EEString_NL(const unsigned char *String);

#endif


#ifdef UI_COLORED_TITLES

#ifndef LCD_COLOR
#error <<< Title requires color display >>>
#endif

extern void Display_ColoredEEString(const unsigned char *String, uint16_t Color);
extern void Display_ColoredEEString_Space(const unsigned char *String, uint16_t Color);
extern void Display_UseTitleColor(void);

#endif


#ifdef UI_CENTER_ALIGN
extern void Display_CenterLine(uint8_t Lines);
#endif


#ifdef FUNC_DISPLAY_COLOREDEESTRING_CENTER
extern void Display_ColoredEEString_Center(const unsigned char *String, uint16_t Color);
#endif


#if defined (UI_COLORED_TITLES) || defined (UI_COLORED_VALUES)
extern void Display_UseOldColor(void);
#endif


#ifdef UI_COLORED_VALUES

#ifndef LCD_COLOR
#error <<< Value requires color display >>>
#endif

extern void Display_UseValueColor(void);

#endif


#if defined (FUNC_DISPLAY_HEXBYTE) || defined (FUNC_DISPLAY_HEXVALUE)
extern void Display_HexDigit(uint8_t Digit);
#endif


#ifdef FUNC_DISPLAY_HEXBYTE
extern void Display_HexByte(uint8_t Value);
#endif


#ifdef FUNC_DISPLAY_HEXVALUE
extern void Display_HexValue(uint16_t Value, uint8_t Bits);
#endif


#if defined (FUNC_DISPLAY_FULLVALUE) || defined (FUNC_DISPLAY_SIGNEDFULLVALUE)
extern void Display_FullValue(uint32_t Value, uint8_t DecPlaces, unsigned char Unit);
#endif


#ifdef FUNC_DISPLAY_SIGNEDFULLVALUE
extern void Display_SignedFullValue(int32_t Value, uint8_t DecPlaces, unsigned char Unit);
#endif


extern void Display_Value(uint32_t Value, int8_t Exponent, unsigned char Unit);
extern void Display_Value2(uint32_t Value);
extern void Display_SignedValue(int32_t Value, int8_t Exponent, unsigned char Unit);


#ifdef FUNC_EVALUE

#define FUNC_DISPLAY_FULLVALUE

extern void Display_EValue(uint16_t Value, int8_t Scale, unsigned char Unit);

#endif


#ifdef FUNC_EIA96

#define FUNC_DISPLAY_FULLVALUE

/* EIA-96 multiplier code */
extern const unsigned char EIA96_Mult_table[];

extern void Display_EIA96(uint8_t Index, int8_t Scale);

#endif // FUNC_EIA96


#ifdef SW_SYMBOLS
extern void Display_FancySemiPinout(uint8_t Line);
#endif


#ifdef UI_QUARTZ_CRYSTAL

#ifndef SW_SYMBOLS
#error <<< Quartz crystal: no symbols specified! >>>
#endif

#define SYMBOLS_EXTRA

extern void Clear_Symbol(uint8_t Line);

#endif


#ifdef SW_CONTRAST

extern void ChangeContrast(void);

extern const unsigned char Contrast_str[];

#endif // SW_CONTRAST


#ifdef SW_FONT_TEST

#define FUNC_DISPLAY_HEXBYTE

extern void FontTest(void);

extern const unsigned char FontTest_str[];

#endif // SW_FONT_TEST


#ifdef SW_SYMBOL_TEST

#ifndef SW_SYMBOLS
#error <<< Symbol test: no symbols specified! >>>
#endif

#define FUNC_DISPLAY_HEXBYTE
extern void SymbolTest(void);

extern const unsigned char SymbolTest_str[];

#endif // SW_SYMBOL_TEST


#ifdef FUNC_COLORCODE

#define FUNC_DISPLAY_FULLVALUE

/* band colors based on value */
extern const uint16_t ColorCode_table[];

extern void Display_ColorCode(uint16_t Value, int8_t Scale, uint16_t TolBand);

#endif // FUNC_COLORCODE


#ifdef UI_PROBE_COLORS

#ifndef LCD_COLOR
#error <<< Probe requires color display >>>
#endif

extern uint16_t      ProbeColors[];      /* probe color coding */

#endif

/* buffers */
extern char            OutBuffer[];        /* output buffer */

/* unit prefixes: p, n, µ, m, 0, k, M (used by value display) */
extern const unsigned char Prefix_table[];

#ifdef UI_KEY_HINTS
extern const unsigned char Menu_or_Test_str[];
#endif


#endif // DISPLAY_H
