#include "tst.h"
#include <stdio.h>
uint8_t tst_t1(struct raw_instruction instruction, struct pico_cpu *cpu){
    uint32_t Rm = (instruction.down & 0b111000) >> 3;
    uint32_t Rn = (instruction.down & 0b111);

    uint32_t *r = get_register(Rm, &cpu->registers);
    uint32_t *n = get_register(Rn, &cpu->registers);
    if (r == NULL || n == NULL)
    {
        printf("invalid tst register for %i & %i", Rm, Rn);
        return 2;
    }
    uint32_t result = *r & *n;
    int signed_r = (int)result;
    cpu->registers.status.negative_condition = ((signed_r < 0));
    cpu->registers.status.zero_condition = result == 0;

    printf("tst %s, %s \n", get_register_name(Rm), get_register_name(Rn));
    return 0;
}