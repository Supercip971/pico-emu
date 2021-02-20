#include "pico_rom.h"
#include "config.h"
#include "pico_cpu.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_valid_rom_addr(pico_addr addr)
{
    return (addr - PICO_ROM_ADDR) < PICO_ROM_SIZE;
}

int read_rom_8(pico_addr raw_addr, struct pico_cpu *cpu, uint8_t *target, struct memory_region *self)
{

    uint8_t *v = self->data + (raw_addr);
    *target = *v;

    return 0;
}

int read_rom_16(pico_addr raw_addr, struct pico_cpu *cpu, uint16_t *target, struct memory_region *self)
{

    uint16_t *v = (uint16_t *)(self->data + (raw_addr));
    *target = *v;

    return 0;
}

int read_rom_32(pico_addr raw_addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self)
{

    uint32_t *v = (uint32_t *)(self->data + (raw_addr));
    *target = *v;

    return 0;
}

int init_rom(struct pico_cpu *cpu)
{

    FILE *f = fopen("bootrom", "rb");

    if (f == NULL)
    {
        printf("error while reading bootrom...\n");
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize > PICO_ROM_SIZE)
    {
        printf("pico bootrom size is %li and should be below %i \n", fsize,
               PICO_ROM_SIZE);
        fclose(f);

        return -2;
    }

    struct memory_region *mem_region = malloc(sizeof(struct memory_region));

    mem_region->data = malloc(PICO_ROM_SIZE);
    mem_region->data_size = PICO_ROM_SIZE;
    mem_region->data_is_malloc = true;

    fread(mem_region->data, 1, fsize, f);
    fclose(f);

    mem_region->can_write = false;
    mem_region->can_read = true;
    mem_region->name = "ROM";
    mem_region->start = PICO_ROM_ADDR;
    mem_region->size = PICO_ROM_SIZE;
    mem_region->read8 = read_rom_8;
    mem_region->read16 = read_rom_16;
    mem_region->read32 = read_rom_32;
    mem_region->write8 = NULL;
    mem_region->write16 = NULL;
    mem_region->write32 = NULL;

    add_dynamic_memory_region(&cpu->regions, mem_region);

    return 0;
}

bool init_boot_rom_vector(struct pico_bootrom_vector *target, struct pico_cpu *cpu)
{
    size_t length_to_read = sizeof(struct pico_bootrom_vector);
    uint8_t *raw_target = malloc(sizeof(struct pico_bootrom_vector));

    for (uint32_t i = 0; i < length_to_read; i++)
    {
        read_memory_byte(cpu, raw_target + i, i);
    }

    memcpy(target, raw_target, length_to_read);

    free(raw_target);
    return true;
}
