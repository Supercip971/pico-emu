#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico_apb.h"
#include "pico_cpu.h"

struct APB_raw_Register abp_reg_table[] = {
    {"SYSINFO", 0, 0x44, abp_null_read, abp_null_write},
    {"SYSCFG", 0x4000, 0x19, abp_syscfg_read, abp_syscfg_write},
    {"CLOCK_REG", 0x80a0, 0x4, abp_clock0_read, abp_clock0_write},
    {"RESET_REG", 0xc000, 0x16, reset_read, reset_write},
    {"VREG_CHIP_RESET", 0x64000, 0x11, abp_vreg_read, abp_vreg_write},
    {"TBMAN", 0x6c000, 0x4, tbman_read, NULL},
    {"WATCHDOG", 0x58000, 0x30, watchdog_read, watchdog_write}};

void select_rw_type(int *type, pico_addr *addr)
{
    if (*addr & APB_OFFSET_XOR && *addr & APB_OFFSET_SET)
    {
        *type = APB_CLR_RW;
    }
    else if (*addr & APB_OFFSET_SET)
    {
        *type = APB_SET_RW;
    }
    else if (*addr & APB_OFFSET_XOR)
    {
        *type = APB_XOR_RW;
    }

    *addr = *addr & ~(0x3 << 12); // just clear offset bits to use the raw address
}

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

int abp_vreg_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.voltage_reg;
    data += addr;
    *target = *((uint32_t *)data);

    return 0;
}

int abp_vreg_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.voltage_reg;
    data += addr;
    *((uint32_t *)data) = target;

    return 0;
}

int abp_clock0_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->clock0;
    data += addr;
    *target = *((uint32_t *)data);

    return 0;
}

int abp_clock0_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{

    uint8_t *data = (uint8_t *)&cpu->clock0;
    data += addr;
    *((uint32_t *)data) = target;

    return 0;
}

int watchdog_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.watchdog_reg;
    data += addr;
    *target = *((uint32_t *)data);

    return 0;
}

int watchdog_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.watchdog_reg;
    data += addr;
    *((uint32_t *)data) = target;

    return 0;
}

int reset_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.reset_reg;
    data += addr;
    *target = *((uint32_t *)data);

    return 0;
}

int reset_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr)
{
    uint8_t *data = (uint8_t *)&cpu->apb_register.reset_reg;
    data += addr;
    *((uint32_t *)data) = target;

    return 0;
}

int tbman_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr)
{
    *target = 0;

    return 0;
}

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

    printf("reading abp %x \n", raw_addr);

    int res = reg->read_handler(reg, cpu, target, raw_addr - reg->base);

    printf("result: %x \n", *target);

    return res;
}

int write_abp_32(pico_addr raw_addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self)
{
    int rw_type = APB_NORMAL_RW;
    select_rw_type(&rw_type, &raw_addr);

    struct APB_raw_Register *reg = select_register(raw_addr);

    if (reg == NULL)
    {
        printf("invalid ABP write %i at offset 0x%x \n", rw_type, raw_addr);
        return -1;
    }
    if (rw_type == APB_NORMAL_RW)
    {
        return reg->write_handler(reg, cpu, target, raw_addr - reg->base);
    }
    else if (rw_type == APB_SET_RW)
    {
        uint32_t targ = reg->read_handler(reg, cpu, &targ, raw_addr - reg->base);
        targ |= target;
        return reg->write_handler(reg, cpu, targ, raw_addr - reg->base);
    }
    else if (rw_type == APB_CLR_RW)
    {
        uint32_t targ = reg->read_handler(reg, cpu, &targ, raw_addr - reg->base);
        targ = targ & ~(target);
        return reg->write_handler(reg, cpu, targ, raw_addr - reg->base);
    }
    else if (rw_type == APB_XOR_RW)
    {
        uint32_t targ = reg->read_handler(reg, cpu, &targ, raw_addr - reg->base);
        targ = targ ^ (target);
        return reg->write_handler(reg, cpu, targ, raw_addr - reg->base);
    }
    return -1;
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
    mem_region->data_is_malloc = false;

    add_dynamic_memory_region(&cpu->regions, mem_region);
    return 0;
}

int reset_abp_voltage_reg(struct APB_Voltage_register *reg)
{
    memset(reg, 0, sizeof(struct APB_Voltage_register));

    reg->control_status.vsel = 0xb;
    reg->control_status.enable = 0x1;
    reg->bod_register.vsel = 0x9;
    reg->bod_register.enable = 0x1;

    return 0;
}
