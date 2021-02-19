#include "pico_cpu.h"
#include "pico_apb.h"
#include "pico_rom.h"
#include "sio_reg.h"
#include "sram.h"
#include <stdio.h>
const char *registers_names[] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC"};
void reset_cpu(struct pico_cpu *cpu)
{
    cpu->registers.status.carry_flag = 0;
    cpu->registers.status.zero_condition = 0;
    cpu->registers.status.overflow_flag = 0;
    cpu->registers.status.negative_condition = 0;
    cpu->apb_register.syscfg_reg.core_0_nmi = 0;
    cpu->apb_register.syscfg_reg.core_1_nmi = 0;
    cpu->apb_register.syscfg_reg.proc_config.proc1_dap_instance_id = 1;

    cpu->registers.LR = 0;
    cpu->registers.PC = 0;

    for (int i = 0; i < R_REGISTER_COUNT; i++)
    {
        cpu->registers.R_register[i] = 0;
    }
}

int init_cpu(struct pico_cpu *cpu, char *file_path)
{
    cpu->regions.memory_region_count = 0;
    if (init_rom(cpu) != 0)
    {
        return -2;
    }

    char *utf2_path = file_path;
    printf("loading sram with file %s \n", utf2_path);

    if (init_sram(utf2_path, cpu) != 0)
    {
        return -3;
    }

    if (init_sio(cpu) != 0)
    {
        return -4;
    };
    if (init_apb(cpu) != 0)
    {
        return -5;
    };
    return 0;
}
uint32_t *get_register(uint32_t id, struct pico_register *table)
{
    if (id < R_REGISTER_COUNT)
    {

        return &table->R_register[id];
    }
    else if (id == 15)
    {

        return &table->PC;
    }
    else if (id == 14)
    {

        return &table->LR;
    }
    else if (id == 13)
    {

        return &table->SP;
    }

    return NULL;
}

const char *get_register_name(uint32_t id)
{
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
    init_boot_rom_vector(&cpu->bootrom_vec, cpu);
    cpu->registers.SP = cpu->bootrom_vec.initial_stack - 1;
    cpu->registers.PC = cpu->bootrom_vec.boot_rom_start - 1;
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
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to read OOB memory %x \n", addr);
        return READ_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to read memory that can't be readed %s at %x \n", res->name, addr - res->start);
        return READ_MEMORY_WO_ERROR;
    }
    return res->read8(addr - res->start, cpu, target, res);
}
int read_memory_word(struct pico_cpu *cpu, uint16_t *target, pico_addr addr)
{
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to read OOB memory %x \n", addr);
        return READ_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to read memory that can't be readed %s at %x \n", res->name, addr - res->start);
        return READ_MEMORY_WO_ERROR;
    }
    return res->read16(addr - res->start, cpu, target, res);
}
int read_memory_dword(struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to read OOB memory %x \n", addr);
        return READ_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to read memory that can't be readed %s at %x \n", res->name, addr);
        return READ_MEMORY_WO_ERROR;
    }
    return res->read32(addr - res->start, cpu, target, res);
}
// -------------------------------- writing --------------------------------
int write_memory_byte(struct pico_cpu *cpu, uint8_t target, pico_addr addr)
{
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to write OOB memory %x \n", addr);
        return WRITE_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to write memory that can't be readed %s at %x \n", res->name, addr - res->start);
        return WRITE_MEMORY_RO_ERROR;
    }
    return res->write8(addr - res->start, cpu, target, res);
}
int write_memory_word(struct pico_cpu *cpu, uint16_t target, pico_addr addr)
{
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to write OOB memory %x \n", addr);
        return WRITE_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to write memory that can't be readed %s at %x \n", res->name, addr - res->start);
        return WRITE_MEMORY_RO_ERROR;
    }
    return res->write16(addr - res->start, cpu, target, res);
}
int write_memory_dword(struct pico_cpu *cpu, uint32_t target, pico_addr addr)
{
    struct memory_region *res = get_memory_region(&cpu->regions, addr);
    if (res == NULL)
    {
        printf("trying to write OOB memory %x \n", addr);
        return WRITE_MEMORY_OOB;
    }
    else if (!res->can_read)
    {
        printf("trying to write memory that can't be readed %s at %x \n", res->name, addr - res->start);
        return WRITE_MEMORY_RO_ERROR;
    }
    return res->write32(addr - res->start, cpu, target, res);
}
