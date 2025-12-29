#include "stdfunc.h"

// ***** ANALOG READ *****

uint16_t adc_read(uint8_t a_pin) {
    a_pin &= 0x7;
    ADMUX = a_pin | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADIF);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADIF);

    return ADC;
}

// ***** TIMER FOR SERVO *****

void timer1_init(uint8_t port) {
    if (port > OC1B) port = OC1A;
    DDRB = (1 << port);
    TCCR1A = (1 << WGM11) | (port == OC1A ? (1 << COM1A1) : 0) | (port == OC1B ? (1 << COM1B1) : 0);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 40000;

#if DEBUG_MODE_OTHER
    UPRINT("OC1A mode: %d\n", ((TCCR1A >> COM1A1) % 2));
    UPRINT("OC1B mode: %d\n", ((TCCR1A >> COM1B1) % 2));
    UPRINT("ICR: %d\n", ICR1);
#endif
}

void timer1_servo_angle(uint8_t angle) {
    if (angle > 180) angle = 180;
    uint16_t us = 500 + ((uint32_t)angle * 2000) / 180;
    OCR1A = (uint16_t)((uint32_t)us * (F_CPU / 8) / 1000000UL);
}