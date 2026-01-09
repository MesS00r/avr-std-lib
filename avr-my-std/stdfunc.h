#ifndef STDFUNC_H_INCLUDED
#define STDFUNC_H_INCLUDED

#define DEBUG_MODE_OTHER 1

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
#include "usartprint.h"

uint16_t adc_read(uint8_t a_pin);

void timer1_init(uint8_t port);
void timer1_servo_angle(uint8_t angle);

#endif //STDFUNC_H_INCLUDED