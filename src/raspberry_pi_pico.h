#ifndef RASPBERRY_PI_PICO_H
#define RASPBERRY_PI_PICO_H
#include "memory.h"
#include "pico_rom.h"

struct rasp_pico_memory
{
    struct pico_bootrom_vector bootrom_vec;
    struct pico_rom boot_rom;
    struct memory_sram sram;
};
#endif