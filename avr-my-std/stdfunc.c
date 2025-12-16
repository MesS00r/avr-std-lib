#include "stdfunc.h"

// ============================================= USART INIT =============================================

void usart_init(uint16_t baud) {
    uint16_t uart_speed = F_CPU / (16UL * baud) - 1;
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

int usart_putch(char ch, FILE* stream) {
    if (ch == '\n') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 =  ch;
    return 0;
}

FILE usart_stdout = FDEV_SETUP_STREAM(usart_putch, NULL, _FDEV_SETUP_WRITE);

// void usart_print_ch(char ch) {
//     if (ch == '\n') {
//         while (!(UCSR0A & (1 << UDRE0)));
//         UDR0 = '\r';
//     }
//     while (!(UCSR0A & (1 << UDRE0)));
//     UDR0 =  ch;
// }

// void usart_print(const char* str, ...) {};

// ============================================= ANALOG_READ =============================================

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

// ============================================= TIMER FOR SERVO =============================================

void tccr1_init(uint8_t port) {
    if (port > 2) port = 1;
    DDRB = (1 << port);
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 40000;
}

void tccr1_servo_angle(uint8_t angle) {
    if (angle > 180) angle = 180;
    uint16_t us = 500 + ((uint32_t)angle * 2000) / 180;
    OCR1A = (uint16_t)((uint32_t)us * (F_CPU / 8) / 1000000UL);
}