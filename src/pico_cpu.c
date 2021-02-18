#include "pico_cpu.h"
#include "sio_reg.h"
#include <stdio.h>
const char* registers_names[] = { "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC"};
void reset_cpu(struct pico_cpu *cpu)
{
    cpu->registers.status.carry_flag = 0;
    cpu->registers.status.zero_condition = 0;
    cpu->registers.status.overflow_flag = 0;
    cpu->registers.status.negative_condition = 0;

    cpu->registers.LR = 0;
    cpu->registers.PC = 0;

    for (int i = 0; i < R_REGISTER_COUNT; i++)
    {
        cpu->registers.R_register[i] = 0;
    }
}

uint32_t* get_register(uint32_t id, struct pico_register* table){
    if(id < R_REGISTER_COUNT){
        
        return &table->R_register[id] ;
    }else if(id == 15){
        
        return &table->PC ;
    }else if(id == 14){
        
        return &table->LR ;
    }else if(id == 13){
        
        return &table->SP ;
    }
    
    return NULL;
}

const char* get_register_name(uint32_t id){
    return registers_names[id];
}

uint8_t fetch_byte(struct pico_cpu *cpu)
{
    uint8_t result;
    read_memory_byte(cpu, &result, cpu->registers.PC);
    cpu->registers.PC += 1;

    return result;
}

void dump_cpu(const struct pico_cpu *cpu)
{
    printf("dump of cpu: \n");
    printf("PC: 0x%x SP: 0x%x LR: %x", cpu->registers.PC, cpu->registers.PC, cpu->registers.LR);
    for (int i = 0; i < R_REGISTER_COUNT; i++)
    {
        if (i % 4 == 0)
        {
            printf("\n");
        }
        printf("R%i: 0x%x \t", i, cpu->registers.R_register[i]);
    }
    printf("\n");
}

bool init_bootrom_table(struct pico_cpu *cpu)
{
    init_boot_rom_vector(&cpu->memory.bootrom_vec, &cpu->memory.boot_rom);
    cpu->registers.SP = cpu->memory.bootrom_vec.initial_stack - 1;
    cpu->registers.PC = cpu->memory.bootrom_vec.boot_rom_start - 1;
    printf("bootrom table: start = 0x%x stack = 0x%x \n", cpu->registers.PC, cpu->registers.SP);
    return true;
}

bool start_cpu(struct pico_cpu *cpu, int cpu_id)
{
    cpu->core_id = cpu_id;
    init_bootrom_table(cpu);

    printf("started cpu: %i \n", cpu_id);
    return true;
}
// this is terrible: todo fix this code
int read_memory_byte(struct pico_cpu *cpu, uint8_t *target, pico_addr addr)
{
    //reading the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        if (!read_rom_8(&cpu->memory.boot_rom, addr - PICO_ROM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    //reading sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!read_sram_8(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    // reading flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't read flash memory for the moment \n");
        dump_cpu(cpu);
        return READ_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to read out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return READ_MEMORY_OOB;
    }
}
int read_memory_word(struct pico_cpu *cpu, uint16_t *target, pico_addr addr)
{
    //reading the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        if (!read_rom_16(&cpu->memory.boot_rom, addr - PICO_ROM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    //reading sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!read_sram_16(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    // reading flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't read flash memory for the moment \n");
        dump_cpu(cpu);
        return READ_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to read out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return READ_MEMORY_OOB;
    }
}
int read_memory_dword(struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    //reading the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        if (!read_rom_32(&cpu->memory.boot_rom, addr - PICO_ROM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    //reading sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!read_sram_32(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return READ_MEMORY_ROM_ERROR;
        }
        return READ_MEMORY_SUCCESS;
    }
    // reading flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't read flash memory for the moment \n");
        dump_cpu(cpu);
        return READ_MEMORY_FLASH_ERROR;
    }
    // reading SIO
    else if (addr >= PICO_SIO_START && addr < PICO_SIO_START + PICO_SIO_LENGTH)
    {
        if (!read_sio_32(cpu, addr - PICO_SIO_START, target))
        {
            return READ_MEMORY_SIO_ERROR;
        }
        return READ_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to read out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return READ_MEMORY_OOB;
    }
}
// -------------------------------- writing --------------------------------
int write_memory_byte(struct pico_cpu *cpu, uint8_t target, pico_addr addr)
{
    //writing the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        printf("error: trying to write to the ROM wich is a RO memory \n");
        return WRITE_MEMORY_ROM_ERROR;
    }
    //writing sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!write_sram_8(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return WRITE_MEMORY_ROM_ERROR;
        }
        return WRITE_MEMORY_SUCCESS;
    }
    // writing flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't write flash memory for the moment \n");
        dump_cpu(cpu);
        return WRITE_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to writing out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return WRITE_MEMORY_OOB;
    }
}
int write_memory_word(struct pico_cpu *cpu, uint16_t target, pico_addr addr)
{
    //writing the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        printf("error: trying to write to the ROM wich is a RO memory \n");
        return WRITE_MEMORY_ROM_ERROR;
    }
    //writing sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!write_sram_16(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return WRITE_MEMORY_ROM_ERROR;
        }
        return WRITE_MEMORY_SUCCESS;
    }
    // writing flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't write flash memory for the moment \n");
        dump_cpu(cpu);
        return WRITE_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to writing out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return WRITE_MEMORY_OOB;
    }
}
int write_memory_dword(struct pico_cpu *cpu, uint32_t target, pico_addr addr)
{
    //writing the rom
    if (addr >= PICO_ROM_ADDR && addr <= PICO_ROM_ADDR + PICO_ROM_SIZE)
    {
        printf("error: trying to write to the ROM wich is a RO memory \n");
        return WRITE_MEMORY_ROM_ERROR;
    }
    //writing sram
    else if (addr >= PICO_SRAM_ADDR && addr <= PICO_SRAM_ADDR + PICO_SRAM_SIZE)
    {
        if (!write_sram_32(&cpu->memory.sram, addr - PICO_SRAM_ADDR, target))
        {
            return WRITE_MEMORY_ROM_ERROR;
        }
        return WRITE_MEMORY_SUCCESS;
    }
    // writing flash
    else if (addr > PICO_FLASH_MEMORY_ADDR && addr < PICO_FLASH_MEMORY_ADDR + PICO_FLASH_MEMORY_SIZE)
    {
        printf("can't write flash memory for the moment \n");
        dump_cpu(cpu);
        return WRITE_MEMORY_FLASH_ERROR;
    }
    else if (addr >= PICO_SIO_START && addr < PICO_SIO_START + PICO_SIO_LENGTH)
    {
        if (!write_sio_32(cpu, addr - PICO_SIO_START, target))
        {
            return WRITE_MEMORY_SIO_ERROR;
        }
        return WRITE_MEMORY_FLASH_ERROR;
    }
    else
    {
        printf("error: cpu trying to writing out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return WRITE_MEMORY_OOB;
    }
}
