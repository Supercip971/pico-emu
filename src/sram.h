#ifndef SRAM_H
#define SRAM_H
#include "config.h"
#include "pico_cpu.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int init_sram(const char *file_path, struct pico_cpu *cpu);

#endif