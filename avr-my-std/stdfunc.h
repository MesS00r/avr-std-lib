#ifndef STDFUNC_H_INCLUDED
#define STDFUNC_H_INCLUDED

#define DEBUG_MODE 0

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7

#define OC1A 1
#define OC1B 2

#define DEC 0
#define BIN 1
#define HEX 2

#define TWI_100K 100000UL
#define TWI_250K 250000UL
#define TWI_400K 400000UL
#define TWI_NACK 0
#define TWI_ACK 1

#include <avr/io.h>
#include <util/delay.h>

uint16_t adc_read(uint8_t a_pin);

void timer1_init(uint8_t port);
void timer1_servo_angle(uint8_t angle);

void usart_init(uint16_t baud);
void usart_print_str(const char* str);

void usart_print_8(const char* str, int8_t num, uint8_t mode);
void usart_print_16(const char* str, int16_t num, uint8_t mode);
void usart_print_32(const char* str, int32_t num, uint8_t mode);

void usart_print_8u(const char* str, uint8_t num, uint8_t mode);
void usart_print_16u(const char* str, uint16_t num, uint8_t mode);
void usart_print_32u(const char* str, uint32_t num, uint8_t mode);

void twi_init(uint32_t scl);
// void twi_start(void);
// void twi_write(uint8_t data);
// uint8_t twi_read(uint8_t ack);
// void twi_stop(void);

typedef enum {
    SUCCESS = 0x00,
    TIMER1_ERR = 0x10,
    USART_ERR = 0x20,
    
    TWI_START_ERR = 0x30,
    TWI_WRITE_ERR = 0x31,
    TWI_WRITE_SCL_ERR = 0x32,
    TWI_READ_ERR = 0x33,
    TWI_STOP_ERR = 0x34,

    // ADC_ERR = 0x40
} r_code;


#define GET_MACRO(_1, _2, _3, NAME, ...) NAME

#define PRINT1A(x) _Generic((x), char*: usart_print_str)(x)
#define PRINT3A8(x, y, z) _Generic((x), char*: usart_print_8)(x, y, z)
#define PRINT3A16(x, y, z) _Generic((x), char*: usart_print_16)(x, y, z)
#define PRINT3A32(x, y, z) _Generic((x), char*: usart_print_32)(x, y, z)

#define PRINT3A8U(x, y, z) _Generic((x), char*: usart_print_8u)(x, y, z)
#define PRINT3A16U(x, y, z) _Generic((x), char*: usart_print_16u)(x, y, z)
#define PRINT3A32U(x, y, z) _Generic((x), char*: usart_print_32u)(x, y, z)

#define UPRINT8(...) GET_MACRO(__VA_ARGS__, PRINT3A8, , PRINT1A) (__VA_ARGS__)
#define UPRINT16(...) GET_MACRO(__VA_ARGS__, PRINT3A16, , PRINT1A) (__VA_ARGS__)
#define UPRINT32(...) GET_MACRO(__VA_ARGS__, PRINT3A32, , PRINT1A) (__VA_ARGS__)

#define UPRINT8U(...) GET_MACRO(__VA_ARGS__, PRINT3A8U, , PRINT1A) (__VA_ARGS__)
#define UPRINT16U(...) GET_MACRO(__VA_ARGS__, PRINT3A16U, , PRINT1A) (__VA_ARGS__)
#define UPRINT32U(...) GET_MACRO(__VA_ARGS__, PRINT3A32U, , PRINT1A) (__VA_ARGS__)

#endif //STDFUNC_H_INCLUDED