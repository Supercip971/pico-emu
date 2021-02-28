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
} __attribute__((packed));

struct APB_Voltage_register_ctrl_status
{
    uint8_t enable : 1;
    uint8_t hiz : 1;
    uint8_t reserved : 2;
    uint8_t vsel : 4;
    uint8_t reserved_2 : 4;
    uint16_t rok : 1;
    uint32_t reserved_3 : 19;
} __attribute__((packed));

struct APB_Voltage_bod_register
{
    uint8_t enable : 1;
    uint8_t reserved : 3;
    uint8_t vsel : 4;
    uint32_t reserved_2 : 24;

} __attribute__((packed));
struct APB_Voltage_chip_reset_register
{
    uint8_t reserved : 8;
    uint8_t hd_por : 1;
    uint8_t reserved_1 : 7;
    uint8_t had_run : 1;
    uint8_t reserved_2 : 3;
    uint8_t HAD_psm_restart : 1; // is the last restart from the debug port
    uint8_t reserved_3 : 3;
    uint8_t psm_restart_flag : 1; // for the bootloader to boot in safe mode
    uint8_t reserved_4 : 7;
} __attribute__((packed));

struct APB_watchdog_control_register
{
    uint32_t time : 24;
    uint8_t pause_jtag : 1;
    uint8_t pause_debug_0 : 1;
    uint8_t pause_debug_1 : 1;
    uint8_t reserved : 3;
    uint8_t enable : 1;
    uint8_t trigger : 1;
} __attribute__((packed));
struct APB_watchdog_load_register
{
    uint32_t load_timer : 24;
    uint32_t reserved : 8;
} __attribute__((packed));

struct APB_watchdog_reason_register
{
    uint8_t timer : 1;
    uint8_t force : 1;
    uint32_t reserved : 30;
} __attribute__((packed));

struct APB_watchdog_tick_register
{
    uint16_t cycles : 9;
    uint8_t enable : 1;
    uint8_t running : 1;
    uint16_t count : 9;
    uint32_t reserved : 12;
} __attribute__((packed));

struct APB_Voltage_register
{
    struct APB_Voltage_register_ctrl_status control_status;
    struct APB_Voltage_bod_register bod_register;
    struct APB_Voltage_chip_reset_register reset_reg;
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

struct APB_watchdog_register
{
    struct APB_watchdog_control_register control;
    struct APB_watchdog_load_register load;
    struct APB_watchdog_reason_register reset_log;
    uint32_t scratch[8];
    struct APB_watchdog_tick_register tick;
} __attribute__((packed));
struct APB_registers
{
    struct APB_syscfg_register syscfg_reg;
    struct APB_Voltage_register voltage_reg;
    struct APB_watchdog_register watchdog_reg;
};
#endif