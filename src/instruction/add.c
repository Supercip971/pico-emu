#include "add.h"
#include "../utils.h"
#include "instruction_list.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

uint8_t adds_immediate_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    struct add_with_carry_result add_result;
    uint16_t Rd = instruction.down & 0b111; // expend to 32bit
    uint16_t Rn = (instruction.down & 0b111000) >> 3;
    uint32_t raw = (instruction.raw_instruction & 0b111000000) >> 6;

    uint32_t *d = get_register(Rd, &cpu->registers);
    uint32_t *n = get_register(Rn, &cpu->registers);

    uint32_t result =
        add_with_carry(*n, raw, false, &add_result);

    *d = result;

    cpu->registers.status.negative_condition = ((int32_t)result) < 0;
    cpu->registers.status.carry_flag = add_result.carry;
    cpu->registers.status.overflow_flag = add_result.overflow;

    cpu->registers.status.zero_condition = (result == 0);

    printf("adds %s, %s, %i \n", get_register_name(Rd), get_register_name(Rn), raw);
    return 0;
}
