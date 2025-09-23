/* ************************************************************************
 *
 *   Logic Green 328P specific global configuration, setup and settings
 *
 *   (c) 2012-2024 by Markus Reschke
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler
 *   refactored by gadefox@EEVblog
 *
 * ************************************************************************ */

#ifndef CONFIG_LGT8F328_H
#define CONFIG_LGT8F328_H


/* ************************************************************************
 *   ADC clock
 * ************************************************************************ */

/*
 *  ADC clock 
 *  - The ADC clock is 125000Hz by default.
 *  - You could also set 250000Hz, but that exceeds the max. ADC clock
 *    of 200kHz for 10 bit resolution!
 *  - Special case for 20MHz MCU clock: 156250Hz
 */

#if F_CPU == 2000000
  /* 20MHz MCU clock */
  #define ADC_FREQ    156250
#else
  /* all other MCU clocks */
  #define ADC_FREQ    125000
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

#define ADC_PORT         PORTC     /* port data register */
#define ADC_DDR          DDRC      /* port data direction register */
#define ADC_PIN          PINC      /* port input pins register */
#define TP1              PC0       /* test pin / probe #1 */
#define TP2              PC1       /* test pin / probe #2 */
#define TP3              PC2       /* test pin / probe #3 */

#define TP_ZENER         PC3       /* test pin for for Zener check (10:1 voltage divider) */
#define TP_REF           PC4       /* test pin for 2.5V reference and relay */
#define TP_BAT           PC5       /* test pin for battery (4:1 voltage divider) */
#define TP_LOGIC         PC3       /* test pin for Logic Probe (4:1 voltage divider) */


/*
 *  probe resistors
 *  - For PWM/squarewave output via probe #2 R_RL_2 has to be PB2/OC1B.
 *  - Don't share this port with POWER_CTRL or TEST_BUTTON!
 */

#define R_PORT           PORTB     /* port data register */
#define R_DDR            DDRB      /* port data direction register */
#define R_RL_1           PB0       /* Rl (680R) for test pin #1 */
#define R_RH_1           PB1       /* Rh (470k) for test pin #1 */
#define R_RL_2           PB2       /* Rl (680R) for test pin #2 */
#define R_RH_2           PB3       /* Rh (470k) for test pin #2 */
#define R_RL_3           PB4       /* Rl (680R) for test pin #3 */
#define R_RH_3           PB5       /* Rh (470k) for test pin #3 */


/*
 *  dedicated signal output via OC1B
 *  - don't change this!
 */

#define SIGNAL_PORT      PORTB     /* port data register */
#define SIGNAL_DDR       DDRB      /* port data direction register */
#define SIGNAL_OUT       PB2       /* MCU's OC1B pin */


/*
 *  power control
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define POWER_PORT       PORTD     /* port data register */
#define POWER_DDR        DDRD      /* port data direction register */
#define POWER_CTRL       PD6       /* control pin (1: on / 0: off) */


/*
 *  test push button 
 *  - can't be same port as ADC_PORT or R_PORT
 */

#define BUTTON_PORT      PORTD     /* port data register */
#define BUTTON_DDR       DDRD      /* port data direction register */
#define BUTTON_PIN       PIND      /* port input pins register */
#define TEST_BUTTON      PD7       /* test/start push button (low active) */


/*
 *  rotary encoder
 */

#define ENCODER_PORT     PORTD     /* port data register */
#define ENCODER_DDR      DDRD      /* port data direction register */
#define ENCODER_PIN      PIND      /* port input pins register */
#define ENCODER_A        PD3       /* rotary encoder A signal */
#define ENCODER_B        PD2       /* rotary encoder B signal */


/*
 *  increase/decrease push buttons
 */

#define KEY_PORT         PORTD     /* port data register */
#define KEY_DDR          DDRD      /* port data direction register */
#define KEY_PIN          PIND      /* port input pins register */
#define KEY_INC          PD2       /* increase push button (low active) */
#define KEY_DEC          PD3       /* decrease push button (low active) */


/*
 *  frequency counter
 *  - input must be pin PD4/T0
 */

#define COUNTER_PORT     PORTD     /* port data register */
#define COUNTER_DDR      DDRD      /* port data direction register */
#define COUNTER_IN       PD4       /* signal input T0 */


/*
 *  ring tester
 *  - counter input must be pin PD4/T0 (uses COUNTER_IN)
 */

