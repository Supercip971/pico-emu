#ifndef BL_H
#define BL_H
#include "instruction_list.h"
#include <stdint.h>
// naming a C function just B is not the best idea

uint8_t BL_instruction_t1(struct raw32_instruction instruction, struct pico_cpu *cpu);

#endif