/* ************************************************************************
 *
 *   IR_RX functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined (SW_IR_RECEIVER) || defined (HW_IR_RECEIVER)

#ifndef IR_RX_H
#define IR_RX_H


#ifdef SW_IR_RECEIVER
#ifdef HW_IR_RECEIVER
#error <<< Select either probes or dedicated IO pin for IR detector! >>>
#endif

#if !defined(SW_IR_RX_PINOUT_G_V_D) && !defined (SW_IR_RX_PINOUT_D_G_V) && !defined(SW_IR_RX_PINOUT_D_V_G)
#error <<< IR receiver: no pinout selected! >>>
#endif
#endif // SW_IR_RECEIVER

#if defined(SW_IR_RX_BEEP) && !defined(HW_BUZZER)
#error <<< IR receiver tool requires buzzer >>>
#endif

#define FUNC_DISPLAY_HEXBYTE


/* IR code buffer size */
#define IR_CODE_BYTES         6         /* 6 bytes = 48 bit */


extern void IR_Detector(void);


extern const unsigned char IR_JVC_str[];
extern const unsigned char IR_Kaseikyo_str[];
extern const unsigned char IR_Matsushita_str[];
extern const unsigned char IR_Motorola_str[];
extern const unsigned char IR_Proton_str[];
extern const unsigned char IR_RC5_str[];
extern const unsigned char IR_RC6_str[];
extern const unsigned char IR_Samsung_str[];
extern const unsigned char IR_Sharp_str[];

#if defined (SW_IR_RX_EXTRA) || defined (SW_IR_TX_EXTRA)
extern const unsigned char IR_Thomson_str[];
#endif

extern const unsigned char IR_Detector_str[];

extern const unsigned char IR_NEC_str[];
extern const unsigned char IR_SIRC_str[];

#ifdef SW_IR_RX_EXTRA
extern const unsigned char IR_IR60_str[];
extern const unsigned char IR_RCA_str[];
extern const unsigned char IR_RECS80_str[];
extern const unsigned char IR_Sanyo_str[];
extern const unsigned char IR_uPD1986C_str[];
#endif


#endif // IR_RX_S

#endif // SW_IR_RECEIVER || HW_IR_RECEIVER
