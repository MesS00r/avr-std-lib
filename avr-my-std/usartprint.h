#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#define DEBUG_MODE 0

#define DEC 0
#define BIN 1
#define HEX 2

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void usart_init(uint16_t baud);

void uprints(const char* str);
void uprint(const char *str, uint16_t num, uint8_t mode);

#endif //USARTPRINT_H_INCLUDED