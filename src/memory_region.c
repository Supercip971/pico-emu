#include "pico_cpu.h"
#include <stdio.h>
#include <stdlib.h>
void add_dynamic_memory_region(struct memory_region_list *mem_list, struct memory_region *region)
{
    printf("loading memory region %s 0x%x-0x%x \n", region->name, region->start, region->size + region->start);
    printf("region write: %i read: %i \n", region->can_write, region->can_read);
    struct memory_region *reg = get_memory_region(mem_list, region->start);
    if (reg != NULL)
    {
        printf("warning: region use a region used by another memory_region %s 0x%x-0x%x \n", reg->name, reg->start, reg->size + reg->start);
    }
    mem_list->regions[mem_list->memory_region_count] = region;
    mem_list->memory_region_count++;
    if (mem_list->memory_region_count > MAX_MEMORY_REGION)
    {
        printf("can't add the memory region: too much memory region used\n");
        exit(1);
    }
}
struct memory_region *get_memory_region(struct memory_region_list *mem_list, pico_addr addr)
{
    for (int i = 0; i < mem_list->memory_region_count; i++)
    {
        if ((addr >= mem_list->regions[i]->start) && addr <= (mem_list->regions[i]->size + mem_list->regions[i]->start))
        {
            return mem_list->regions[i];
        }
    }
    return NULL;
}
void destroy_dynamic_memory_region_list(struct memory_region_list *mem_list)
{
    for (int i = 0; i < mem_list->memory_region_count; i++)
    {
        free(mem_list->regions[i]);
    }
}
int read_memory_region_8(struct memory_region *region, uint8_t *target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_read)
    {
        region->read8(addr, cpu, target, region);
    }
    else
    {
        printf("trying to read a memory region that is not readable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}
int read_memory_region_16(struct memory_region *region, uint16_t *target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_read)
    {
        region->read16(addr, cpu, target, region);
    }
    else
    {
        printf("trying to read a memory region that is not readable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}
int read_memory_region_32(struct memory_region *region, uint32_t *target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_read)
    {
        region->read32(addr, cpu, target, region);
    }
    else
    {
        printf("trying to read a memory region that is not readable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}

int write_memory_region_8(struct memory_region *region, uint8_t target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_write)
    {
        region->write8(addr, cpu, target, region);
    }
    else
    {
        printf("trying to write a memory region that is not writable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}
int write_memory_region_16(struct memory_region *region, uint16_t target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_write)
    {
        region->write16(addr, cpu, target, region);
    }
    else
    {
        printf("trying to write a memory region that is not writable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}
int write_memory_region_32(struct memory_region *region, uint32_t target, pico_addr addr, struct pico_cpu *cpu)
{
    if (region->can_write)
    {
        region->write32(addr, cpu, target, region);
    }
    else
    {
        printf("trying to write a memory region that is not writable: 0x%x \n", addr);
        return -1;
    }
    return 0;
}
