#include "instruction.h"
#include <stdio.h>
#include "instruction/instruction_list.h"

uint8_t run_instruction(struct pico_cpu *cpu)
{
    uint8_t first = fetch_byte(cpu);
    uint8_t second = fetch_byte(cpu);
    uint16_t instruction = first | (second << 8);
    struct raw_instruction raw_instruction;
    raw_instruction.up = second;
    raw_instruction.down = first;
    raw_instruction.raw_instruction = instruction;

    if((second & 0b11111000) == 0b01001000){ // LDR (literal) instruction
        return ldr_litteral_instruction(raw_instruction, cpu);
    }    
    printf("invalid instruction %x at %x \n ", instruction, cpu->registers.PC-2);
    return 0;
}
