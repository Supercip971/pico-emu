#ifndef SUB_H
#define SUB_H

#include "instruction_list.h"
#include <stdint.h>

uint8_t sub_immediate_t2(struct raw_instruction instruction, struct pico_cpu *cpu);

#endif