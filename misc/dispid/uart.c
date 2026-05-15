#include <avr/io.h>

#include "common.h"
#include "uart.h"


/*
 * UART
 */

void uart_init(void) {
  UBRR0 = F_CPU / 16 / 19200 - 1;
  UCSR0B = BV(TXEN0) | BV(RXEN0);
  UCSR0C = BV(UCSZ01) | BV(UCSZ00);
}

void uart_ch(char c) {
  while (!ISB(UCSR0A, BV(UDRE0)))
    /* nop */;
  UDR0 = c;
}

void uart_nl(void) {
  uart_ch('\r');
  uart_ch('\n');
}

void uart_str(const char *s) {
  char c;

  while (1) {
    c = *s;
    if (c == '\0')
      break;
    uart_ch(c);
    s++;
  }
}

void uart_print(const char* s) {
  uart_str(s);
  uart_nl();
}

void uart_hex(uint32_t value, uint8_t digits) {
  uint8_t nibble;

  digits <<= 2;
  do {
    digits -= 4;

    nibble = (value >> digits) & 0xF;
    if (nibble < 10)
      nibble += '0';
    else
      nibble += 'A' - 10;

    uart_ch(nibble);
  } while (digits != 0);
}

void uart_uint16(uint16_t value) {
  char buf[5];  /* sizeof("65535")=5 */
  int i = 0;

  do {
    buf[i++] = '0' + (value % 10);
    value /= 10;
  } while (value != 0);

  while (i != 0)
    uart_ch(buf[--i]);
}
