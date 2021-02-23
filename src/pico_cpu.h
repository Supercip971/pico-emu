#ifndef PICO_CPU_H
#define PICO_CPU_H

#include <stddef.h>
#include <stdint.h>

#include "config.h"
#include "memory_region.h"
#include "pico_apb_registers.h"

#define R_REGISTER_COUNT 13
#define READ_MEMORY_SUCCESS 0
#define READ_MEMORY_OOB -1
#define READ_MEMORY_WO_ERROR -2

#define WRITE_MEMORY_SUCCESS 0
#define WRITE_MEMORY_OOB -1
#define WRITE_MEMORY_RO_ERROR -2

union raw_special_instruction{
    struct{
        uint32_t low : 3;
        uint32_t high : 5;

    };
    uint32_t raw;
}__attribute__((packed));
struct special_register{
    uint32_t APSR;
    uint32_t IAPSR;
    uint32_t EAPSR;
    uint32_t XPSR;
    uint32_t unused;
    uint32_t IPSR;
    uint32_t EPSR;
    uint32_t IEPSR;
    uint32_t MSP;
    uint32_t PSP;
    uint32_t PRIMASK;
    uint32_t CONTROL;
};
struct APS_Register
{
    uint8_t negative_condition : 1;
    uint8_t zero_condition : 1;
    uint8_t carry_flag : 1;
    uint8_t overflow_flag : 1;
    uint8_t _reserved : 4;
    uint16_t _reserved_2;
};

struct pico_register
{
    uint32_t R_register[R_REGISTER_COUNT]; // from R0-R12
    uint32_t SP;                           // stack pointer
    uint32_t LR;                           // link register
    uint32_t PC;                           // programm counter

    struct APS_Register status;
    struct special_register special_reg;
    
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
    int region_count;

    struct memory_region_list regions;
    struct pico_bootrom_vector bootrom_vec;
    struct pico_register registers;
    struct APB_registers apb_register;
    bool privileged;
};

int init_cpu(struct pico_cpu *cpu, char *file_path);
uint32_t *get_register(uint32_t id, struct pico_register *table);
const char *get_register_name(uint32_t id); // for debugging

uint32_t *get_special_register(uint32_t id, struct pico_register *table);
const char *get_special_register_name(uint32_t id); // for debugging


void dump_cpu(const struct pico_cpu *cpu);
void reset_cpu(struct pico_cpu *cpu);
uint8_t fetch_byte(struct pico_cpu *cpu);
void start_cpu(struct pico_cpu *cpu, int cpu_id);
void stop_cpu(struct pico_cpu *cpu);

int read_memory_byte(struct pico_cpu *cpu, uint8_t *target, pico_addr addr);
int read_memory_word(struct pico_cpu *cpu, uint16_t *target, pico_addr addr);
int read_memory_dword(struct pico_cpu *cpu, uint32_t *target, pico_addr addr);

int write_memory_byte(struct pico_cpu *cpu, uint8_t target, pico_addr addr);
int write_memory_word(struct pico_cpu *cpu, uint16_t target, pico_addr addr);
int write_memory_dword(struct pico_cpu *cpu, uint32_t target, pico_addr addr);

#endif
