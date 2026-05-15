/* ************************************************************************
 *
 *   opto coupler tool functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_OPTO_COUPLER

#ifndef OPTO_COUPLER_H
#define OPTO_COUPLER_H


#define VAR_START_STR

extern void OptoCoupler_Tool(void);

extern const unsigned char If_str[];
extern const unsigned char t_on_str[];
extern const unsigned char t_off_str[];

extern const unsigned char OptoCoupler_str[];
extern const unsigned char None_str[];
extern const unsigned char CTR_str[];


#endif // OPTO_COUPLER_H

#endif // SW_OPTO_COUPLER
