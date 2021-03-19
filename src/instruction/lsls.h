#ifndef LSLS_H
#define LSLS_H

#include "instruction_list.h"
#include <stdint.h>

uint8_t lsls_immediate_t1(struct raw_instruction instruction, struct pico_cpu *cpu);
uint8_t lsls_immediate_t2(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif