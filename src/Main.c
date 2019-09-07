#include "virtual_machine.h"
#include "user_interface.h"
#include "logger.h"
#include <unistd.h>

int main(int argc, char** argv) 
{
    // There must be an input file.
    if (argc <= 1) 
    {
        logger_log_string("[ERROR] no input files");
        return -1;
    }

    // Initalize Virtual Machine.
    if(vm_initalize(argv[1]) != 0)
    {
        logger_log_string("[ERROR] Could not initalize virtual machine.");
        return -1;
    }

    // Initialize the user interface.
    if(ui_initalize() != 0)
    {
        logger_log_string("[ERROR] Could not initalize user interface");
        return -1;
    }

    // Run.
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

        // Check for exit key.
        char user_input = getch();
        if(user_input == 'q')
            break;
    }

    // Shut it down!
    vm_shutdown();
    ui_shutdown();
    return 0;
}