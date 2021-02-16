#include "ldr.h"
#include <stdio.h>
#include "../utils.h"
#include "../pico_cpu.h"
uint8_t ldr_litteral_instruction(struct raw_instruction instruction, struct pico_cpu* cpu){
    uint8_t target_register = instruction.up & 0b111;
    uint32_t end_value = instruction.down * 4;


    uint32_t base = ALIGN(cpu->registers.PC, 4);
    uint32_t addr = (base + end_value);

    read_memory_dword(cpu,&cpu->registers.R_register[target_register], addr );
    printf("ldr litteral instruction: R%i with offset 0x%x final addr: %x \n", target_register, end_value, addr);
    
    return 0;
}