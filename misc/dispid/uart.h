#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_ch(char c);
void uart_nl(void);
void uart_str(const char *s);
void uart_print(const char* s);
void uart_hex(uint32_t value, uint8_t digits);
void uart_uint16(uint16_t value);

#endif /* UART_H */
