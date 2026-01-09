#undef F_CPU
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <usartprint.h>
#include <twilib.h>

int main(void) {
    usart_init(9600);
    twi_init(TWI_400K);

    UPRINT0("Hello world!!!\n");
    UPRINT("NUMS: %d, %B, %x\n", 400, 23, 8932);

    while (1) {}

    return 0;
}