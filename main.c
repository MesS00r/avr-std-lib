#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdfunc.h>

extern FILE usart_stdout;

int main(void) {
    usart_init(9600);
    stdout = &usart_stdout;

    while (1) {
        uint16_t a0 = adc_read(A0);
        uint16_t a1 = adc_read(A1);

        printf("%d : %d", a0, a1);
        printf("\033[2K\r");
    }
    return 0;
}