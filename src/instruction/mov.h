#ifndef MOV_H
#define MOV_H

#include "instruction_list.h"
#include <stdint.h>


uint8_t mov_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu);
uint8_t mov_instruction_t2(struct raw_instruction instruction, struct pico_cpu *cpu);
#endif