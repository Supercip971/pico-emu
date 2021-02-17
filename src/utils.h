#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <stdint.h>
#define ALIGN(addr, factor) ((addr / factor) * factor)


struct add_with_carry_result {
    uint32_t result;
    uint8_t carry;
    uint8_t overflow;
};

uint32_t add_with_carry(uint32_t x, uint32_t y, bool carry, struct add_with_carry_result* target);


inline int xor(int x, int y){
    return x != y;
}
#endif