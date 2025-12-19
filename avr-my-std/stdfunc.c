#include "stdfunc.h"

// ============================================= USART INIT =============================================

void usart_init(uint16_t baud) {
    uint16_t uart_speed = F_CPU / (16UL * baud) - 1;
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

#if DEBUG_MODE
    UPRINT("USART speed: %d\n", uart_speed, DEC);
    UPRINT("RX mode: %d\n", ((UCSR0B >> RXEN0) % 2), DEC);
    UPRINT("TX mode: %d\n", ((UCSR0B >> TXEN0) % 2), DEC);
    UPRINT("UCSR: %d\n", UCSR0C, BIN);
#endif
}

// ============================================= USART PRINT =============================================

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
    uint16_t r_num = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    while (num) {
        r_num *= 10;
        r_num += num % 10;
        num /= 10;
    }

    while (r_num) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[(r_num % 10)];
        r_num /= 10;
    }
}

void usart_print_bin(uint8_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[8] = {0,0,0,0,0,0,0,0};

    usart_print_str("0b");
    if (num == 0) {
        usart_print_str("0b00000000");
        return;
    }

    for (uint8_t i = 0; i < 8; i++) {
        r_num[i] = num % 2;
        num /= 2;
    }

    for (uint8_t i = 8; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_hex(uint8_t num) {
    const char nums_arr[] = "0123456789ABCDEF";
    uint8_t r_num[2] = {0,0};

    usart_print_str("0x");
    if (num == 0) {
        usart_print_str("0x00");
        return;
    }

    for (uint8_t i = 0; i < 2; i++) {
        r_num[i] = num % 16;
        num /= 16;
    }

    for (uint8_t i = 2; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print(const char* str, int16_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

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