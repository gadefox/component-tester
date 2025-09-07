/* ************************************************************************
 *
 *   DHTXX functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifdef SW_DHTXX

#ifndef DHTXX_H
#define DHTXX_H


#define VAR_START_STR
#define FUNC_DISPLAY_SIGNEDFULLVALUE
#define FUNC_DISPLAY_FULLVALUE

#ifdef UI_FAHRENHEIT
#define FUNC_CELSIUS2FAHRENHEIT
#endif

extern uint8_t DHTxx_Tool(void);

extern const unsigned char DHTxx_str[];
extern const unsigned char RH_str[];
extern const unsigned char DHT11_str[];
extern const unsigned char DHT22_str[];


#endif // DHTXX_H

#endif // SW_DHTXX
