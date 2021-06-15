#ifndef ADD_H
#define ADD_H
#include "instruction_list.h"
#include <stdint.h>

uint8_t adds_immediate_t1(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif
