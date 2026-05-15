/* ************************************************************************
 *
 *   Language variables
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef LANG_VARIABLES_H
#define LANG_VARIABLES_H


/* firmware */
extern const unsigned char Tester_str[];

/* common terms and texts */
extern const unsigned char Probing_str[];
extern const unsigned char Timeout_str[];
extern const unsigned char Failed1_str[];
extern const unsigned char Failed2_str[];
extern const unsigned char Done_str[];
extern const unsigned char Select_str[];
extern const unsigned char Adjustment_str[];
extern const unsigned char Save_str[];
extern const unsigned char Load_str[];
extern const unsigned char Show_str[];
extern const unsigned char Remove_str[];
extern const unsigned char Create_str[];
extern const unsigned char ShortCircuit_str[];
extern const unsigned char DischargeFailed_str[];
extern const unsigned char Error_str[];
extern const unsigned char Exit_str[];
extern const unsigned char Checksum_str[];
extern const unsigned char BJT_str[];
extern const unsigned char Thyristor_str[];
extern const unsigned char Triac_str[];
extern const unsigned char PUT_str[];
extern const unsigned char Bye_str[];

/* units */
extern const unsigned char Hertz_str[];

#ifdef VAR_START_STR
extern const unsigned char Start_str[];
#endif


#endif // LANG_VARIABLES_H
