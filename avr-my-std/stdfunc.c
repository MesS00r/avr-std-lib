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
        usart_print_ch(*str);
        str++;
    }
}

void usart_print_dec(int16_t num) {
    const char nums_arr[] = "0123456789";
    uint16_t max_dcount = 10000;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

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

void usart_print_bin(int16_t num) {
    const char nums_arr[] = "01";

    if (num > 0) {
        usart_print_str("0b");
    } else if (num < 0) {
        usart_print_str("1b");
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    while (num) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[(num % 2)];
        num /= 2;
    }
}

void usart_sprint(const char* str, int16_t num, uint8_t mode) {
    switch (mode) {
    case DEC: usart_print_str(str); usart_print_dec(num); break;
    case BIN: usart_print_str(str); usart_print_bin(num); break;
    case HEX: break;
    }
}

// void usart_print(const char* str, ...) {
//     // uint16_t scount = 0;
//     va_list args;
//     va_start(args, str);

//     while (*str) {

//         if (*str == '%') {
//             // scount++;
//             // if (*(str + 1) == ) {
                
//             // }

//             switch (*str++) {
//             case 'c': usart_print_ch(va_arg(args, int16_t)); break;
//             case 's': usart_print_str(va_arg(args, const char*)); break;
//             case 'd': usart_print_int(va_arg(args, int16_t)); break;
//             // case 'b': break;
//             // case 'x': break;
//             case '%': usart_print_ch('%'); break;
//             default: continue;
//             }
//         }
//         usart_print_ch(*str);
//         str++;
//     }
//     va_end(args);
// };

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