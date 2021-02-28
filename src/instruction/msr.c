#include "msr.h"
#include "instruction_list.h"
#include <stdio.h>

uint8_t MSR_instruction_t1(struct raw32_instruction instruction, struct pico_cpu *cpu)
{

    uint32_t Rn = instruction.value_16_24 & 0b1111;
    uint32_t Sysm = instruction.value_0_8;
    union raw_special_instruction rinstruction;
    rinstruction.raw = (Sysm);
    if (Rn == 13 || Rn == 14 || Rn == 15)
    {

        return 1;
    }
    uint32_t *val = get_special_register(Sysm, &cpu->registers);
    if (val == NULL)
    {
        printf("invalid special register %i \n", Sysm);
        return 1;
    }
    uint32_t *from = get_register(Rn, &cpu->registers);
    if (rinstruction.high == 0b00000)
    {
        if ((Sysm & 0b10) == 0)
        {
            cpu->registers.special_reg.APSR = cpu->registers.special_reg.APSR | (*from & (0b11111 >> 27));
        }
    }
    else if (rinstruction.high == 0b00001)
    {
        if (cpu->privileged)
        {
            if ((Sysm & 0b111) == 0b000)
            {

                cpu->registers.special_reg.MSP = *from & ~(0b11);
            }
            else if ((Sysm & 0b111) == 0b001)
            {
                cpu->registers.special_reg.PSP = *from & ~(0b11);
            }
        }
    }
    else if (rinstruction.high == 0b00010)
    {
        if (cpu->privileged)
        {
            printf("no support for readingg/writing for SYSM high 00010 for the moment");
            return -1;
        }
    }
    printf("MSR %s, %s", get_special_register_name(Sysm), get_register_name(Rn));
    return 0;
}