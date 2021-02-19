#include "b.h"
#include "cond.h"
#include <stdio.h>
uint8_t B_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    uint8_t condition = instruction.up & 0b1111;
    uint32_t target_addr = (instruction.down << 1) + 2;

    target_addr += 2;
    int res = is_condition_met(condition, cpu->registers.status);
    if (res == 1)
    {
        cpu->registers.PC += target_addr;
    }
    else if (res < 0)
    {
        printf("invalid B%i instruction opcode \n", condition);
        return 1; // error
    }

    printf("B%i 0x%x = %i \n", condition, target_addr, res);
    return 0;
}