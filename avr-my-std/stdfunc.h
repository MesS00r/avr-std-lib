#ifndef STDFUNC_H_INCLUDED
#define STDFUNC_H_INCLUDED

#define DEBUG_MODE 1

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7

#define OC1A 1
#define OC1B 2

#include <avr/io.h>
#include <util/delay.h>
// #include <stdio.h>
#include <stdarg.h>

void usart_init(uint16_t baud);
void usart_print(const char* str, uint8_t count, ...);
uint16_t adc_read(uint8_t a_pin);
void tccr1_init(uint8_t port);
void tccr1_servo_angle(uint8_t angle);



void usart_print_ch(char ch);
void usart_print_str(const char* str);
void usart_print_int(uint8_t num);

#endif //STDFUNC_H_INCLUDED