#include "sio_reg.h"
#include <stdio.h>
#include <stdlib.h>
struct SIO_Register reg_table[] = {
    {"CPUID", cpuid_read, null_write},
    {"GPIO_IN", null_read, null_write}};

int null_write(struct SIO_Register *self, struct pico_cpu *cpu, const uint32_t target)
{
    printf("error: trying to not writable SIO register: %s at 0x%x \n", self->name, cpu->registers.PC);
    return 1;
}

int null_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target)
{
    printf("error: trying to not readable SIO register: %s at 0x%x \n", self->name, cpu->registers.PC);
    return 1;
}

int cpuid_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target)
{
    *target = cpu->core_id;
    return 0;
}

int read_sio_32(pico_addr raw_addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self)
{
    uint32_t target_register = raw_addr / 4;

    if (target_register >= (sizeof(reg_table) / sizeof(reg_table[0])))
    {
        printf("invalid sio read at offset %x \n", raw_addr);
        return -1;
    }

    return reg_table[target_register].read_handler(&reg_table[target_register], cpu, target) == 0;
}

int write_sio_32(pico_addr raw_addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self)
{

    uint32_t target_register = raw_addr / 4;

    if (target_register > (sizeof(reg_table) / sizeof(reg_table[0])))
    {
        printf("invalid sio write at offset %x \n", raw_addr);
        return -1;
    }
    return reg_table[target_register].write_handler(&reg_table[target_register], cpu, target) == 0;
}

int init_sio(struct pico_cpu *cpu)
{

    struct memory_region *mem_region = malloc(sizeof(struct memory_region));

    mem_region->can_write = true;
    mem_region->can_read = true;
    mem_region->name = "SIO";
    mem_region->start = PICO_SIO_START;
    mem_region->size = PICO_SIO_LENGTH;
    mem_region->read8 = NULL;
    mem_region->read16 = NULL;
    mem_region->read32 = read_sio_32;
    mem_region->write8 = NULL;
    mem_region->write16 = NULL;
    mem_region->write32 = write_sio_32;
    mem_region->data_is_malloc = false;

    add_dynamic_memory_region(&cpu->regions, mem_region);
    return 0;
}
