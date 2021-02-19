#ifndef CMP_H
#define CMP_H
#include "instruction_list.h"
#include <stdint.h>
uint8_t cmp_immediate(struct raw_instruction instruction, struct pico_cpu *cpu);
uint8_t cmp_register_t1(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif