#include "instruction.h"

#include "instruction/instruction_list.h"
#include <stdio.h>
// run Special data instruction and Branch and Exchange 
uint8_t run_SdiBE_instruction(struct pico_cpu *cpu, struct raw_instruction instruction){
    uint8_t raw_opcode = (instruction.raw_instruction & 0b1111000000) >> 6;
    if((raw_opcode & 0b1100) == 0b1000){
        return mov_instruction_t1(instruction, cpu);
    }else if((raw_opcode & 0b1110) == 0b1100){
        // branch and Exchange
    }else if((raw_opcode & 0b1110) == 0b1110){
        // branch with link and Exchange
    }else if((raw_opcode & 0b1100) == 0b0000){
        // add
    }else if((raw_opcode & 0b1111) == 0b0101){
        // compare 1
    }else if((raw_opcode & 0b1110) == 0b0110){
        // compare 2
    }
    
    
    printf("invalid special data  instruction or branch and exchange %x at %x \n ", instruction.raw_instruction , cpu->registers.PC - 2);
    return 1;
}
uint8_t run_32bit_instruction(struct pico_cpu *cpu, struct raw_instruction start_instruction){
    struct raw32_instruction instruction_32;
    uint32_t third = fetch_byte(cpu);
    uint32_t fourth = fetch_byte(cpu);
    instruction_32.value_0_8 = third;
    instruction_32.value_8_16 = fourth;
    instruction_32.value_16_24 = start_instruction.down;
    instruction_32.value_24_32 = start_instruction.up;
    instruction_32.raw_instruction = ((uint32_t)start_instruction.raw_instruction) << 16;
    instruction_32.raw_instruction |= third;
    instruction_32.raw_instruction |= fourth << 8;

    if((instruction_32.value_24_32 & 0b00011000) == 0b00010000 && (instruction_32.value_8_16 & 0b11000000) == 0b11000000){ // 32bit instruction
        return BL_instruction_t1( instruction_32, cpu);
    }
    printf("invalid 32bit instruction %x at %x \n ", instruction_32.raw_instruction , cpu->registers.PC - 2);
    return 1;
}
uint8_t run_instruction(struct pico_cpu *cpu)
{
    uint8_t first = fetch_byte(cpu);
    uint8_t second = fetch_byte(cpu);
    uint16_t instruction = first | (second << 8);
    struct raw_instruction raw_instruction;
    raw_instruction.up = second;
    raw_instruction.down = first;
    raw_instruction.raw_instruction = instruction;
    // LDR (literal) instruction
    if ((second & 0b11111000) == 0b01001000)
    { 
        return ldr_litteral_instruction(raw_instruction, cpu);
    }
    // LDR (immediate) instruction
    else if ((second & 0b11111000) == 0b01101000)
    { 
        return ldr_immediate_instruction(raw_instruction, cpu);
    }
    // CMP (immediate) instruction

    else if ((second & 0b11111000) == 0b00101000)
    { 
        return cmp_immediate(raw_instruction, cpu);
    }
    // branch instruction
    else if((second & 0b11110000) == 0b11010000){
        return B_instruction_t1(raw_instruction, cpu);
    }
    // Special data instruction or Branch or Exchange 
    else if((second & 0b11111100) == 0b01000100){
        return run_SdiBE_instruction(cpu, raw_instruction);
    }
    // MOVS (mov immediate)
    else if((second & 0b11111000) == 0b00100000){
        return mov_immediate(raw_instruction, cpu);
    }// MVNS (mvn with register)
    else if((raw_instruction.raw_instruction & 0b1111111111000000) == 0b0100001111000000){
        return mvns_instruction_t1(raw_instruction, cpu);
    }// STR (str immediate t1)
    else if((second & 0b11111000) == 0b01100000){
        return str_immediate_t1(raw_instruction, cpu);
    }
    // 32bit instruction
    else if((second & 0b11100000) == 0b11100000){ // 32bit instruction
        return run_32bit_instruction( cpu,raw_instruction);
    }
    printf("invalid instruction %x at %x \n ", instruction, cpu->registers.PC - 2);
    return 1;
}
