#ifndef PUSH_H
#define PUSH_H
#include "instruction_list.h"
#include <stdint.h>
uint8_t push_t1_instruction(struct raw_instruction instruction, struct pico_cpu *cpu);
#endif