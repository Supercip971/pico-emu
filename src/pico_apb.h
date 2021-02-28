#ifndef PICO_APB_H
#define PICO_APB_H
#include <stdbool.h>
#include <stdint.h>

#include "config.h"
#include "pico_cpu.h"

#define REG_TABLE_ENTRY_COUNT sizeof(abp_reg_table) / sizeof(struct APB_raw_Register)
typedef struct pico_cpu pico_cpu;

#define APB_OFFSET_NORMAL 0x0
#define APB_OFFSET_XOR (1 << 12)
#define APB_OFFSET_SET (2 << 12)
#define APB_OFFSET_CLR (APB_OFFSET_XOR | APB_OFFSET_SET)

#define APB_NORMAL_RW 0
#define APB_XOR_RW 1
#define APB_SET_RW 2
#define APB_CLR_RW 3

#define APB_NU 0b00              // null
#define APB_RO 0b01              // read only
#define APB_WO 0b10              // write only
#define APB_RW (APB_RO | APB_WO) // read/write

struct APB_raw_Register
{
    const char *name;
    pico_addr base;
    pico_addr size;

    int (*read_handler)(struct APB_raw_Register *, struct pico_cpu *, uint32_t *, pico_addr);
    int (*write_handler)(struct APB_raw_Register *, struct pico_cpu *, const uint32_t, pico_addr);
};

int abp_null_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr);
int abp_null_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr);

int abp_syscfg_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr);
int abp_syscfg_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr);

int abp_vreg_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr);
int abp_vreg_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr);

int abp_clock0_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr);
int abp_clock0_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr);

int watchdog_read(struct APB_raw_Register *self, struct pico_cpu *cpu, uint32_t *target, pico_addr addr);
int watchdog_write(struct APB_raw_Register *self, struct pico_cpu *cpu, const uint32_t target, pico_addr addr);

struct APB_raw_Register *select_register(pico_addr addr);
int read_abp_32(pico_addr raw_addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self);
int write_abp_32(pico_addr raw_addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self);
int init_apb(struct pico_cpu *cpu);
int reset_abp_voltage_reg(struct APB_Voltage_register *reg);

#endif