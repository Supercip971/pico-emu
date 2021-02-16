#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef uint32_t pico_addr;

// 0x0000
#define PICO_ROM_ADDR 0x0
// 16kb
#define PICO_ROM_SIZE 16384

// ~512M
#define PICO_SRAM_ADDR 0x20000000
// 264 kb
#define PICO_SRAM_SIZE 0x40000
// there are 4 pico memory bank
#define PICO_BANK_COUNT 4

// ~256M
#define PICO_FLASH_MEMORY_ADDR 0x10000000
// 2 meg
#define PICO_FLASH_MEMORY_SIZE 2097152

// 133 Mhz
#define CPU_CLOCK_SPEED 139460608

// single cycle io block
#define PICO_SIO_START 0xd0000000
#define PICO_SIO_LENGTH 0xfffffff
#endif