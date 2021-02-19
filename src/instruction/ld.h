#ifndef LDR_H
#define LDR_H
#include "instruction_list.h"
#include <stdint.h>
uint8_t ldr_litteral_instruction(struct raw_instruction instruction, struct pico_cpu *cpu);
uint8_t ldr_immediate_instruction(struct raw_instruction instruction, struct pico_cpu *cpu);
uint8_t ldm_t1_instruction(struct raw_instruction instruction, struct pico_cpu *cpu);
#endif