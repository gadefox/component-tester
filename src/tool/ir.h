/* ************************************************************************
 *
 *   IR functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#if defined (SW_IR_RECEIVER) || defined (HW_IR_RECEIVER) || defined(SW_IR_TRANSMITTER)

#ifndef IR_H
#define IR_H


extern const unsigned char IR_JVC_str[];
extern const unsigned char IR_Kaseikyo_str[];
extern const unsigned char IR_Matsushita_str[];
extern const unsigned char IR_Motorola_str[];
extern const unsigned char IR_Proton_str[];
extern const unsigned char IR_RC5_str[];
extern const unsigned char IR_RC6_str[];
extern const unsigned char IR_Samsung_str[];
extern const unsigned char IR_Sharp_str[];

#if defined(SW_IR_RX_EXTRA) || defined(SW_IR_TX_EXTRA)
extern const unsigned char IR_Thomson_str[];
#endif


#endif // IR_H

#endif // SW_IR_RECEIVER || HW_IR_RECEIVER || SW_IR_TRANSMITTER
