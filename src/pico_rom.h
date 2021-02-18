#ifndef PICO_ROM_H
#define PICO_ROM_H
#include "config.h"
#include "pico_cpu.h"

#include <stdbool.h>
struct pico_rom
{
    uint8_t rom_data[PICO_ROM_SIZE] __attribute__((aligned(4)));
};

typedef struct pico_rom pico_rom_t;
bool is_valid_rom_addr(pico_addr addr);

bool init_boot_rom_vector(struct pico_bootrom_vector *target, struct pico_cpu *cpu);
int init_rom(struct pico_cpu *cpu);
#endif