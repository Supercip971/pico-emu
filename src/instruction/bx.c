#include "bx.h"

#include <stdio.h>
uint8_t BX_instruction_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{
    uint32_t m = (instruction.down & 0b1111000) >> 3;
    if (m == 15)
    {
        return 1;
    }
    cpu->registers.PC = (~(1)) & *get_register(m, &cpu->registers);
    printf("BX PC, %s \n", get_register_name(m));

    return 0;
}
