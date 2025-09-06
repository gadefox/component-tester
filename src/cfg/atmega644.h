/* ************************************************************************
 *
 *   ATmega 324/644/1284 specific global configuration, setup and settings
 *
 *   (c) 2012-2024 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef CONFIG_ATMEGA644_H
#define CONFIG_ATMEGA644_H


/*
 *  include header files
 */

#include "atmega.h"


/* ************************************************************************
 *   display module
 * ************************************************************************ */

#ifdef LCD_RDDISPLAY
#error <<< RD display is not suported for this mcu! >>>
#endif


/* ************************************************************************
 *   port and pin assignments
 * ************************************************************************ */

/*
 *  test pins / probes:
 *  - Must be an ADC port!
 *  - It's recommended to use the lower 3 pins for the probes.
 *  - Don't share this port with POWER_CTRL or TEST_BUTTON!
 */

#define ADC_PORT         PORTA     /* port data register */
#define ADC_DDR          DDRA      /* port data direction register */
#define ADC_PIN          PINA      /* port input pins register */
#define TP1              PA0       /* test pin / probe #1 */
#define TP2              PA1       /* test pin / probe #2 */
#define TP3              PA2       /* test pin / probe #3 */

#define TP_ZENER         PA3       /* test pin for for Zener check (10:1 voltage divider) */
#define TP_REF           PA4       /* test pin for 2.5V reference and relay */
#define TP_BAT           PA5       /* test pin for battery (4:1 voltage divider) */
#define TP_CAP           PA7       /* test pin for self-adjustment cap */
#define TP_LOGIC         PA6       /* test pin for Logic Probe (4:1 voltage divider) */

/*
 *  probe resistors
 *  - For PWM/squarewave output via probe #2 R_RL_2 has to be PD4/OC1B.
 *  - Don't share this port with POWER_CTRL or TEST_BUTTON!
 */

#define R_PORT           PORTD     /* port data register */
#define R_DDR            DDRD      /* port data direction register */
#define R_RL_1           PD2       /* Rl (680R) for test pin #1 */
#define R_RH_1           PD3       /* Rh (470k) for test pin #1 */
#define R_RL_2           PD4       /* Rl (680R) for test pin #2 */
#define R_RH_2           PD5       /* Rh (470k) for test pin #2 */
#define R_RL_3           PD6       /* Rl (680R) for test pin #3 */
#define R_RH_3           PD7       /* Rh (470k) for test pin #3 */


/*
 *  dedicated signal output via OC1B
 *  - don't change this!
 */

#define SIGNAL_PORT      PORTD     /* port data register */
#define SIGNAL_DDR       DDRD      /* port data direction register */
#define SIGNAL_OUT       PD4       /* MCU's OC1B pin */


/*
 *  power control
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define POWER_PORT       PORTC     /* port data register */
#define POWER_DDR        DDRC      /* port data direction register */
#define POWER_CTRL       PC6       /* control pin (1: on / 0: off) */


/*
 *  test push button
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define BUTTON_PORT      PORTC     /* port data register */
#define BUTTON_DDR       DDRC      /* port data direction register */
#define BUTTON_PIN       PINC      /* port input pins register */
#define TEST_BUTTON      PC7       /* test/start push button (low active) */


/*
 *  rotary encoder
 */

#define ENCODER_PORT     PORTC     /* port data register */
#define ENCODER_DDR      DDRC      /* port data direction register */
#define ENCODER_PIN      PINC      /* port input pins register */
#define ENCODER_A        PC3       /* rotary encoder A signal */
#define ENCODER_B        PC4       /* rotary encoder B signal */


/*
 *  increase/decrease push buttons
 */

#define KEY_PORT         PORTC     /* port data register */
#define KEY_DDR          DDRC      /* port data direction register */
#define KEY_PIN          PINC      /* port input pins register */
#define KEY_INC          PC4       /* increase push button (low active) */
#define KEY_DEC          PC3       /* decrease push button (low active) */


/*
 *  frequency counter
 *  - basic and extented version
 *  - input must be pin PB0/T0
 */

/* counter input */
#define COUNTER_PORT          PORTB     /* port data register */
#define COUNTER_DDR           DDRB      /* port data direction register */
#define COUNTER_IN            PB0       /* signal input T0 */

/* control of extended frequency counter */
#define COUNTER_CTRL_PORT     PORTC     /* port data register */ 
#define COUNTER_CTRL_DDR      DDRC      /* port data direction register */
#define COUNTER_CTRL_DIV      PC0       /* prescaler (low 1:1, high x:1) */
#define COUNTER_CTRL_CH0      PC1       /* channel addr #0 */
#define COUNTER_CTRL_CH1      PC2       /* channel addr #1 */


/*
 *  ring tester
 *  - counter input must be pin PB0/T0 (uses COUNTER_IN)
 */

#define RINGTESTER_PORT       PORTC     /* port data register */
#define RINGTESTER_DDR        DDRC      /* port data direction register */
#define RINGTESTER_OUT        PC0       /* pulse output */


/*
 *  L/C meter
 *  - frequency input must be pin PB0/T0 (uses COUNTER_IN)
 *  - control of L/C meter
 */

#define LC_CTRL_PORT     PORTC     /* port data register */ 
#define LC_CTRL_DDR      DDRC      /* port data direction register */
#define LC_CTRL_CP       PC0       /* reference cap (low: on / high: off) */
#define LC_CTRL_LC       PC1       /* L/C selection (low: C / high: L */


