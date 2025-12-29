#undef F_CPU
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <usartprint.h>
#include <twilib.h>

int main(void) {
    usart_init(9600);
    // twi_init(TWI_400K);
    uprint("Sss: %d\n", 11, DEC);

    // usart_print("sss: %d\n", 11, DEC);
    // uprint("SSS: %d\n");

    while (1) {}

    return 0;
}