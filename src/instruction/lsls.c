#include "lsls.h"
#include "../utils.h"
#include <stdint.h>

#include "utils.h"
#include <stdio.h>

uint8_t lsls_immediate_t1(struct raw_instruction instruction, struct pico_cpu *cpu){
    
    uint32_t imm5 = (instruction.raw_instruction & 0b11111000000) >> 6;
    if(imm5 == 0)
    {
        return mov_instruction_t2(instruction, cpu);
    }
    uint8_t Rd = instruction.down & 0b111;
    uint8_t Rm = (instruction.down & 0b111000) >> 3;
    uint32_t d = Rd;
    uint32_t m = Rm;
    uint32_t *from = get_register(m, &cpu->registers);
    uint32_t *to = get_register(d, &cpu->registers);
    if (from == NULL || to == NULL)
    {
        printf("invalid lsls register for id %i -> %i", m, d);
        return 2;
    }
    struct shift_c_result res;
    shift_c(*from, SRType_LSL, imm5, cpu->registers.status.carry_flag, &res);

    
    cpu->registers.status.negative_condition = ((int32_t)res.result < 0);
    cpu->registers.status.carry_flag = res.carry;
    cpu->registers.status.zero_condition = (res.result == 0);
    *to = res.result;
    printf("lsls %s, %s, #%i \n", get_register_name(d), get_register_name(m), imm5);
    return 0;
}