/*
 *  IR detector/decoder
 *  - fixed module connected to dedicated I/O pin
 */

#define IR_PORT          PORTC     /* port data register */
#define IR_DDR           DDRC      /* port data direction register */
#define IR_PIN           PINC      /* port input pins register */
#define IR_DATA          PC2       /* data signal */


/*
 *  TTL serial interface
 *  - hardware USUART uses:
 *    USART0: Rx PD0 and Tx PD1
 *    USART1: Rx PD2 and Tx PD3
 */

/* for hardware TTL serial */
#define SERIAL_USART     0         /* use USART0 */
/* for bit-bang TTL serial */
#define SERIAL_PORT      PORTD     /* port data register */
#define SERIAL_DDR       DDRD      /* port data direction register */
#define SERIAL_PIN       PIND      /* port input pins register */
#define SERIAL_TX        PD1       /* pin for Tx (transmit) */
#define SERIAL_RX        PD0       /* pin for Rx (receive) */
#define SERIAL_PCINT     24        /* PCINT# for Rx pin */


/*
 *  OneWire
 *  - dedicated I/O pin
 */

#define ONEWIRE_PORT     PORTC     /* port data register */
#define ONEWIRE_DDR      DDRC      /* port data direction register */
#define ONEWIRE_PIN      PINC      /* port input pins register */
#define ONEWIRE_DQ       PC2       /* DQ (data line) */


/*
 *  fixed cap for self-adjustment
 *  - ADC pin is TP_CAP from above
 *  - settings are for 470k resistor
 *  - should be film cap with 100nF - 1000nF
 */

#define ADJUST_PORT      PORTC     /* port data register */
#define ADJUST_DDR       DDRC      /* port data direction register */
#define ADJUST_RH        PC5       /* Rh (470k) for fixed cap */


/*
 *  buzzer
 */

#define BUZZER_PORT      PORTC     /* port data register */
#define BUZZER_DDR       DDRC      /* port data direction register */
#define BUZZER_CTRL      PC2       /* control pin (low: off / high: on) */


/*
 *  MAX6675
 */

#define MAX6675_PORT     PORTC     /* port data register */
#define MAX6675_DDR      DDRC      /* port data direction register */
#define MAX6675_CS       PC2       /* port pin used for /CS */


/*
 *  MAX31855
 */

#define MAX31855_PORT    PORTC     /* port data register */
#define MAX31855_DDR     DDRC      /* port data direction register */
#define MAX31855_CS      PC2       /* port pin used for /CS */


/*
 *  boost converter for Zener check (dedicated I/O pin)
 */

#define BOOST_PORT       PORTC     /* port data register */
#define BOOST_DDR        DDRC      /* port data direction register */
#define BOOST_CTRL       PC2       /* control pin */


/*
 *  flashlight / general purpose switched output
 */

#define FLASHLIGHT_PORT  PORTC     /* port data register */
#define FLASHLIGHT_DDR   DDRC      /* port data direction register */
#define FLASHLIGHT_CTRL  PC2       /* control pin */


/*
 *  relay for parallel cap (sampling ADC)
 *  - between TP1 & TP3
 *  - cap should have 10nF - 27nF
 *  - not supported yet
 */

#define CAP_RELAY_PORT   PORTC     /* port data register */
#define CAP_RELAY_DDR    DDRC      /* port data direction register */
#define CAP_RELAY_CTRL   PC2       /* control pin */


/* ************************************************************************
 *   internal stuff
 * ************************************************************************ */


/* ADC reference selection: internal 1.1V bandgap */
#define ADC_REF_BANDGAP       (1 << REFS1)

/* ADC reference selection: internal 2.56V (bandgap * 2.328) */
#define ADC_REF_256           ((1 << REFS1) | (1 << REFS0))

/* ADC MUX channel: internal 1.1V bandgap reference */
#define ADC_CHAN_BANDGAP      0x1e      /* 11110 */

/* ADC MUX channel: filter mask for register bits */
#define ADC_CHAN_MASK         0b00011111     /* MUX0-4 */


/* ************************************************************************
 *   MCU specific setup to support different AVRs
 * ************************************************************************ */

#if defined (__AVR_ATmega324P__)
  /*
   *  ATmega 324P/324PA
   */

  /* this MCU has 32kB Flash, 1kB EEPROM and 2kB RAM (enable extra features) */
  #define RES_FLASH           32
  #define RES_EEPROM          1
  #define RES_RAM             2
#elif defined (__AVR_ATmega644__)
  /*
   *  ATmega 644/644P/644PA
   */

  /* this MCU has 64kB Flash, 2kB EEPROM and 4kB RAM (enable extra features) */
  #define RES_FLASH           64
  #define RES_EEPROM          2
  #define RES_RAM             4
#elif defined (__AVR_ATmega1284__)
  /*
   *  ATmega 1284/1284P
   */

  /* this mcu has 128kb flash, 4kb eeprom and 16kb ram (enable extra features) */
  #define res_flash           128
  #define res_eeprom          4
  #define RES_RAM             16
#else
  /*
   *  missing or unsupported MCU
   */

  #error <<< No or wrong MCU type selected! >>>
#endif


#endif // CONFIG_ATMEGA644_H
