#ifndef PICO_CPU_H
#define PICO_CPU_H
#include "config.h"
#include "pico_rom.h"
#include "raspberry_pi_pico.h"

#include <stdint.h>

struct APS_Register
{
    uint8_t negative_condition : 1;
    uint8_t zero_confition : 1;
    uint8_t carry_flag : 1;
    uint8_t overflow_flag : 1;

    uint8_t _reserved : 4;
    uint16_t _reserved_2;
};
#define R_REGISTER_COUNT 13
struct pico_register
{
    uint32_t R_register[R_REGISTER_COUNT]; // from R0-R12
    uint32_t SP;                           // stack pointer
    uint32_t LR;                           // link register
    uint32_t PC;                           // programm counter
};

struct pico_cpu
{
    int core_id;
    struct rasp_pico_memory memory;
    struct pico_register registers;
    struct APS_Register status;
};


void dump_cpu(const struct pico_cpu *cpu);
void reset_cpu(struct pico_cpu *cpu);
uint8_t fetch_byte(struct pico_cpu *cpu);
bool start_cpu(struct pico_cpu *cpu, int cpu_id);

#define READ_MEMORY_SUCCESS 0
#define READ_MEMORY_OOB -1
#define READ_MEMORY_ROM_ERROR -2
#define READ_MEMORY_SRAM_ERROR -3
#define READ_MEMORY_FLASH_ERROR -4
int read_memory_byte(struct pico_cpu *cpu, uint8_t *target, pico_addr addr);
int read_memory_word(struct pico_cpu *cpu, uint16_t*target, pico_addr addr);
int read_memory_dword(struct pico_cpu *cpu, uint32_t*target, pico_addr addr);
#endif