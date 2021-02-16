#include "pico_cpu.h"
#include <stdio.h>

void reset_cpu(struct pico_cpu *cpu)
{
    cpu->status.carry_flag = 0;
    cpu->status.zero_confition = 0;
    cpu->status.overflow_flag = 0;
    cpu->status.negative_condition = 0;

    cpu->registers.LR = 0;
    cpu->registers.PC = 0;

    for (int i = 0; i < R_REGISTER_COUNT; i++)
    {
        cpu->registers.R_register[i] = 0;
    }
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

bool start_cpu(struct pico_cpu *cpu)
{
    init_bootrom_table(cpu);
    return true;
}

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
int read_memory_dword(struct pico_cpu *cpu, uint32_t*target, pico_addr addr)
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
    else
    {
        printf("error: cpu trying to read out of bound memory address 0x%x \n", addr);
        dump_cpu(cpu);
        return READ_MEMORY_OOB;
    }
}