#ifndef BITMATH_H_INCLUDED
#define BITMATH_H_INCLUDED

#include <stdint.h>

uint16_t divu10(uint16_t num);
uint16_t remu10(uint16_t num);

uint16_t divu10d(uint16_t num, uint8_t deg);
uint16_t remu10d(uint16_t num, uint8_t deg);

#define divu2(n) ((n) >> 1)
#define remu2(n) ((n) & 0x01)

uint16_t divu2d(uint16_t num, uint8_t deg);
uint16_t remu2d(uint16_t num, uint8_t deg);

#define divu16(n) ((n) >> 4)
#define remu16(n) ((n) & 0x0f)

uint16_t divu16d(uint16_t num, uint8_t deg);
uint16_t remu16d(uint16_t num, uint8_t deg);

#endif //BITMATH_H_INCLUDED