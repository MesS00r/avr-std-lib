#include "usartprint.h"

// ****** USART INIT ******

void usart_init(uint16_t baud) {
    uint16_t uart_speed = (uint16_t)(F_CPU / (16UL * baud) - 1UL);
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

#if DEBUG_MODE
    UPRINT16U("Baud: %d\n", baud, DEC);
    UPRINT16U("USART speed: %d\n", uart_speed, DEC);
    UPRINT8U("RX mode: %d\n", ((UCSR0B >> RXEN0) % 2), DEC);
    UPRINT8U("TX mode: %d\n", ((UCSR0B >> TXEN0) % 2), DEC);
    UPRINT8U("UCSR: %d\n", UCSR0C, BIN);
#endif
}

// ****** USART PRINT STR ******

void usart_print_ch(char ch) {
    if (ch == '\n') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 =  ch;
}

void usart_print_str(const char *str) {
    while (*str) {
        usart_print_ch(*str);
        str++;
    }
}

// ****** USART PRINT DEC ******

void usart_print_dec_8(uint8_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t highor_d = 100;

    if (num == 0) {
        usart_print_ch('0');
        return;
    }

    while (highor_d) {
        if ((num / highor_d) == 0) {
            highor_d /= 10; continue;
        }
        usart_print_ch(nums_arr[((num / highor_d) % 10)]);
        highor_d /= 10;
    }
}

void usart_print_dec_16(uint16_t num) {
    const char nums_arr[] = "0123456789";
    uint16_t highor_d = 10000;

    if (num == 0) {
        usart_print_ch('0');
        return;
    }

    while (highor_d) {
        if ((num / highor_d) == 0) {
            highor_d /= 10; continue;
        }
        usart_print_ch(nums_arr[((num / highor_d) % 10)]);
        highor_d /= 10;
    }
}

// ****** USART PRINT BIN ******

void usart_print_bin_8(uint8_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[8] = {0,0,0,0, 0,0,0,0};

    usart_print_str("0b");
    if (num == 0) {
        usart_print_str("00000000");
        return;
    }

    for (uint8_t i = 0; i < 8; i++) {
        r_num[i] = num % 2;
        num /= 2;
    }

    for (uint8_t i = 8; i > 0; i--)
        usart_print_ch(nums_arr[r_num[i - 1]]);
}

void usart_print_bin_16(uint16_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

    usart_print_str("0b");
    if (num == 0) {
        usart_print_str("00000000 00000000");
        return;
    }

    for (uint8_t i = 0; i < 16; i++) {
        r_num[i] = num % 2;
        num /= 2;
    }

    for (uint8_t i = 16; i > 0; i--) {
        usart_print_ch(nums_arr[r_num[i - 1]]);
        if (i % 9 == 0) usart_print_ch(' ');
    }
}

// ****** USART PRINT HEX ******

void usart_print_hex_8(uint8_t num) {
    const char nums_arr[] = "0123456789ABCDEF";
    uint8_t r_num[2] = {0,0};

    usart_print_str("0x");
    if (num == 0) {
        usart_print_str("00");
        return;
    }

    for (uint8_t i = 0; i < 2; i++) {
        r_num[i] = num % 16;
        num /= 16;
    }

    for (uint8_t i = 2; i > 0; i--)
        usart_print_ch(nums_arr[r_num[i - 1]]);
}

void usart_print_hex_16(uint16_t num) {
    const char nums_arr[] = "0123456789ABCDEF";
    uint8_t r_num[4] = {0,0,0,0};

    usart_print_str("0x");
    if (num == 0) {
        usart_print_str("0000");
        return;
    }

    for (uint8_t i = 0; i < 4; i++) {
        r_num[i] = num % 16;
        num /= 16;
    }

    for (uint8_t i = 4; i > 0; i--)
        usart_print_ch(nums_arr[r_num[i - 1]]);
}

// ****** USART PRINT ******

void usart_print_8(const char *str, uint8_t num, uint8_t mode) {
    bool is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_dec_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_bin_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_hex_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

void usart_print_16(const char *str, uint16_t num, uint8_t mode) {
    bool is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_dec_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_bin_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && (*(str + 1) == 'd' || *(str + 1) == 'D') && !is_print) {
                is_print = 1; usart_print_hex_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

void uprint(const char *str, uint16_t num, uint8_t mode) {
    if (num > 0xff) usart_print_16(str, num, mode);
    else usart_print_8(str, num, mode);
}

void uprints(const char* str) {
    usart_print_str(str);
}

// void uprint(const char *str, ...) {
//     va_list args;
//     va_start(args, 3);
//
//     uint16_t num = 0;
//     int8_t mode = -1;
//
//     num = va_arg(args, int);
//     mode = va_arg(args, int);
//
//     if (num == 0 && mode == -1) usart_print_str(str);
//     else if (num <= 0xff) usart_print_8(str, num, mode);
//     else usart_print_16(str, num, mode);
// }