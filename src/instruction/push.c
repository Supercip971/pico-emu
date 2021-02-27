#include "push.h"
#include "instruction_list.h"
#include <stdio.h>
uint8_t push_t1_instruction(struct raw_instruction instruction, struct pico_cpu *cpu){
    
    uint32_t registers = instruction.down | ((instruction.up & 1) << 14);

    int write_count = 0; // 
    int registers_target[9];
    uint32_t* sp = get_register(13, &cpu->registers);
    if(sp == NULL){
        return -1;
    }
    for(int i = 0; i < 15; i++){

        int current = ((registers & (1 << i)) != 0);
        if(current){
            registers_target[write_count] = i;
            write_count++;
        
        }
    }
        
    *sp = *sp - 4*write_count;
    uint32_t target = *sp;
    for(int i = 0; i < write_count;i++){
        
        printf("%x \n", target);
        uint32_t* targ = get_register(registers_target[i], &cpu->registers);
        write_memory_dword(cpu, *targ, target);
        target+=4;
    }
    printf("push { ");
    for(int i = 0; i < write_count; i++){
        printf("%s", get_register_name(registers_target[i]));
        if(i != write_count-1){
            printf(",");
        }
    }
    printf("} \n");

    return 0;
}