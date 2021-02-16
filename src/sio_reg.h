#ifndef PICO_SIO_H
#define PICO_SIO_H
#include "config.h"
#include <stdint.h>
#include "pico_cpu.h"
#include <stdbool.h>

#define SIO_NU 0b00 // null
#define SIO_RO 0b01 // read only
#define SIO_WO 0b10 // write only
#define SIO_RW (SIO_REG_READ_ONLY | SIO_REG_WRITE_ONLY) // read/write


struct SIO_Register{
    const char* name;
    uint8_t (*read_handler) (struct SIO_Register*, struct pico_cpu *, uint32_t* );
    uint8_t (*write_handler)(struct SIO_Register*, struct pico_cpu *, uint32_t);
};



bool read_sio_32(struct pico_cpu *cpu, pico_addr raw_addr, uint32_t *target);

bool write_sio_32(struct pico_cpu *cpu, pico_addr raw_addr, uint32_t target);

#endif