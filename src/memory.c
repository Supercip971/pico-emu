

#include "memory.h"

int init_sram(struct memory_sram *sram, const char *file_path)
{
    return 0;
}
bool read_sram_8(struct memory_sram *sram, const pico_addr raw_addr, uint8_t *target)
{
    uint8_t *v = (uint8_t *)(sram->raw + (raw_addr));
    *target = *v;
    return true;
}
bool read_sram_16(struct memory_sram *sram, const pico_addr raw_addr, uint16_t *target)
{
    uint16_t *v = (uint16_t *)(sram->raw + (raw_addr));
    *target = *v;
    return true;
}
bool read_sram_32(struct memory_sram *sram, const pico_addr raw_addr, uint32_t *target)
{
    uint32_t *v = (uint32_t *)(sram->raw + (raw_addr));
    *target = *v;
    return true;
}

bool write_sram_8(struct memory_sram *sram, const pico_addr raw_addr, const uint8_t target)
{
    uint8_t *v = (uint8_t *)(sram->raw + (raw_addr));
    *v = target;
    return true;
}
bool write_sram_16(struct memory_sram *sram, const pico_addr raw_addr, const uint16_t target)
{
    uint16_t *v = (uint16_t *)(sram->raw + (raw_addr));
    *v = target;
    return true;
}
bool write_sram_32(struct memory_sram *sram, const pico_addr raw_addr, const uint32_t target)
{
    uint32_t *v = (uint32_t *)(sram->raw + (raw_addr));
    *v = target;
    return true;
    
}
