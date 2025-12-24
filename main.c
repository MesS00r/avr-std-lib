#undef F_CPU
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdfunc.h>

int main(void) {
    usart_init(9600);
    twi_init(TWI_400K);

    UPRINT16U("UPRINT16U: %d\n", 5423, BIN);
    UPRINT32U("UPRINT32U: %d\n", 5423, BIN);

    UPRINT16U("UPRINT16U: %d\n", 5423, HEX);
    UPRINT32U("UPRINT32U: %d\n", 5423, HEX);

    UPRINT8U("UPRINT8U: %d\n", 0, BIN);
    UPRINT16U("UPRINT16U: %d\n", 0, BIN);
    UPRINT32U("UPRINT32U: %d\n", 0, BIN);

    UPRINT8("UPRINT8: %d\n", 0, BIN);
    UPRINT16("UPRINT16: %d\n", 0, BIN);
    UPRINT32("UPRINT32: %d\n", 0, BIN);

    // UPRINT16U("SSS: %d\n", 5423, HEX);
    // UPRINT32U("SSS: %d\n", 5423, HEX);

    while (1) {}

    return 0;
}