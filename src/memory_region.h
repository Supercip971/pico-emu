#ifndef MEM_REGION_H
#define MEM_REGION_H
#include <stddef.h>
#include <stdbool.h>

#include "config.h"
#include "pico_cpu.h"

typedef struct pico_cpu pico_cpu;
struct memory_region
{
    pico_addr start;
    pico_addr size;
    const char *name;

    uint8_t *data;
    bool data_is_malloc;
    size_t data_size;

    bool can_read;
    int (*read8)(pico_addr, struct pico_cpu *, uint8_t *, struct memory_region *);
    int (*read16)(pico_addr, struct pico_cpu *, uint16_t *, struct memory_region *);
    int (*read32)(pico_addr, struct pico_cpu *, uint32_t *, struct memory_region *);

    bool can_write;
    int (*write8)(pico_addr, struct pico_cpu *, uint8_t, struct memory_region *);
    int (*write16)(pico_addr, struct pico_cpu *, uint16_t, struct memory_region *);
    int (*write32)(pico_addr, struct pico_cpu *, uint32_t, struct memory_region *);
};
struct memory_region_list
{
    struct memory_region *regions[MAX_MEMORY_REGION];
    int memory_region_count;
};

void add_dynamic_memory_region(struct memory_region_list *mem_list, struct memory_region *region);
void destroy_dynamic_memory_region_list(struct memory_region_list *mem_list);
struct memory_region *get_memory_region(struct memory_region_list *mem_list, pico_addr addr);

int read_memory_region_8(struct memory_region *region, uint8_t *target, pico_addr addr, struct pico_cpu *cpu);
int read_memory_region_16(struct memory_region *region, uint16_t *target, pico_addr addr, struct pico_cpu *cpu);
int read_memory_region_32(struct memory_region *region, uint32_t *target, pico_addr addr, struct pico_cpu *cpu);

int write_memory_region_8(struct memory_region *region, uint8_t target, pico_addr addr, struct pico_cpu *cpu);
int write_memory_region_16(struct memory_region *region, uint16_t target, pico_addr addr, struct pico_cpu *cpu);
int write_memory_region_32(struct memory_region *region, uint32_t target, pico_addr addr, struct pico_cpu *cpu);

#endif