#ifndef PICO_ABP_REG_H
#define PICO_ABP_REG_H
#include "config.h"
struct APB_proc_config_register
{
    uint8_t proc0_halted : 1;
    uint8_t proc1_halted : 1;
    uint32_t reserved : 22;
    uint32_t proc0_dap_instance_id : 4;
    uint32_t proc1_dap_instance_id : 4;
};

struct APB_syscfg_register
{
    uint32_t core_0_nmi;
    uint32_t core_1_nmi;
    struct APB_proc_config_register proc_config;
    uint32_t proc_in_sync_bypass;
    uint32_t proc_in_sync_bypass_hi;
    uint32_t debug_force;
    uint32_t mem_power_down;
} __attribute__((packed));

struct APB_registers
{
    struct APB_syscfg_register syscfg_reg;
};
#endif