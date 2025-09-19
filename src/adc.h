/* ************************************************************************
 *
 *   ADC functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef ADC_H
#define ADC_H

extern uint16_t ReadU(uint8_t Channel);

extern uint16_t ReadU_5ms(uint8_t Channel);
extern uint16_t ReadU_20ms(uint8_t Channel);

#endif // ADC_H
