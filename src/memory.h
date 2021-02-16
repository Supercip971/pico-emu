#ifndef MEMORY_H
#define MEMORY_H
#include "config.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
struct memory_sram
{
    uint8_t raw[PICO_SRAM_SIZE]  __attribute__((aligned(4)));
};

int init_sram(struct memory_sram *sram, const char *file_path);

bool read_sram_8(struct memory_sram *sram, const pico_addr raw_addr, uint8_t *target);
bool read_sram_16(struct memory_sram *sram, const pico_addr raw_addr, uint16_t *target);
bool read_sram_32(struct memory_sram *sram, const pico_addr raw_addr, uint32_t *target);

bool write_sram_8(struct memory_sram *sram, const pico_addr raw_addr, const uint8_t target);
bool write_sram_16(struct memory_sram *sram, const pico_addr raw_addr, const uint16_t target);
bool write_sram_32(struct memory_sram *sram, const pico_addr raw_addr, const uint32_t target);
#endif