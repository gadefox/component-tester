/* ************************************************************************
 *
 *   IR remote control
 *
 *   (c) 2015-2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

/*
 *  include header files
 */

#include "../common.h"                /* common header file */


#if defined(SW_IR_RECEIVER) || defined(HW_IR_RECEIVER) || defined(SW_IR_TRANSMITTER)


const unsigned char IR_JVC_str[] MEM_TYPE = "JVC";
const unsigned char IR_Kaseikyo_str[] MEM_TYPE = "Kas";
const unsigned char IR_Matsushita_str[] MEM_TYPE = "Mat";
const unsigned char IR_Motorola_str[] MEM_TYPE = "Mot";
const unsigned char IR_Proton_str[] MEM_TYPE = "Prot";
const unsigned char IR_RC5_str[] MEM_TYPE = "RC-5";
const unsigned char IR_RC6_str[] MEM_TYPE = "RC-6";
const unsigned char IR_Samsung_str[] MEM_TYPE = "Sams";
const unsigned char IR_Sharp_str[] MEM_TYPE = "Sharp";

#if defined(SW_IR_RX_EXTRA) || defined(SW_IR_TX_EXTRA)
const unsigned char IR_Thomson_str[] MEM_TYPE = "Thom";
#endif


#endif // SW_IR_RECEIVER || HW_IR_RECEIVER || defined(SW_IR_TRANSMITTER)
