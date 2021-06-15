#include "b.h"
#include "cond.h"
#include <stdio.h>
uint8_t B_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    uint8_t condition = instruction.up & 0b1111;
    int32_t target_addr = ((int8_t)instruction.down) * 2;

    int res = is_condition_met(condition, cpu->registers.status);
    if (res == 1)
    {
        cpu->registers.PC += target_addr + 2;
        cpu->registers.PC &= (uint32_t) ~(1);
    }
    else if (res < 0)
    {
        printf("invalid B%i instruction opcode \n", condition);
        return 1; // error
    }

    printf("B%i 0x%x = %i ; %x \n", condition, target_addr, res, cpu->registers.PC);
    return 0;
}
