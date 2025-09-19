/* ************************************************************************
 *
 *   IR_TX functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_IR_TRANSMITTER

#ifndef IR_TX_H
#define IR_TX_H


#if !defined(HW_ENCODER) && !defined(HW_INCDEC_KEYS) && !defined(HW_TOUCH)
#error <<< IR transmitter tool requires keys >>>
#endif


#define FUNC_DISPLAY_HEXVALUE

extern void IR_RemoteControl(void);

extern const unsigned char IR_NEC_Std_str[];
extern const unsigned char IR_NEC_Ext_str[];
extern const unsigned char IR_SIRC_12_str[];
extern const unsigned char IR_SIRC_15_str[];
extern const unsigned char IR_SIRC_20_str[];

extern const unsigned char IR_Transmitter_str[];
extern const unsigned char IR_Send_str[];


#endif // IR_TX_H

#endif // SW_IR_TRANSMITTER
