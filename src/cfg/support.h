/* ************************************************************************
 *
 *   support for global configuration
 *
 *   (c) 2012-2025 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef CONFIG_SUPPORT_H
#define CONFIG_SUPPORT_H


/* ************************************************************************
 *   derived values
 * ************************************************************************ */

/*
 *  number of MCU cycles per µs
 *  - min. 1 (for 1MHz)
 *  - max. 20 (for 20MHz)
 */

#define MCU_CYCLES_PER_US     (F_CPU / 1000000)


/*
 *  number of MCU cycles per ADC cycle
 *  - min. 4
 *  - max. 128
 */ 

#define MCU_CYCLES_PER_ADC    (F_CPU / ADC_FREQ)


/*
 *  time of a MCU cycle (in 0.1 ns)
 */

#define MCU_CYCLE_TIME        (10000 / (F_CPU / 1000000))


/*
 *  define clock divider
 *  - supports 1MHz, 2MHz, 4MHz, 8MHz, 16MHz and 20MHz MCU clocks
 *  - we got only 7 fixed prescalers from 2 up to 128
 */

#if F_CPU / ADC_FREQ == 4
  /* 1MHz/250kHz */
#define ADC_CLOCK_DIV (1 << ADPS1) 
#elif F_CPU / ADC_FREQ == 8
  /* 1MHz/125kHz 2MHz/250kHz */
#define ADC_CLOCK_DIV (1 << ADPS1) | (1 << ADPS0)
#elif F_CPU / ADC_FREQ == 16
  /* 2MHz/125kHz 4MHz/250kHz */
#define ADC_CLOCK_DIV (1 << ADPS2)
#elif F_CPU / ADC_FREQ == 32
  /* 4MHz/125kHz 8MHz/250kHz */
#define ADC_CLOCK_DIV (1 << ADPS2) | (1 << ADPS0)
#elif F_CPU / ADC_FREQ == 64
  /* 8MHz/125kHz 16MHz/250kHz */
#define ADC_CLOCK_DIV (1 << ADPS2) | (1 << ADPS1)
#elif F_CPU / ADC_FREQ == 128
  /* 16MHz/125kHz 20MHz/156.25kHz */
#define ADC_CLOCK_DIV (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)
#endif // F_CPU / ADC_FREQ


/* ************************************************************************
 *   memory/storage options 
 * ************************************************************************ */

/*
 *  storage of program data (EEPROM/Flash)
 */

#if defined (DATA_EEPROM)
  /* memory type EEPROM */
  #define MEM_TYPE            EEMEM

  /* read functions */
  #define DATA_read_byte(addr)     eeprom_read_byte(addr)
  #define DATA_read_word(addr)     eeprom_read_word(addr)
#elif defined (DATA_FLASH)
  /* memory type Flash */
  #define MEM_TYPE            PROGMEM

  /* read functions */
  #define DATA_read_byte(addr)     pgm_read_byte(addr)
  #define DATA_read_word(addr)     pgm_read_word(addr)
#endif


#endif // CONFIG_SUPPORT_H
