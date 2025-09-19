/* ************************************************************************
 *
 *   ATmega 640/1280/2560 specific global configuration, setup and settings
 *
 *   (c) 2012-2024 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef CONFIG_ATMEGA1280_H
#define CONFIG_ATMEGA1280_H


/*
 *  include header files
 */

#include "atmega.h"


/* ************************************************************************
 *   port and pin assignments
 * ************************************************************************ */

/*
 *  test pins / probes:
 *  - Must be an ADC port!
 *    - Port K (ADC8-15) is not supported.
 *  - It's recommended to use the lower 3 pins for the probes.
 *  - Don't share this port with POWER_CTRL or TEST_BUTTON!
 */

#define ADC_PORT         PORTF     /* port data register */
#define ADC_DDR          DDRF      /* port data direction register */
#define ADC_PIN          PINF      /* port input pins register */
#define TP1              PF0       /* test pin / probe #1 */
#define TP2              PF1       /* test pin / probe #2 */
#define TP3              PF2       /* test pin / probe #3 */

#define TP_ZENER         PF3       /* test pin for Zener check (10:1 voltage divider) */
#define TP_REF           PF4       /* test pin for 2.5V reference and relay */
#define TP_BAT           PF5       /* test pin for battery (4:1 voltage divider) */
#define TP_CAP           PF7       /* test pin for self-adjustment cap */
#define TP_LOGIC         PF6       /* test pin for Logic Probe (4:1 voltage divider) */


/*
 *  probe resistors
 *  - For PWM/squarewave output via probe #2 R_RL_2 has to be PB6/OC1B.
 *  - Don't share this port with POWER_CTRL or TEST_BUTTON!
 */

#define R_PORT           PORTK     /* port data register */
#define R_DDR            DDRK      /* port data direction register */
#define R_RL_1           PK0       /* Rl (680R) for test pin #1 */
#define R_RH_1           PK1       /* Rh (470k) for test pin #1 */
#define R_RL_2           PK2       /* Rl (680R) for test pin #2 */
#define R_RH_2           PK3       /* Rh (470k) for test pin #2 */
#define R_RL_3           PK4       /* Rl (680R) for test pin #3 */
#define R_RH_3           PK5       /* Rh (470k) for test pin #3 */


/*
 *  dedicated signal output via OC1B
 *  - don't change this!
 */

#define SIGNAL_PORT      PORTB     /* port data register */
#define SIGNAL_DDR       DDRB      /* port data direction register */
#define SIGNAL_OUT       PB6       /* MCU's OC1B pin */


/*
 *  power control
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define POWER_PORT       PORTA     /* port data register */
#define POWER_DDR        DDRA      /* port data direction register */
#define POWER_CTRL       PA6       /* control pin (1: on / 0: off) */


/*
 *  test push button
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define BUTTON_PORT      PORTA     /* port data register */
#define BUTTON_DDR       DDRA      /* port data direction register */
#define BUTTON_PIN       PINA      /* port input pins register */
#define TEST_BUTTON      PA7       /* test/start push button (low active) */


/*
 *  rotary encoder
 */

#define ENCODER_PORT     PORTA     /* port data register */
#define ENCODER_DDR      DDRA      /* port data direction register */
#define ENCODER_PIN      PINA      /* port input pins register */
#define ENCODER_A        PA3       /* rotary encoder A signal */
#define ENCODER_B        PA1       /* rotary encoder B signal */


/*
 *  increase/decrease push buttons
 */

#define KEY_PORT         PORTA     /* port data register */
#define KEY_DDR          DDRA      /* port data direction register */
#define KEY_PIN          PINA      /* port input pins register */
#define KEY_INC          PA3       /* increase push button (low active) */
#define KEY_DEC          PA1       /* decrease push button (low active) */


/*
 *  frequency counter
 *  - basic and extented version
 *  - input must be pin PD7/T0
 */

/* counter input */
#define COUNTER_PORT          PORTD     /* port data register */
#define COUNTER_DDR           DDRD      /* port data direction register */
#define COUNTER_IN            PD7       /* signal input T0 */

/* control of extended frequency counter */
#define COUNTER_CTRL_PORT     PORTD     /* port data register */ 
#define COUNTER_CTRL_DDR      DDRD      /* port data direction register */
#define COUNTER_CTRL_DIV      PD4       /* prescaler (low 1:1, high x:1) */
#define COUNTER_CTRL_CH0      PD5       /* channel addr #0 */
#define COUNTER_CTRL_CH1      PD6       /* channel addr #1 */


/*
 *  ring tester
 *  - counter input must be pin PD7/T0 (uses COUNTER_IN)
 */

#define RINGTESTER_PORT       PORTD     /* port data register */
#define RINGTESTER_DDR        DDRD      /* port data direction register */
#define RINGTESTER_OUT        PD4       /* pulse output */


/*
 *  L/C meter
 *  - frequency input must be pin PB0/T0 (uses COUNTER_IN)
 *  - control of L/C meter
 */

