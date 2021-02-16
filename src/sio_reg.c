#include "sio_reg.h"
#include <stdio.h>
uint8_t null_write(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t target)
{

    printf("error: trying to not writable SIO register: %s at 0x%x \n", self->name, cpu->registers.PC);
    return 1;
}
uint8_t null_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target)
{

    printf("error: trying to not readable SIO register: %s at 0x%x \n", self->name, cpu->registers.PC);
    return 1;
}

uint8_t cpuid_read(struct SIO_Register *self, struct pico_cpu *cpu, uint32_t *target)
{
    *target = cpu->core_id;
    return 0;
}
struct SIO_Register reg_table[] = {
    {"CPUID", cpuid_read, null_write},
    {"GPIO_IN", null_read, null_write}};

bool read_sio_32(struct pico_cpu *cpu, pico_addr raw_addr, uint32_t *target)
{
    uint32_t target_register = raw_addr / 4;
    return reg_table[target_register].read_handler(&reg_table[target_register], cpu, target) == 0;
}

bool write_sio_32(struct pico_cpu *cpu, pico_addr raw_addr, uint32_t target)
{

    uint32_t target_register = raw_addr / 4;
    return reg_table[target_register].write_handler(&reg_table[target_register], cpu, target) == 0;
}
