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
    if ((addr - PICO_ROM_ADDR) > PICO_ROM_SIZE)
    {
        return false;
    }
    return true;
}

bool read_rom_8(pico_rom_t *rom, pico_addr raw_addr, uint8_t *target)
{

    uint8_t *v = rom->rom_data + (raw_addr);
    *target = *v;
    return true;
}
bool read_rom_16(pico_rom_t *rom, pico_addr raw_addr, uint16_t *target)
{
    if (raw_addr % 2 != 0)
    {
        printf("warning: trying to read 16bit without aligned addr: 0x%x", raw_addr);
    }
    uint16_t *v = (uint16_t *)(rom->rom_data + (raw_addr));
    *target = *v;
    return true;
}
bool read_rom_32(pico_rom_t *rom, pico_addr raw_addr, uint32_t *target)
{
    if (raw_addr % 4 != 0)
    {
        printf("warning: trying to read 32bit without aligned addr: 0x%x", raw_addr);
    }
    uint32_t *v = (uint32_t *)(rom->rom_data + (raw_addr));
    *target = *v;
    return true;
}

int init_rom(pico_rom_t *rom)
{

    FILE *f = fopen("bootrom", "rb");
    if (ferror(f))
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

    fread(rom->rom_data, 1, fsize, f);
    fclose(f);
    return 0;
}

bool init_boot_rom_vector(struct pico_bootrom_vector *target, pico_rom_t *rom)
{
    size_t length_to_read = sizeof(struct pico_bootrom_vector);
    uint8_t *raw_target = malloc(sizeof(struct pico_bootrom_vector));
    for (int i = 0; i < length_to_read; i++)
    {
        read_rom_8(rom, i, raw_target + i);
    }
    memcpy(target, raw_target, length_to_read);
    free(raw_target);
    return true;
}