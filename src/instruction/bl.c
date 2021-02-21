#include "bl.h"
#include "../utils.h"
#include <stdio.h>
uint8_t BL_instruction_t1(struct raw32_instruction instruction, struct pico_cpu *cpu)
{
    uint32_t j1 = ((instruction.raw_instruction) & (0b10000000000000)) != 0;

    uint32_t j2 = ((instruction.raw_instruction) & (0b100000000000)) != 0;
    uint32_t imm11 = instruction.raw_instruction & 0b11111111111;
    uint32_t imm10 = (instruction.raw_instruction >> 16) & (0b1111111111);
    ;
    uint32_t s = ((instruction.raw_instruction >> 25) & 1);
    uint32_t i1 = !xor(s, j1);
    uint32_t i2 = !xor(s, j2);
    uint32_t rs = 0;
    if (s)
    {
        rs = 0b11111111;
    }
    int32_t target_addr = (((uint32_t)rs) << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1);

    int32_t res = (uint32_t)cpu->registers.PC + (int32_t)target_addr+2;
    printf("BL 0x%x \n", res);
    cpu->registers.LR = cpu->registers.PC;
    cpu->registers.PC = res;
    return 0;
}