#ifndef PICO_APB_H
#define PICO_APB_H
#include "config.h"
#include "pico_cpu.h"
#include <stdbool.h>
#include <stdint.h>
typedef struct pico_cpu pico_cpu;

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

int init_apb(struct pico_cpu *cpu);
#endif