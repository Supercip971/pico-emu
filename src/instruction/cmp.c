#include "cmp.h"
#include "../utils.h"

#include <stdio.h>
uint8_t cmp_immediate(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    uint32_t imm32 = (int32_t)instruction.down; // expend to 32bit
    uint8_t base_register = (instruction.up & 0b111);
    uint32_t base_register_value = cpu->registers.R_register[base_register];
    struct add_with_carry_result res = {0, 0, 0};
    cpu->registers.status.negative_condition = imm32 > base_register_value;
    cpu->registers.status.carry_flag = res.carry;
    cpu->registers.status.zero_condition = (res.result == 0);
    cpu->registers.status.overflow_flag = res.overflow;

    printf("cmp R%i, 0x%x \n", base_register, imm32);
    return 0;
}
uint8_t cmp_register_t1(struct raw_instruction instruction, struct pico_cpu *cpu){
    uint8_t Rn = instruction.down & 0b111;
    uint8_t Rm = (instruction.down &0b111000) >> 3;
    uint32_t n = Rn;
    uint32_t m = Rm;
    uint32_t Mv = *get_register(m, &cpu->registers);
    uint32_t Nv = *get_register(n, &cpu->registers);

    struct add_with_carry_result v;

    add_with_carry(Nv, ~(Mv),1 , &v);
    int32_t signed_cond = v.result;
    cpu->registers.status.negative_condition = (signed_cond < 0);
    cpu->registers.status.zero_condition = (v.result == 0);
    cpu->registers.status.carry_flag = (v.carry);
    cpu->registers.status.overflow_flag = (v.overflow);

    printf("cmp %s, %s \n", get_register_name(n), get_register_name(m));
    return 0;


}