#include "sub.h"
#include "../utils.h"

#include <stdio.h>
uint8_t sub_immediate_t2(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    uint32_t imm32 = (uint32_t)instruction.down;
    uint8_t d = instruction.up & 0b111;
    uint8_t n = instruction.up & 0b111;
    
    uint32_t *rd = get_register(d, &cpu->registers);
    uint32_t *rn = get_register(n, &cpu->registers);

    if (rd == NULL || rn == NULL)
    {
        printf("invalid sub register for id %i -> %i", d, n);
        return 2;
    }

    struct add_with_carry_result v;
    
    add_with_carry(*rn, ~(imm32), 1, &v);
    *rd = v.result;

    cpu->registers.status.negative_condition = (((int32_t)v.result) < 0);
    cpu->registers.status.zero_condition = (v.result == 0);
    cpu->registers.status.carry_flag = (v.carry);
    cpu->registers.status.overflow_flag = (v.overflow);

    printf("subs %s, #%i \n", get_register_name(d), imm32);

    return 0;
}