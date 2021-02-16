#ifndef PICO_ROM_H
#define PICO_ROM_H
#include "config.h"
#include <stdbool.h>
struct pico_rom
{
    uint8_t rom_data[PICO_ROM_SIZE] __attribute__((aligned(4)));
};

struct pico_bootrom_vector
{
    uint32_t initial_stack;
    uint32_t boot_rom_start;
    uint32_t nmi_handler;
    uint32_t fault_handler;
    uint8_t magic[3];
    uint8_t version; // must be 2
    uint16_t public_function_lookup_table;
    uint16_t public_data_lookup_table;
    uint16_t helper_function;
} __attribute__((packed));

typedef struct pico_rom pico_rom_t;
bool is_valid_rom_addr(pico_addr addr);
bool read_rom_8(pico_rom_t *rom, pico_addr raw_addr, uint8_t *target);
bool read_rom_16(pico_rom_t *rom, pico_addr raw_addr, uint16_t *target);
bool read_rom_32(pico_rom_t *rom, pico_addr raw_addr, uint32_t *target);

bool init_boot_rom_vector(struct pico_bootrom_vector *target, pico_rom_t *rom);
int init_rom(pico_rom_t *target);
#endif