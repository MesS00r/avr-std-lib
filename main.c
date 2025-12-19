#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdfunc.h>

#undef F_CPU
#define F_CPU 16000000UL
#undef DEBUG_MODE
#define DEBUG_MODE 1

int main(void) {
    usart_init(9600);

    // UPRINT("NUM: %d\n", 6, HEX);

    while (1) {
        uint16_t a0 = adc_read(A0);
        uint16_t a1 = adc_read(A1);

        // UPRINT("%d : ", a0, DEC);
        // UPRINT("%d\n", a1, DEC);
        // UPRINT("\033[2K\r\n");
    }
    return 0;
}