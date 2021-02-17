#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "pico_cpu.h"
#include <stdint.h>
struct raw_instruction
{
    uint16_t raw_instruction;
    uint8_t up;
    uint8_t down;
};
struct raw32_instruction
{
    uint32_t raw_instruction;
    uint8_t value_24_32;
    uint8_t value_16_24;
    uint8_t value_8_16;
    uint8_t value_0_8;
};
typedef struct raw_instruction raw_instruction_t;

uint8_t run_instruction(struct pico_cpu *cpu);

#endif