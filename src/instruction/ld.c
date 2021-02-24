#include "ld.h"
#include "../pico_cpu.h"
#include "../utils.h"
#include <stdio.h>
uint8_t ldr_litteral_instruction(struct raw_instruction instruction, struct pico_cpu *cpu)
{
    uint8_t target_register = instruction.up & 0b111;
    uint32_t end_value = ((uint32_t)instruction.down) << 2;

    uint32_t base = ((cpu->registers.PC+2) & 0xfffffffc);
    uint32_t addr = (base + end_value);

    read_memory_dword(cpu, &cpu->registers.R_register[target_register], addr);
    printf("ldr %s [PC, #%i] ; 0x%x \n", get_register_name(target_register), end_value, addr);

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


    printf("ldr immediate instruction: R%i -> R%i  with offset 0x%x final addr: 0x%x = 0x%x\n", base_register, destination_register, imm32, offset_addr, cpu->registers.R_register[destination_register]);
    return 0;
}
uint8_t ldm_t1_instruction(struct raw_instruction instruction, struct pico_cpu *cpu){
    uint32_t n = instruction.up & 0b111;
    uint32_t registers = instruction.down;

    int wback = ((registers & (1 << n)) == 0);
    int write_count = 0; // 
    int registers_count[7];
    uint32_t* Rn = get_register(n, &cpu->registers);
    if(Rn == NULL){
        return -1;
    }
    uint32_t addr = *Rn;
    for(int i = 0; i < 7; i++){

        int current = ((registers & (1 << i)) != 0);
        if(current){
            registers_count[write_count] = i;

            write_count++;
            uint32_t* target = get_register(i, &cpu->registers);
            read_memory_dword(cpu, target, addr);
            addr +=4;

        }
    }
    if(wback){
        printf("LDM %s!,", get_register_name(n));
        *Rn = *Rn + (write_count*4);
    }else{

        printf("LDM %s,", get_register_name(n));
    }
    
    printf("{");
    for(int i = 0; i < write_count; i++){
        printf("%s,", get_register_name(registers_count[i]));
    }
    printf("} \n");
    return 0;
}