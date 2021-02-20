#include "config.h"
#include "instruction.h"
#include "pico_cpu.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("you must provide a utf2 file to picemu \n");
        printf("picemu {flags} [utf2_file]\n");
        return -1;
    }
    
    struct pico_cpu pico;
    printf("starting picemu... \n");

    init_cpu(&pico, argv[1]);

    printf("resetting pico cpu... \n");

    reset_cpu(&pico);
    start_cpu(&pico, 0);

    while (run_instruction(&pico) == 0);

    stop_cpu(&pico);
    return 0;
}
