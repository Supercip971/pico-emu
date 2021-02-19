#include "str.h"

#include <stdio.h>
uint8_t str_immediate_t1(struct raw_instruction instruction, struct pico_cpu *cpu)
{
    uint32_t t = instruction.down & 0b111;
    uint32_t n = (instruction.down & 0b111000) >> 3;
    uint32_t imm5 = (instruction.raw_instruction & 0b11111000000) >> 6;
    uint32_t imm32 = imm5 << 2;

    uint32_t *rt = get_register(t, &cpu->registers);
    uint32_t *rn = get_register(n, &cpu->registers);

    if (rt == NULL || rn == NULL)
    {
        printf("invalid str register for id %i -> %i", t, n);
        return 2;
    }

    uint32_t offset = *rn + imm32;

    int result = write_memory_dword(cpu, *rt, offset);
    printf("str %s, [%s, #%x] \n", get_register_name(t), get_register_name(n), imm32);

    if (result != WRITE_MEMORY_SUCCESS)
    {

        printf("invalid str write for id %x -> %x", *rt, *rn);
        return 3;
    }
    return 0;
}