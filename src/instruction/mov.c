#include "mov.h"

#include <stdio.h>
// use the same variable name from the arm documentation 
uint8_t mov_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu){
    uint8_t Rd = instruction.down & 0b111;
    uint8_t Rm = (instruction.down & 0b1111000) >> 3;
    uint8_t D = (instruction.down & 0b10000000) >> 7;
    uint32_t d = (D << 3) | Rd;
    uint32_t m = Rm; 
    uint32_t* result = get_register(m, &cpu->registers);
    uint32_t* target = get_register(d , &cpu->registers);
    if(result == NULL || target == NULL){
        printf("invalid mov register for id %i -> %i", m, d);
        return 2;
    }
    *target = *result;

    printf("mov %s %s \n", get_register_name(d), get_register_name(m));
    return 0;
}

uint8_t mov_immediate(struct raw_instruction instruction, struct pico_cpu *cpu){
    uint32_t d = (instruction.up & 0b111);
    uint32_t imm32 = instruction.down; 
    uint32_t* reg = get_register(d, &cpu->registers);
    if(reg == NULL){

        printf("invalid mov immediate register for id %i\n", d);
        return 2;
    }
    *reg = imm32;

    printf("movs %s, 0x%x \n", get_register_name(d), imm32);

    cpu->registers.status.zero_condition = (imm32 == 0);
    cpu->registers.status.negative_condition = 0; // the manual say tu put it only if the 31th bit of imm32 is set, but the 31th bit is always low... :^(
    
    return 0;

}