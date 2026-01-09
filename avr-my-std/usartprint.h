#ifndef USARTPRINT_H_INCLUDED
#define USARTPRINT_H_INCLUDED

#define DEBUG_MODE_USART 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "bitmath.h"

void usart_init(uint16_t baud);
void uprint(const char *fmt, const uint8_t len, const uint16_t *args);

#define ARGS_COUNT(...) ARGS_COUNT_(__VA_ARGS__, 5, 4, 3 ,2 ,1)
#define ARGS_COUNT_(_1, _2, _3, _4, _5, N, ...) N

#define UPRINT(fmt, ...) \
    _Static_assert(sizeof((uint16_t[]){__VA_ARGS__}) > 0, \
        "UPRINT requires at least one argument."); \
    uprint((fmt), ARGS_COUNT(__VA_ARGS__), (uint16_t[]){__VA_ARGS__})

#define UPRINT0(fmt) uprint(fmt, 0, (void*)0)

#endif //USARTPRINT_H_INCLUDED