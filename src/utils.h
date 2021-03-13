#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define ALIGN(addr, factor) ((addr) - (addr % factor))

struct add_with_carry_result
{
    uint32_t result;
    uint8_t carry;
    uint8_t overflow;
};

uint32_t add_with_carry(uint32_t x, uint32_t y, bool carry, struct add_with_carry_result *target);

inline int xor (int x, int y) {
    return x != y;
}

inline uint32_t bit_get(uint32_t base_value, uint32_t offset) {
	return (base_value >> offset) & 1;
}

enum SRType{
    SRType_LSL = 0,
    SRType_LSR = 1,
    SRType_ASR = 2,
    SRType_ROR = 3,
    SRType_RRX = 4,
        
};
struct shift_c_result {
    uint32_t result;
    bool carry;
};
uint32_t lsl_c(uint32_t value, int amount, struct shift_c_result* res);
uint32_t lsr_c(uint32_t value, int amount, struct shift_c_result* res);
uint32_t asr_c(uint32_t value, int amount, struct shift_c_result* res);
uint32_t ror_c(uint32_t value, int amount, struct shift_c_result* res);
uint32_t rrx_c(uint32_t value, bool carry_in, struct shift_c_result* res);

uint32_t shift_c(uint32_t value, int shift_type, int amount, bool carry_in, struct shift_c_result* res);
#endif