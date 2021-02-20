#ifndef PICO_SIO_H
#define PICO_SIO_H
#include "config.h"
#include "pico_cpu.h"
#include <stdbool.h>
#include <stdint.h>

#define SIO_NU 0b00                                     // null
#define SIO_RO 0b01                                     // read only
#define SIO_WO 0b10                                     // write only
#define SIO_RW (SIO_REG_READ_ONLY | SIO_REG_WRITE_ONLY) // read/write

struct SIO_Register
{
    const char *name;
    int (*read_handler)(struct SIO_Register *, struct pico_cpu *, uint32_t *);
    int (*write_handler)(struct SIO_Register *, struct pico_cpu *, const uint32_t);
};

int init_sio(struct pico_cpu *cpu);
int null_write(struct SIO_Register *self, struct pico_cpu *cpu, const uint32_t target);
int null_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target);
int cpuid_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target);
int read_sio_32(pico_addr raw_addr, struct pico_cpu *cpu, uint32_t *target, struct memory_region *self);
int write_sio_32(pico_addr raw_addr, struct pico_cpu *cpu, const uint32_t target, struct memory_region *self);

#endif