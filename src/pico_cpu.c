#include <stdio.h>

#include "pico_cpu.h"
#include "pico_apb.h"
#include "pico_rom.h"
#include "sio_reg.h"
#include "sram.h"

const char *registers_names[] = {"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC"};
const char *special_registers_names[] = {"APSR", "IAPSR", "EAPSR", "XPSR", "unused", "IPSR", "EPSR", "IEPSR", "MSP", "PSP", "PRIMASK", "CONTROL"};
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

    reset_abp_voltage_reg(&cpu->apb_register.voltage_reg);

    for (int i = 0; i < R_REGISTER_COUNT; i++)
    {
        cpu->registers.R_register[i] = 0;
    }
}

int init_cpu(struct pico_cpu *cpu, char *file_path)
{
    cpu->privileged = true;
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
    }

    if (init_apb(cpu) != 0)
    {
        return -5;
    }
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
const char *get_special_register_name(uint32_t id)
{
    if(id < 10&& id != 4){
        return special_registers_names[id];

    }else if(id == 16){
        return "PRIMASK";
    }else if(id == 20){
        return "CONTROL";
    }
    return " -- invalid special register -- ";
}
uint32_t *get_special_register(uint32_t id, struct pico_register *table)
{
    if (id < 10 && id != 4)
    {

        return (((uint32_t*)&table->special_reg) + id);
    }
    else if (id == 16)
    {

        return &table->special_reg.PRIMASK;
    }
    else if (id == 20)
    {

        return &table->special_reg.CONTROL;
    }

    return NULL;
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

void init_bootrom_table(struct pico_cpu *cpu)
{
    init_boot_rom_vector(&cpu->bootrom_vec, cpu);

    cpu->registers.SP = cpu->bootrom_vec.initial_stack - 1;
    cpu->registers.PC = cpu->bootrom_vec.boot_rom_start - 1;

    printf("bootrom table: start = 0x%x stack = 0x%x \n", cpu->registers.PC, cpu->registers.SP);
}

void start_cpu(struct pico_cpu *cpu, int cpu_id)
{
    cpu->core_id = cpu_id;
    init_bootrom_table(cpu);

    printf("started cpu: %i \n", cpu_id);
}

void stop_cpu(struct pico_cpu *cpu)
{
    destroy_dynamic_memory_region_list(&cpu->regions);
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
