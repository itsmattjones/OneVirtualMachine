#include "virtual_machine.h"
#include "user_interface.h"
#include "logger.h"

int main(int argc, char** argv) 
{
    // There must be an input file.
    if (argc <= 1) 
    {
        logger_log_string("[ERROR] no input files");
        return -1;
    }

    if(vm_initalize(argv[1]) != 0)
    {
        logger_log_string("[ERROR] Could not initalize virtual machine.");
        return -1;
    }

    if(ui_initalize() != 0)
    {
        logger_log_string("[ERROR] Could not initalize user interface");
        return -1;
    }

    // Run
    vm_registers[RUN] = true;
    while (vm_registers[RUN]) 
    {
        sleep(1); // For dramatic effect.

        // Tick the Virtual Machine.
        if(vm_tick() != 0)
            logger_log_string("[ERROR] Could not tick virtual machine.");

        // Tick the User Interface.
        if(ui_tick() != 0)
            logger_log_string("[ERROR] Could not tick User Interface.");
    }

    vm_shutdown();
    ui_shutdown();
    return 0;
}