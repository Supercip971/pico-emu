#include "config.h"
#include "instruction.h"
#include "pico_cpu.h"
#include "raspberry_pi_pico.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("you must provide a utf2 file to picemu \n");
        printf("picemu {flags} [utf2_file]\n");
        return -1;
    }
    printf("starting picemu... \n");
    printf("loading bootrom at 0x%x-0x%x ... \n", PICO_ROM_ADDR, PICO_ROM_ADDR + PICO_ROM_SIZE);

    struct pico_cpu pico;
    if (init_rom(&pico.memory.boot_rom) != 0)
    {
        return -2;
    }

    char *utf2_path = argv[1];
    printf("loading sram with file %s at 0x%x-0x%x... \n", utf2_path, PICO_SRAM_ADDR, PICO_SRAM_ADDR + PICO_SRAM_SIZE);

    if (init_sram(&pico.memory.sram, utf2_path) != 0)
    {
        return -3;
    }
    printf("resetting pico cpu... \n");
    reset_cpu(&pico);
    start_cpu(&pico, 0);
    for (int i = 0; i < 7; i++)
    {
        run_instruction(&pico);
    }
    return 0;
}