#define RINGTESTER_PORT  PORTD     /* port data register */
#define RINGTESTER_DDR   DDRD      /* port data direction register */
#define RINGTESTER_OUT   PD5       /* pulse output */



/*
 *  IR detector/decoder
 *  - fixed module connected to dedicated I/O pin
 */

#define IR_PORT          PORTD     /* port data register */
#define IR_DDR           DDRD      /* port data direction register */
#define IR_PIN           PIND      /* port input pins register */
#define IR_DATA          PD5       /* data signal */


/*
 *  TTL serial interface
 *  - hardware USART uses
 *    USART0: Rx PD0 and Tx PD1
 */

/* for hardware RS232 */
#define SERIAL_USART     0         /* use USART0 */
/* for bit-bang RS232 */
#define SERIAL_PORT      PORTD     /* port data register */
#define SERIAL_DDR       DDRD      /* port data direction register */
#define SERIAL_PIN       PIND      /* port input pins register */
#define SERIAL_TX        PD1       /* pin for Tx (transmit) */
#define SERIAL_RX        PD0       /* pin for Rx (receive) */
#define SERIAL_PCINT     16        /* PCINT# for Rx pin */


/*
 *  OneWire
 *  - dedicated I/O pin
 */

#define ONEWIRE_PORT     PORTD     /* port data register */
#define ONEWIRE_DDR      DDRD      /* port data direction register */
#define ONEWIRE_PIN      PIND      /* port input pins register */
#define ONEWIRE_DQ       PD5       /* DQ (data line) */


/*
 *  buzzer
 */

#define BUZZER_PORT      PORTD     /* port data register */
#define BUZZER_DDR       DDRD      /* port data direction register */
#define BUZZER_CTRL      PD5       /* control pin (low: off / high: on) */


/*
 *  MAX6675
 */

#define MAX6675_PORT     PORTD     /* port data register */
#define MAX6675_DDR      DDRD      /* port data direction register */
#define MAX6675_CS       PD5       /* port pin used for /CS */


/*
 *  MAX31855
 */

#define MAX31855_PORT    PORTD     /* port data register */
#define MAX31855_DDR     DDRD      /* port data direction register */
#define MAX31855_CS      PD5       /* port pin used for /CS */


/*
 *  boost converter for Zener check (dedicated I/O pin)
 */

#define BOOST_PORT       PORTD     /* port data register */
#define BOOST_DDR        DDRD      /* port data direction register */
#define BOOST_CTRL       PD5       /* control pin */


/*
 *  flashlight / general purpose switched output
 */

#define FLASHLIGHT_PORT  PORTD     /* port data register */
#define FLASHLIGHT_DDR   DDRD      /* port data direction register */
#define FLASHLIGHT_CTRL  PD5       /* control pin */


/* ************************************************************************
 *   internal stuff
 * ************************************************************************ */

/* ADC reference selection: AVcc */
#define ADC_REF_VCC           (1 << REFS0)

/* ADC reference selection: internal 1.1V bandgap */
#define ADC_REF_BANDGAP       0
#define ADC_REF_BANDGAP2      (1 << REFS2)

/* ADC reference selection: filter mask for register bits */
#define ADC_REF_MASK          ((1 << REFS1) | (1 << REFS0))
#define ADC_REF_MASK2         (1 << REFS2)

/* ADC MUX channel: internal 1.1V bandgap reference */
#define ADC_CHAN_BANDGAP      ((1 << CHMUX3) | (1 << CHMUX2) | (1 << CHMUX1))  /* 4/5 VDO */

/* ADC MUX channel: filter mask for register bits */
#define ADC_CHAN_MASK         ((1 << CHMUX4) | (1 << CHMUX3) | (1 << CHMUX2) | (1 << CHMUX1) | (1 << CHMUX0))


/* ************************************************************************
 *   MCU specific setup to support different AVRs
 * ************************************************************************ */

/*
 *  LGT8F 328P
 */

/* estimated internal resistance of port to GND (in 0.1 Ohms) */
#define R_MCU_LOW           200

/* estimated internal resistance of port to VCC (in 0.1 Ohms) */
#define R_MCU_HIGH          220

/* voltage offset of MCU's analog comparator (in mV): -50 up to 50 */
#define COMPARATOR_OFFSET   0

/* this MCU has 32kB Flash, 1kB EEPROM and 2kB RAM (enable extra features) */
#define RES_FLASH           32
#define RES_EEPROM          4
#define RES_RAM             2


#endif // CONFIG_LGT8F328_H
