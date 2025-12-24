#include "stdfunc.h"

// ****** USART INIT ******

void usart_init(uint16_t baud) {
    uint16_t uart_speed = F_CPU / (16UL * baud) - 1;
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

// #if DEBUG_MODE
//     UPRINT("USART speed: %d\n", uart_speed, DEC);
//     UPRINT("RX mode: %d\n", ((UCSR0B >> RXEN0) % 2), DEC);
//     UPRINT("TX mode: %d\n", ((UCSR0B >> TXEN0) % 2), DEC);
//     UPRINT("UCSR: %d\n", UCSR0C, BIN);
// #endif
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

void usart_print_str(const char* str) {
    while (*str) {
        usart_print_ch(*str);
        str++;
    }
}

// ****** USART PRINT 8 ******

void usart_print_dec_8(int8_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint8_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_bin_8(uint8_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[8] = {0,0,0,0,0,0,0,0};

    usart_print_str("0b");
    if (num == 0) {
        usart_print_str("00000000");
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

    for (uint8_t i = 2; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_8(const char* str, int8_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

// ****** USART PRINT 16 ******

void usart_print_dec_16(int16_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint16_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_bin_16(uint16_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[16];
    for (uint8_t i = 0; i < 16; i++) r_num[i] = 0;

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
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
        if (i % 9 == 0) {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = ' ';
        }
    }
}

void usart_print_hex_16(uint16_t num) {
    const char nums_arr[] = "0123456789ABCDEF";
    uint8_t r_num[4] = {0,0,0,0};

    usart_print_str("0x");
    if (num == 0) {
        usart_print_str("0x0000");
        return;
    }

    for (uint8_t i = 0; i < 4; i++) {
        r_num[i] = num % 16;
        num /= 16;
    }

    for (uint8_t i = 4; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_16(const char* str, int16_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

// ****** USART PRINT 32 ******

void usart_print_dec_32(int32_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint32_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_bin_32(uint32_t num) {
    const char nums_arr[] = "01";
    uint8_t r_num[32];
    for (uint8_t i = 0; i < 32; i++) r_num[i] = 0;

    usart_print_str("0b");
    if (num == 0) {
        usart_print_str("00000000 00000000 00000000 00000000");
        return;
    }

    for (uint8_t i = 0; i < 32; i++) {
        r_num[i] = num % 2;
        num /= 2;
    }

    for (uint8_t i = 32; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
        if (i % 9 == 0) {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = ' ';
        }
    }
}

void usart_print_hex_32(uint32_t num) {
    const char nums_arr[] = "0123456789ABCDEF";
    uint8_t r_num[8] = {0,0,0,0,0,0,0,0};

    usart_print_str("0x");
    if (num == 0) {
        usart_print_str("00000000"); 
        return;
    }

    for (uint8_t i = 0; i < 8; i++) {
        r_num[i] = num % 16;
        num /= 16;
    }

    for (uint8_t i = 8; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_32(const char* str, int32_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_32(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_32(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_32(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

// ****** USART PRINT UNSIGNED ******

void usart_print_dec_8u(uint8_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint8_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_dec_16u(uint16_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num < 0) {
        usart_print_ch('-');
        num = ~num + 1;
    } else if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint16_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_dec_32u(uint32_t num) {
    const char nums_arr[] = "0123456789";
    uint8_t num_len = 0;

    if (num == 0) {
        usart_print_ch('0');
        return;
    }

    uint32_t num_copy = num;
    while (num_copy) {
        num_len++; num_copy /= 10;
    }
    uint8_t r_num[num_len];
    for (uint8_t i = 0; i < num_len; i++) r_num[i] = 0;

    for (uint8_t i = 0; i < num_len; i++) {
        r_num[i] = num % 10;
        num /= 10;
    }

    for (uint8_t i = num_len; i > 0; i--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = nums_arr[r_num[i - 1]];
    }
}

void usart_print_8u(const char* str, uint8_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_8u(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_8(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

void usart_print_16u(const char* str, uint16_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_16u(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_16(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}

void usart_print_32u(const char* str, uint32_t num, uint8_t mode) {
    uint8_t is_print = 0;
    switch (mode) {
    case DEC: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_dec_32u(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case BIN: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_bin_32(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    case HEX: 
        while (*str) {
            if (*str == '%' && *(str + 1) == 'd' && !is_print) {
                is_print = 1; usart_print_hex_32(num);
                str += 2; continue;
            } else usart_print_ch(*str);
            str++;
        } break;
    }
}
