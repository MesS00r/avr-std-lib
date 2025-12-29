#ifndef TWILIB_H_INCLUDED
#define TWILIB_H_INCLUDED

#define DEBUG_MODE_TWI 0

#define TWI_100K 100000UL
#define TWI_250K 250000UL
#define TWI_400K 400000UL
#define TWI_NACK 0
#define TWI_ACK 1

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdbool.h>
#include "usartprint.h"

typedef enum {
    TWI_SUCCESS = 0x00,
    TWI_START_ERR = 0x01,
    TWI_WRITE_ERR = 0x02,
    TWI_WRITE_SCL_ERR = 0x03,
    TWI_READ_ACK_ERR = 0x04,
    TWI_READ_NACK_ERR = 0x05,
} twi_code;

void twi_init(uint32_t scl);
twi_code twi_read_reg(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t len);
twi_code twi_write_reg(uint8_t addr, uint8_t reg, uint8_t data);

#endif //TWILIB_H_INCLUDED