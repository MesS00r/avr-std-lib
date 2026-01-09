#include "usartprint.h"

// ****** USART INIT ******

void usart_init(const uint16_t baud) {
    uint16_t uart_speed = (uint16_t)(F_CPU / (16UL * baud) - 1UL);
    UBRR0H = (uint8_t)(uart_speed >> 8);
    UBRR0L = (uint8_t)uart_speed;

    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

#if DEBUG_MODE_USART
    UPRINT("Baud: %d\n", baud);
    UPRINT("USART speed: %d\n", uart_speed);
    UPRINT("RX mode: %d\n", ((UCSR0B >> RXEN0) % 2));
    UPRINT("TX mode: %d\n", ((UCSR0B >> TXEN0) % 2));
    UPRINT("UCSR: %b\n", UCSR0C);
#endif
}

// ****** USART PRINT STR ******

static inline void usart_print_ch(const char ch) {
    if (ch == '\n') {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '\r';
    }
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 =  ch;
}

static inline void usart_print_str(const char *str) {
    char ch = 0;
    while ((ch = pgm_read_byte(str))) {
        usart_print_ch(ch);
        str++;
    }
}

// ****** USART PRINT DEC ******

static inline void usart_print_dec(const uint16_t num) {
    for (uint8_t deg = 4; deg > 0; deg--) {
        uint16_t divnum = divu10d(num, deg - 1);
        uint16_t rem = remu10(divnum);

        if (divnum == 0) continue;
        else usart_print_ch(('0' + rem));
    }
}

// ****** USART PRINT BIN ******

static inline void usart_print_bin(const uint16_t num) {
    uint8_t deg = num > 0xff ? 16 : 8;

    usart_print_str(PSTR("0b"));
    for (; deg > 0; deg--) {
        uint16_t divnum = divu2d(num, deg - 1);
        uint16_t rem = remu2(divnum);

        usart_print_ch(('0' + rem));
        if (deg == 9) usart_print_ch(' ');
    }
}
// ****** USART PRINT HEX ******

static inline uint16_t hex_mask(const uint8_t num) {
    return num > 9 ? ('A' + (num - 10)) : ('0' + num);
}

static inline void usart_print_hex(const uint16_t num) {
    uint8_t deg = num > 0xff ? 4 : 2;

    usart_print_str(PSTR("0x"));
    for (; deg > 0; deg--) {
        uint16_t divnum = divu16d(num, deg - 1);
        uint16_t rem = remu16(divnum);
        char hex_num = hex_mask(rem);

        usart_print_ch(hex_num);
    }
}

// ****** USART PRINT ******

void uprint(const char *fmt, const uint8_t len, const uint16_t *args) {
    uint8_t index = 0;
    char ch0 = 0, ch1 = 0;

    while ((ch0 = pgm_read_byte(fmt))) {
        fmt++;

        if (ch0 == '%' && index < len) {
            ch1 = pgm_read_byte(fmt);
            fmt++;

            switch (ch1) {
            case 'D':
            case 'd':
                usart_print_dec(args[index]);
                break;
            case 'b':
            case 'B':
                usart_print_bin(args[index]);
                break;
            case 'x':
            case 'X':
                usart_print_hex(args[index]);
                break;
            default:
                usart_print_ch('%');
                usart_print_ch(ch1);
                break;
            }

            index++;
        } else {
            usart_print_ch(ch0);
        }
    }
}