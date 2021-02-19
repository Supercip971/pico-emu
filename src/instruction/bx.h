#ifndef BX_H
#define BX_H
#include "instruction_list.h"
#include <stdint.h>

uint8_t BX_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif