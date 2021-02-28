

#include "sram.h"
#include <stdbool.h>
#include <stdlib.h>
int read_sram_8(pico_addr addr, struct pico_cpu *cpu, uint8_t *target, struct memory_region *self);
int read_sram_16(pico_addr addr, struct pico_cpu *cpu, uint16_t *target, struct memory_region *self);
int read_sram_32(pico_addr addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self);

int write_sram_8(pico_addr addr, struct pico_cpu *cpu, const uint8_t target, struct memory_region *self);
int write_sram_16(pico_addr addr, struct pico_cpu *cpu, const uint16_t target, struct memory_region *self);
int write_sram_32(pico_addr addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self);

int init_sram(const char *file_path, struct pico_cpu *cpu)
{
    struct memory_region *mem_region = malloc(sizeof(struct memory_region));

    mem_region->can_write = true;
    mem_region->can_read = true;
    mem_region->name = "SRAM";
    mem_region->start = PICO_SRAM_ADDR;
    mem_region->size = PICO_SRAM_SIZE;
    mem_region->data = aligned_alloc(4, PICO_SRAM_SIZE);
    mem_region->read8 = read_sram_8;
    mem_region->read16 = read_sram_16;
    mem_region->read32 = read_sram_32;
    mem_region->write8 = write_sram_8;
    mem_region->write16 = write_sram_16;
    mem_region->write32 = write_sram_32;
    mem_region->data_is_malloc = true;

    add_dynamic_memory_region(&cpu->regions, mem_region);

    return 0;
}

int read_sram_8(pico_addr addr, struct pico_cpu *cpu, uint8_t *target, struct memory_region *self)
{
    uint8_t *v = (uint8_t *)(self->data + (addr));
    *target = *v;
    return 0;
}

int read_sram_16(pico_addr addr, struct pico_cpu *cpu, uint16_t *target, struct memory_region *self)
{
    uint16_t *v = (uint16_t *)(self->data + (addr));
    *target = *v;
    return 0;
}

int read_sram_32(pico_addr addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self)
{
    uint32_t *v = (uint32_t *)(self->data + (addr));
    *target = *v;
    return 0;
}

int write_sram_8(pico_addr addr, struct pico_cpu *cpu, const uint8_t target, struct memory_region *self)
{
    uint8_t *v = (uint8_t *)(self->data + (addr));
    *v = target;
    return 1;
}

int write_sram_16(pico_addr addr, struct pico_cpu *cpu, const uint16_t target, struct memory_region *self)
{
    uint16_t *v = (uint16_t *)(self->data + (addr));
    *v = target;
    return 1;
}

int write_sram_32(pico_addr addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self)
{
    uint32_t *v = (uint32_t *)(self->data + (addr));
    *v = target;
    return 1;
}