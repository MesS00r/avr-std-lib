#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#define DEBUG_MODE_USART 0
// #define NULL ((void*)0)

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bitmath.h"

void usart_init(uint16_t baud);
void uprint(const char *fmt, const uint8_t len, const uint16_t *args);

#define ARG_COUNT(...) ARG_COUNT_(__VA_ARGS__, 5, 4, 3 ,2 ,1)
#define ARG_COUNT_(_1, _2, _3, _4, _5, N, ...) N

#define UPRINT(fmt, ...) _Generic((fmt), \
    char*: uprint \
) ((fmt), ARG_COUNT(__VA_ARGS__), (uint16_t[]){__VA_ARGS__})

#endif //USARTPRINT_H_INCLUDED