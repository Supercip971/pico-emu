#include "pico_apb.h"

#include "pico_cpu.h"
#include <stdio.h>
#include <stdlib.h>
int abp_null_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{

    printf("error: trying write to not writable ABP register: %s at 0x%x \n", self->name, addr);
    return 1;
}
int abp_null_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{

    printf("error: trying read to not readable ABP register: %s at 0x%x \n", self->name, addr);
    return 1;
}
int apb_syscfg_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    return 1;
}
int abp_syscfg_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.syscfg_reg;
    data += addr;
    *target = *((uint32_t *)data);
    return 0;
}
int abp_syscfg_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.syscfg_reg;
    data += addr;
    *((uint32_t *)data) = target;
    return 0;
}
struct APB_raw_Register abp_reg_table[] = {
    {"SYSINFO_BASE", 0, 0x44, abp_null_read, abp_null_write},
    {"SYSCFG_BASE", 0x4000, 0x19, abp_syscfg_read, abp_syscfg_write}};

#define REG_TABLE_ENTRY_COUNT sizeof(abp_reg_table) / sizeof(struct APB_raw_Register)
struct APB_raw_Register *select_register(pico_addr addr)
{
    for (int i = 0; i < REG_TABLE_ENTRY_COUNT; i++)
    {
        if (abp_reg_table[i].base <= addr && (abp_reg_table[i].base + abp_reg_table[i].size) >= addr)
        {
            return &abp_reg_table[i];
        }
    }
    return NULL;
}

int read_abp_32(pico_addr raw_addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self)
{
    struct APB_raw_Register *reg = select_register(raw_addr);
    if (reg == NULL)
    {
        printf("invalid ABP read at offset 0x%x \n", raw_addr);
        return -1;
    }
    return reg->read_handler(reg, cpu, target, raw_addr - reg->base);
}

int write_abp_32(pico_addr raw_addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self)
{
    struct APB_raw_Register *reg = select_register(raw_addr);
    if (reg == NULL)
    {
        printf("invalid ABP write at offset 0x%x \n", raw_addr);
        return -1;
    }
    return reg->write_handler(reg, cpu, target, raw_addr - reg->base);
}

int init_apb(struct pico_cpu *cpu)
{

    struct memory_region *mem_region = malloc(sizeof(struct memory_region));

    mem_region->can_write = true;
    mem_region->can_read = true;
    mem_region->name = "ABP";
    mem_region->start = PICO_APB_START;
    mem_region->size = PICO_APB_LENGTH;
    mem_region->read8 = NULL;
    mem_region->read16 = NULL;
    mem_region->read32 = read_abp_32;
    mem_region->write8 = NULL;
    mem_region->write16 = NULL;
    mem_region->write32 = write_abp_32;
    add_dynamic_memory_region(&cpu->regions, mem_region);
    return 0;
}