#include "ldr.h"
#include "../pico_cpu.h"
#include "../utils.h"
#include <stdio.h>
uint8_t ldr_litteral_instruction(struct raw_instruction instruction, struct pico_cpu *cpu)
{
    uint8_t target_register = instruction.up & 0b111;
    uint32_t end_value = instruction.down * 4;

    uint32_t base = ALIGN(cpu->registers.PC, 4);
    uint32_t addr = (base + end_value);

    read_memory_dword(cpu, &cpu->registers.R_register[target_register], addr);
    printf("ldr litteral instruction: R%i with offset 0x%x final addr: 0x%x \n", target_register, end_value, addr);

    return 0;
}
uint8_t ldr_immediate_instruction(struct raw_instruction instruction, struct pico_cpu *cpu)
{

    uint16_t destination_register = instruction.down & 0b111;
    uint16_t base_register = (instruction.down & 0b111000) >> 3;
    uint32_t imm32 = (instruction.raw_instruction & 0b11111000000) >> 6;
    imm32 *= 4; // dword aligned
    uint32_t offset_addr = cpu->registers.R_register[base_register] + imm32;
    read_memory_dword(cpu, &cpu->registers.R_register[destination_register], offset_addr);
    ;

    printf("ldr immediate instruction: R%i -> R%i  with offset 0x%x final addr: 0x%x = 0x%x\n", base_register, destination_register, imm32, offset_addr, cpu->registers.R_register[destination_register]);
    return 0;
}