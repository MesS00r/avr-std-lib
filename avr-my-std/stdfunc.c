#include "stdfunc.h"

// ***** TWI BASE FUNC *****

void twi_init(uint32_t scl) {
    TWSR = 0;
    TWBR = (((F_CPU / scl) - 16UL) / 2UL);

#if DEBUG_MODE
    UPRINT16U("F_SCL: %dK\n", (uint16_t)(scl / 1000UL), DEC);
    UPRINT8U("Bit rate: %d\n", TWBR, DEC);
#endif
}

r_code twi_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if (!(TWSR & 0xf8)) {
        return TWI_START_ERR;
#if DEBUG_MODE
        UPRINT8U("TWI START ERROR");
#endif
    }
    else {
        return SUCCESS;
#if DEBUG_MODE
        UPRINT8U("SUCCESS");
#endif
    }
}

r_code twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if (!(TWSR & 0xf8)) {
        return TWI_WRITE_ERR;
#if DEBUG_MODE
        UPRINT8U("TWI WRITE ERROR");
#endif
    }
    else {
        return SUCCESS;
#if DEBUG_MODE
        UPRINT8U("SUCCESS");
#endif
    }
}

r_code twi_write_scl(uint8_t scl) {
    TWDR = scl;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if (!(TWSR & 0xf8)) {
        return TWI_WRITE_SCL_ERR;
#if DEBUG_MODE
        UPRINT8U("TWI WRITE SCL ERROR");
#endif
    }
    else {
        return SUCCESS;
#if DEBUG_MODE
        UPRINT8U("SUCCESS");
#endif
    }
}

uint8_t twi_read(uint8_t ack) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
    while (!(TWCR & (1 << TWINT)));

    if (!(TWSR & 0xf8)) {
        return TWI_READ_ERR;
#if DEBUG_MODE
        UPRINT8U("TWI STOP ERROR");
#endif
    }
    else {
        return TWDR;
#if DEBUG_MODE
        UPRINT8U("SUCCESS");
#endif
    }
}

r_code twi_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if (!(TWSR & 0xf8)) {
        return TWI_STOP_ERR;
#if DEBUG_MODE
        UPRINT8U("TWI STOP ERROR");
#endif
    }
    else {
        return SUCCESS;
#if DEBUG_MODE
        UPRINT8U("SUCCESS");
#endif
    }
}

// ***** TWI *****

r_code twi_write_reg(uint8_t addr, uint8_t data) {
    r_code err_code;

    err_code = twi_start();
    if (err_code != SUCCESS) return err_code;

    err_code = twi_write_scl(addr);
    if (err_code != SUCCESS) return err_code;

    err_code = twi_start();
    if (err_code != SUCCESS) return err_code;

    err_code = twi_write(data);
    if (err_code != SUCCESS) return err_code;

    return SUCCESS;
#if DEBUG_MODE
    UPRINT8("WRITE REG SUCCESS");
#endif
}

// ***** ANALOG READ *****

uint16_t adc_read(uint8_t a_pin) {
    a_pin &= 0x7;
    ADMUX = a_pin | (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADIF);

    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADIF);

    return ADC;
}

// ***** TIMER FOR SERVO *****

void timer1_init(uint8_t port) {
    if (port > OC1B) port = OC1A;
    DDRB = (1 << port);
    TCCR1A = (1 << WGM11) | (port == OC1A ? (1 << COM1A1) : 0) | (port == OC1B ? (1 << COM1B1) : 0);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 40000;

#if DEBUG_MODE
    UPRINT8U("OC1A mode: %d\n", ((TCCR1A >> COM1A1) % 2), DEC);
    UPRINT8U("OC1B mode: %d\n", ((TCCR1A >> COM1B1) % 2), DEC);
    UPRINT16U("ICR: %d\n", ICR1, DEC);
#endif
}

void timer1_servo_angle(uint8_t angle) {
    if (angle > 180) angle = 180;
    uint16_t us = 500 + ((uint32_t)angle * 2000) / 180;
    OCR1A = (uint16_t)((uint32_t)us * (F_CPU / 8) / 1000000UL);
}