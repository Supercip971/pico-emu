#ifndef B_H
#define B_H
#include <stdint.h>
#include "instruction_list.h"
// naming a C function just B is not the best idea

uint8_t B_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu);

// not implemented yet
uint8_t B_instruction_t2(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif