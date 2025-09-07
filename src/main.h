/* ************************************************************************
 *
 *   main functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef MAIN_H
#define MAIN_H


#if defined(UI_PROBING_DONE_BEEP) && !defined(HW_BUZZER)
#error <<< Probing beep requires buzzer >>>
#endif

/* additional font characters: battery symbol */
#ifdef UI_BATTERY
#define FONT_EXTRA
#endif

/* power switch: prefer soft-latching type */
#ifdef POWER_SWITCH_SOFT
#define POWER_SWITCH_MANUAL
#endif

/* battery monitoring: if disabled */
#ifdef BAT_NONE
#undef UI_BATTERY
#undef UI_BATTERY_LASTLINE
#endif

#ifdef UI_QUESTION_MARK
#ifndef SW_SYMBOLS
#error <<< Question mark: no symbols specified! >>>
#endif

#define SYMBOLS_EXTRA
#endif // UI_QUESTION_MARK

#ifdef UI_NO_TEXTPINOUT
#ifndef SW_SYMBOLS
#error <<< No text pinout no symbols specified! >>>
#endif
#endif // UI_NO_TEXTPINOUT


/* operation mode/state flags (bitfield) */
#define OP_NONE               0b00000000     /* no flags */
#define OP_AUTOHOLD           0b00000001     /* auto-hold mode (instead of continuous) */
#define OP_EXT_REF            0b00000100     /* external voltage reference used */
#define OP_SPI                0b00001000     /* SPI is set up */
#define OP_I2C                0b00010000     /* I2C is set up */
#define OP_AUTOHOLD_TEMP      0b00100000     /* temporary auto-hold mode */


/* operation control/signaling flags (bitfield) */
#define OP_BREAK_KEY          0b00000001     /* exit key processing */
#define OP_OUT_LCD            0b00000010     /* output to display */
#define OP_OUT_SER            0b00000100     /* output to TTL serial */
#define OP_RX_LOCKED          0b00001000     /* RX buffer locked */
#define OP_RX_OVERFLOW        0b00010000     /* RX buffer overflow */
#define OP_PWR_TIMEOUT        0b00100000     /* auto-power-off for auto-hold mode */


extern uint8_t Get_SemiPinDesignator(uint8_t Probe);

#if !defined(UI_NO_TEXTPINOUT) || defined(SW_ENCODER)
extern void Show_SemiPinout(void);
#endif

extern void Show_SimplePinout(uint8_t Des1, uint8_t Des2, uint8_t Des3);

extern void CheckVoltageRefs(void);

extern void PowerOff(void);

#ifndef BAT_NONE
extern void ShowBattery(void);
extern void CheckBattery(void);
#endif


/* component symbols */
extern const unsigned char Diode_AC_str[];
extern const unsigned char Diode_CA_str[];


/* firmware */
extern const unsigned char Version_str[];

/* common terms and texts */
extern const unsigned char MOS_str[];
extern const unsigned char FET_str[];
extern const unsigned char Channel_str[];
extern const unsigned char Enhancement_str[];
extern const unsigned char Depletion_str[];
extern const unsigned char IGBT_str[];
extern const unsigned char C_gs_str[];
extern const unsigned char NPN_str[];
extern const unsigned char PNP_str[];
extern const unsigned char h_FE_str[];
extern const unsigned char V_BE_str[];
extern const unsigned char V_GT_str[];
extern const unsigned char I_CEO_str[];
extern const unsigned char Vf_str[];
extern const unsigned char DiodeCap_str[];
extern const unsigned char Vth_str[];
extern const unsigned char I_R_str[];
extern const unsigned char V_T_str[];
extern const unsigned char I_DSS_str[];
extern const unsigned char I_leak_str[];
extern const unsigned char R_DS_str[];
extern const unsigned char V_GSoff_str[];

#if ! defined (BAT_NONE) && ! defined (UI_BATTERY)
extern const unsigned char Battery_str[];
extern const unsigned char OK_str[];
extern const unsigned char Weak_str[];
extern const unsigned char Low_str[];
#endif

#ifdef BAT_EXT_UNMONITORED
extern const unsigned char External_str[];
#endif


#endif // MAIN_H
