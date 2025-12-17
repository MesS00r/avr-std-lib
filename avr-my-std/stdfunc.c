#include "stdfunc.h"

// ============================================= USART INIT =============================================
// int usart_putch(char ch, FILE* stream);
// FILE usart_stdout = FDEV_SETUP_STREAM(usart_putch, NULL, _FDEV_SETUP_WRITE);

void usart_init(uint16_t baud) {
    uint16_t uart_speed = F_CPU / (16UL * baud) - 1;
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

// #ifndef DEBUG_MODE
//     printf("USART speed: %d\n", uart_speed);
//     printf("RX mode: %d\n", ((UCSR0B >> RXEN0) % 2));
//     printf("TX mode: %d\n", ((UCSR0B >> TXEN0) % 2));
//     printf("Char size: %X\n", UCSR0C);
// #endif
}

// int usart_putch(char ch, FILE* stream) {
//     // if (ch == '\n') {
//     //     while (!(UCSR0A & (1 << UDRE0)));
//     //     UDR0 = '\r';
//     // }
//     while (!(UCSR0A & (1 << UDRE0)));
//     UDR0 =  ch;
//     return 0;
// }

void usart_print_ch(char ch) {
    if (ch == '\n') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 =  ch;
}

void usart_print_str(const char* str) {
    while (*str) {
        if (*str == '\n') {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = '\r';
        }
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 =  *str;
        str++;
    }
}

void usart_print_int(uint8_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t max_dcount = 100;

    while (num) {
        if (num / max_dcount == 0) {
            max_dcount /=10;
            continue;
        }

        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[(num / max_dcount)];
        num %= max_dcount;
    }
}

void usart_print(const char* str, uint8_t count, ...) {
    uint16_t scount = 0;
    uint16_t ccount = 0;
    va_list args;
    va_start(args, count);

    while (*str) {
        ccount++;
        if (ccount == count) {
            va_end(args);
            va_start(args, count);
        }

        if (*str == '%') {
            scount++;
            if (scount > count) {
                usart_print_ch('\0'); str += 2;
                continue;
            }

            switch (*(str + 1)) {
            case 'c': usart_print_ch(); break;
            case 's': usart_print_str(); break;
            case 'd': usart_print_int(); break;
            // case 'b': break;
            // case 'x': break;
            }
            str += 2;
        }
        usart_print_ch(*str);
        str++;
    }
    va_end(args);
};

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