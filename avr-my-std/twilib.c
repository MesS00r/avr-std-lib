#include "twilib.h"

// ***** TWI BASE FUNC *****

void twi_init(uint32_t scl) {
    TWSR = 0;
    TWBR = (((F_CPU / scl) - 16UL) / 2UL);
    if (TWBR > 255) TWBR = 255;

#if DEBUG_MODE_TWI
    UPRINT("F_SCL: %dK\n", (uint16_t)(scl / 1000UL));
    UPRINT("Bit rate: %d\n", TWBR);
#endif
}

twi_code twi_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xf8) != TW_START &&
        (TWSR & 0xf8) != TW_REP_START) {
        return TWI_START_ERR;
#if DEBUG_MODE_TWI
        UPRINT0("TWI START ERROR\n");
#endif
    }
    else {
        return TWI_SUCCESS;
#if DEBUG_MODE_TWI
        UPRINT0("SUCCESS\n");
#endif
    }
}

twi_code twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xf8) != TW_MR_DATA_ACK &&
        (TWSR & 0xf8) != TW_MR_DATA_NACK) {
        return TWI_WRITE_ERR;
#if DEBUG_MODE_TWI
        UPRINT0("TWI WRITE ERROR\n");
#endif
    }
    else {
        return TWI_SUCCESS;
#if DEBUG_MODE_TWI
        UPRINT0("SUCCESS\n");
#endif
    }
}

twi_code twi_write_scl(uint8_t scl) {
    TWDR = scl;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xf8) != TW_MT_SLA_ACK &&
        (TWSR & 0xf8) != TW_MT_SLA_NACK) {
        return TWI_WRITE_SCL_ERR;
#if DEBUG_MODE_TWI
        UPRINT0("TWI WRITE SCL ERROR\n");
#endif
    }
    else {
        return TWI_SUCCESS;
#if DEBUG_MODE_TWI
        UPRINT0("SUCCESS\n");
#endif
    }
}

twi_code twi_read(uint8_t* data, bool ack) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (ack ? (1 << TWEA) : 0);
    while (!(TWCR & (1 << TWINT)));

    if (ack && TW_STATUS != TW_MR_DATA_ACK) {
        return TWI_READ_ACK_ERR;
#if DEBUG_MODE_TWI
        UPRINT0("TWI READ ACK ERROR\n");
#endif
    }
    else if (!ack && TW_STATUS != TW_MR_DATA_NACK) {
        return TWI_READ_NACK_ERR;
#if DEBUG_MODE_TWI
        UPRINT0("TWI READ NACK ERROR\n");
#endif
    }
    else {
        *data = TWDR;
        return TWI_SUCCESS;
#if DEBUG_MODE_TWI
        UPRINT0("SUCCESS\n");
#endif
    }
}

void twi_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (!(TWCR & (1 << TWSTO)));

#if DEBUG_MODE_TWI
    UPRINT0("SUCCESS\n");
#endif
}

// ***** TWI *****

twi_code twi_write_reg(uint8_t addr, uint8_t reg, uint8_t data) {
    twi_code err_code;

    err_code = twi_start();
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write_scl(((addr << 1) | TW_WRITE));
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write(reg);
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write(data);
    if (err_code != TWI_SUCCESS) return err_code;

    twi_stop();

    return TWI_SUCCESS;
#if DEBUG_MODE_TWI
    UPRINT0("WRITE REG SUCCESS\n");
#endif
}

twi_code twi_read_reg(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len) {
    twi_code err_code;

    err_code = twi_start();
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write_scl(((addr << 1) | TW_WRITE));
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write(reg);
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_start();
    if (err_code != TWI_SUCCESS) return err_code;

    err_code = twi_write_scl(((addr << 1) | TW_READ));
    if (err_code != TWI_SUCCESS) return err_code;

    for (uint8_t i = 0; i < len; i++) {
        err_code = twi_read(&data[i], (i < len - 1 ? TWI_ACK : TWI_NACK));
        if (err_code) return err_code;
    }

    twi_stop();

    return TWI_SUCCESS;
#if DEBUG_MODE_TWI
    UPRINT0("READ REG SUCCESS\n");
#endif
}