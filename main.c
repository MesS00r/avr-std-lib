#undef F_CPU
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdfunc.h>

int main(void) {
    usart_init(9600);
    twi_init(TWI_400K);
    
    while (1) {}

    return 0;
}