#define LC_CTRL_PORT     PORTD     /* port data register */ 
#define LC_CTRL_DDR      DDRD      /* port data direction register */
#define LC_CTRL_CP       PD4       /* reference cap (low: on / high: off) */
#define LC_CTRL_LC       PD5       /* L/C selection (low: C / high: L */


/*
 *  IR detector/decoder
 *  - fixed module connected to dedicated I/O pin
 */

#define IR_PORT          PORTA     /* port data register */
#define IR_DDR           DDRA      /* port data direction register */
#define IR_PIN           PINA      /* port input pins register */
#define IR_DATA          PA0       /* data signal */


/*
 *  TTL serial interface
 *  - hardware USART uses
 *    USART0: Rx PE0 and Tx PE1
 *    USART1: Rx PD2 and Tx PD3
 *    USART2: Rx PH0 and Tx PH1
 *    USART3: Rx PJ0 and Tx PJ1
 */

/* for hardware TTL serial */
#define SERIAL_USART     0         /* use USART0 */
/* for bit-bang TTL serial */
#define SERIAL_PORT      PORTE     /* port data register */
#define SERIAL_DDR       DDRE      /* port data direction register */
#define SERIAL_PIN       PINE      /* port input pins register */
#define SERIAL_TX        PE1       /* pin for Tx (transmit) */
#define SERIAL_RX        PE0       /* pin for Rx (receive) */
#define SERIAL_PCINT     8         /* PCINT# for Rx pin */


/*
 *  OneWire
 *  - dedicated I/O pin
 */

#define ONEWIRE_PORT     PORTA     /* port data register */
#define ONEWIRE_DDR      DDRA      /* port data direction register */
#define ONEWIRE_PIN      PINA      /* port input pins register */
#define ONEWIRE_DQ       PA4       /* DQ (data line) */


/*
 *  fixed cap for self-adjustment
 *  - ADC pin is TP_CAP from above
 *  - settings are for 470k resistor
 *  - should be film cap with 100nF - 1000nF
 */

#define ADJUST_PORT      PORTA     /* port data register */
#define ADJUST_DDR       DDRA      /* port data direction register */
#define ADJUST_RH        PA5       /* Rh (470k) for fixed cap */


/*
 *  buzzer
 */

#define BUZZER_PORT      PORTA     /* port data register */
#define BUZZER_DDR       DDRA      /* port data direction register */
#define BUZZER_CTRL      PA2       /* control pin (low: off / high: on) */


/*
 *  MAX6675
 */

#define MAX6675_PORT     PORTA     /* port data register */
#define MAX6675_DDR      DDRA      /* port data direction register */
#define MAX6675_CS       PA2       /* port pin used for /CS */


/*
 *  MAX31855
 */

#define MAX31855_PORT    PORTA     /* port data register */
#define MAX31855_DDR     DDRA      /* port data direction register */
#define MAX31855_CS      PA2       /* port pin used for /CS */


/*
 *  boost converter for Zener check (dedicated I/O pin)
 */

#define BOOST_PORT       PORTA     /* port data register */
#define BOOST_DDR        DDRA      /* port data direction register */
#define BOOST_CTRL       PA2       /* control pin */


/*
 *  flashlight / general purpose switched output
 */

#define FLASHLIGHT_PORT  PORTA     /* port data register */
#define FLASHLIGHT_DDR   DDRA      /* port data direction register */
#define FLASHLIGHT_CTRL  PA2       /* control pin */


/*
 *  relay for parallel cap (sampling ADC)
 *  - between TP1 & TP3
 *  - cap should have 10nF - 27nF
 *  - not supported yet
 */

#define CAP_RELAY_PORT   PORTA     /* port data register */
#define CAP_RELAY_DDR    DDRA      /* port data direction register */
#define CAP_RELAY_CTRL   PA2       /* control pin */


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
#define ADC_CHAN_MASK         0b00011111     /* ADMUX: MUX0-4 */
#define ADC_CHAN_MASK2        0b00001000     /* ADCSRB: MUX5 */


/* ************************************************************************
 *   MCU specific setup to support different AVRs
 * ************************************************************************ */

#if defined (__AVR_ATmega640__)
  /*
   *  ATmega 640
   */

  /* this MCU has 64kB Flash, 4kB EEPROM and 8kB RAM (enable extra features) */
  #define RES_FLASH           64
  #define RES_EEPROM          4
  #define RES_RAM             8
#elif defined (__AVR_ATmega1280__)
  /*
   *  ATmega 1280
   */

  /* this MCU has 128kB Flash, 4kB EEPROM and 8kB RAM (enable extra features) */
  #define RES_FLASH           128
  #define RES_EEPROM          4
  #define RES_RAM             8
#elif defined (__AVR_ATmega2560__)
  /*
   *  ATmega 2560
   */

  /* this MCU has 256kB Flash, 4kB EEPROM and 8kB RAM (enable extra features) */
  #define RES_FLASH           256
  #define RES_EEPROM          4
  #define RES_RAM             8
#else
  /*
   *  missing or unsupported MCU
   */

  #error <<< No or wrong MCU type selected! >>>
#endif


#endif // CONFIG_ATMEGA1280_H
