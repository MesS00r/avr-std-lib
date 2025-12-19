#define F_CPU 16000000UL
#define DEBUG_MODE 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdfunc.h>

// extern FILE usart_stdout;

int main(void) {
    usart_init(9600);
    // stdout = &usart_stdout;

    usart_sprint("NUM : ", 4, BIN);
    // usart_print_ch('s');
    // usart_print_int(22);
    // usart_print_str("sss\n");

    while (1) {
        uint16_t a0 = adc_read(A0);
        uint16_t a1 = adc_read(A1);

        // printf("%d : %d", a0, a1);
        // printf("\033[2K\r\n");
    }
    return 0;
}