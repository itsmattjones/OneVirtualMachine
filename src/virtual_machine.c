#include "virtual_machine.h"
#include "instructions.h"
#include "logger.h"
#include <stdlib.h>

// Quick access (hack)...
#define SP (vm_registers[SP])
#define IP (vm_registers[IP])

int vm_evaluate(int instruction) 
{
    vm_registers[JMP] = false;
    switch (instruction) 
    {
        case HLT: 
        {
            vm_registers[RUN] = false;
            break;
        }
        case PSH: 
        {
            SP = SP + 1;
            IP = IP + 1;
            vm_stack[SP] = vm_instructions[IP];
            break;
        }
        case POP: 
        {
            SP = SP - 1;
            break;
        }
        case ADD: 
        {
            vm_registers[A] = vm_stack[SP];
            SP = SP - 1;
            vm_registers[B] = vm_stack[SP];
            vm_registers[C] = vm_registers[B] + vm_registers[A];
            vm_stack[SP] = vm_registers[C];
            break;
        }
        case MUL: 
        {
            vm_registers[A] = vm_stack[SP];
            SP = SP - 1;
            vm_registers[B] = vm_stack[SP];
            vm_registers[C] = vm_registers[B] * vm_registers[A];
            vm_stack[SP] = vm_registers[C];
            break;
        }
        case DIV: 
        {
            vm_registers[A] = vm_stack[SP];
            SP = SP - 1;
            vm_registers[B] = vm_stack[SP];
            vm_registers[C] = vm_registers[B] / vm_registers[A];
            vm_stack[SP] = vm_registers[C];
            break;
        }
        case SUB: 
        {
            vm_registers[A] = vm_stack[SP];
            SP = SP - 1;
            vm_registers[B] = vm_stack[SP];
            vm_registers[C] = vm_registers[B] - vm_registers[A];
            vm_stack[SP] = vm_registers[C];
            break;
        }
        case MOD:
        {
            vm_registers[A] = vm_stack[SP];
            SP = SP - 1;
            vm_registers[B] = vm_stack[SP];
            vm_registers[C] = vm_registers[A] % vm_registers[B];
            vm_stack[SP] = vm_registers[C];
            break;
        }
        case SLT: 
        {
            SP = SP - 1;
            vm_stack[SP] = vm_registers[vm_instructions[IP + 1]] < vm_registers[vm_instructions[IP + 2]];
            IP = IP + 2;
            break;
        }
        case MOV: 
        {
            vm_registers[vm_instructions[IP + 2]] = vm_registers[vm_instructions[IP + 1]];
            IP = IP + 2;
            break;
        }
        case SET: 
        {
            vm_registers[vm_instructions[IP + 1]] = vm_instructions[IP + 2];
            IP = IP + 2;
            break;
        }
        case LOG: 
        {
            if(!logger_log_int(vm_instructions[IP + 1]))
                printf("Failed to append log\n");
                
            IP = IP + 1;
            break;
        }
        case LOGR:
        {
            if(!logger_log_int(vm_registers[vm_instructions[IP + 1]]))
                printf("Failed to append log\n");

            IP = IP + 1;
            break;
        }
        case LOGS:
        {
            if(!logger_log_int(vm_stack[SP - vm_registers[vm_instructions[IP + 1]]]))
                printf("Failed to append log\n");

            IP = IP + 1;
            break;
        }
        case IF: 
        {
            if (vm_registers[vm_instructions[IP + 1]] == vm_instructions[IP + 2]) 
            {
                IP = vm_instructions[IP + 3];
                vm_registers[JMP] = true;
            }
            else
                IP = IP + 3;
            break;
        }
        case IFN: 
        {
            if (vm_registers[vm_instructions[IP + 1]] != vm_instructions[IP + 2]) 
            {
                IP = vm_instructions[IP + 3];
                vm_registers[JMP] = true;
            }
            else 
                IP = IP + 3;
            break;
        }
        case GLD: 
        {
            SP = SP + 1;
            IP = IP + 1;
            vm_stack[SP] = vm_registers[vm_instructions[IP]];
            break;
        }
        case GPT: 
        {
            vm_registers[vm_instructions[IP + 1]] = vm_stack[SP];
            IP = IP + 1;
            break;
        }
        case NOP: 
        {
            break;
        }
        default:
        {
            char error_str[32];
            sprintf(error_str, "%s: %d", "[ERROR] Unknown Instruction", instruction);
            logger_log_string(error_str);
            break;
        }
    }
    return 0; 
}

int vm_tick()
{
    // First evaluate any remaining vm_instructions.
    if(IP < vm_instruction_count)
    {
        vm_evaluate(vm_instructions[IP]);
        if(!vm_registers[JMP])
            IP = IP + 1;
    }

    return 0;
}

int vm_load_instructions(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        logger_log_string("[ERROR] could not read file");
        return -1;
    }

    // Allocate space for vm_instructions
    vm_instructions = malloc(sizeof(*vm_instructions) * vm_instruction_space); // 4 vm_instructions

    // Read the data from the file
    int num;
    int i = 0;
    while (fscanf(file, "%d", &num) > 0) 
    {
        vm_instructions[i] = num;
        i++;
        if (i >= vm_instruction_space) 
        {
            vm_instruction_space *= 2;
            vm_instructions = realloc(vm_instructions, sizeof(*vm_instructions) * vm_instruction_space); // double size
        }
    }
    vm_instruction_count = i;
    fclose(file);

    return 0;
}

int vm_initalize(char* filename) 
{
    // Local Varibles
    vm_instruction_count = 0;
    vm_instruction_space = 4;

    // There must be an input file.
    if (!filename) 
    {
        logger_log_string("[VM-ERROR] no input files");
        return -1;
    }

    if(vm_load_instructions(filename) != 0)
    {
        logger_log_string("[VM-ERROR] could not initalise vm_instructions");
        return -1;
    }

    SP = -1;
    return 0;
}

void vm_shutdown()
{
    free(vm_instructions);
}