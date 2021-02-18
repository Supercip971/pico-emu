#ifndef PICO_CPU_H
#define PICO_CPU_H
#include "config.h"
#include "memory_region.h"
#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>

struct APS_Register
{
    uint8_t negative_condition : 1;
    uint8_t zero_condition : 1;
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
    struct APS_Register status;
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

struct pico_cpu
{
    int core_id;
    struct pico_bootrom_vector bootrom_vec;
    struct pico_register registers;
    int region_count;
    struct memory_region_list regions;
};

int init_cpu(struct pico_cpu *cpu, char *file_path);
uint32_t *get_register(uint32_t id, struct pico_register *table);
const char *get_register_name(uint32_t id); // for debugging

void dump_cpu(const struct pico_cpu *cpu);
void reset_cpu(struct pico_cpu *cpu);
uint8_t fetch_byte(struct pico_cpu *cpu);
bool start_cpu(struct pico_cpu *cpu, int cpu_id);

#define READ_MEMORY_SUCCESS 0
#define READ_MEMORY_OOB -1
#define READ_MEMORY_WO_ERROR -2
int read_memory_byte(struct pico_cpu *cpu, uint8_t *target, pico_addr addr);
int read_memory_word(struct pico_cpu *cpu, uint16_t *target, pico_addr addr);
int read_memory_dword(struct pico_cpu *cpu, uint32_t *target, pico_addr addr);

#define WRITE_MEMORY_SUCCESS 0
#define WRITE_MEMORY_OOB -1
#define WRITE_MEMORY_RO_ERROR -2
int write_memory_byte(struct pico_cpu *cpu, uint8_t target, pico_addr addr);
int write_memory_word(struct pico_cpu *cpu, uint16_t target, pico_addr addr);
int write_memory_dword(struct pico_cpu *cpu, uint32_t target, pico_addr addr);

#endif