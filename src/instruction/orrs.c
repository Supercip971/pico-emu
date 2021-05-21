#include "orrs.h"
#include "instruction_list.h"
#include <stdio.h>
#include "../utils.h"

uint8_t ORRS_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{
    struct shift_c_result res = {0, 0};
    uint16_t Rdn = instruction.down & 0b111; // expend to 32bit
    uint16_t Rm = (instruction.down & 0b111000) >> 3;

    uint32_t * mr = get_register(Rm, &cpu->registers);
    uint32_t * nr = get_register(Rdn, &cpu->registers);


    uint32_t shifted = shift_c(*mr, SRType_LSL, 0, cpu->registers.status.carry_flag, &res);

    *nr = *nr | shifted;
    cpu->registers.status.negative_condition = ((int32_t)shifted) < 0;

    cpu->registers.status.carry_flag = res.carry;
    cpu->registers.status.zero_condition = (res.result == 0);


    printf("orrs R%s, Rm%s \n", get_register_name(Rdn), get_register_name(Rm));
    return 0;
}