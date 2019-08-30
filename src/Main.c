#include "instructions.h"
#include "registers.h"
#include "user_interface.h"
#include "logger.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

/* Stack */
#define STACK_SIZE 256
int stack[STACK_SIZE];

/* Registers */
int registers[REGISTER_SIZE];

/* Instructions */
int *instructions;
int instruction_count = 0;
int instruction_space = 4;

/* Quick Access hack for SP & IP */
#define SP (registers[SP])
#define IP (registers[IP])

/* User Interface Windows */
WINDOW *instruction_window;
WINDOW *stack_window;
WINDOW *register_window;

void eval(int instr) 
{
    registers[JMP] = false;
    switch (instr) 
    {
        case HLT: 
        {
            registers[RUN] = false;
            break;
        }
        case PSH: 
        {
            SP = SP + 1;
            IP = IP + 1;
            stack[SP] = instructions[IP];
            break;
        }
        case POP: 
        {
            SP = SP - 1;
            break;
        }
        case ADD: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] + registers[A];
            stack[SP] = registers[C];
            break;
        }
        case MUL: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] * registers[A];
            stack[SP] = registers[C];
            break;
        }
        case DIV: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] / registers[A];
            stack[SP] = registers[C];
            break;
        }
        case SUB: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] - registers[A];
            stack[SP] = registers[C];
            break;
        }
        case MOD:
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[A] % registers[B];
            stack[SP] = registers[C];
            break;
        }
        case SLT: 
        {
            SP = SP - 1;
            stack[SP] = registers[instructions[IP + 1]] < registers[instructions[IP + 2]];
            IP = IP + 2;
            break;
        }
        case MOV: 
        {
            registers[instructions[IP + 2]] = registers[instructions[IP + 1]];
            IP = IP + 2;
            break;
        }
        case SET: 
        {
            registers[instructions[IP + 1]] = instructions[IP + 2];
            IP = IP + 2;
            break;
        }
        case LOG: 
        {
            if(!log_append_int(instructions[IP + 1]))
                printf("Failed to append log\n");
                
            IP = IP + 1;
            break;
        }
        case LOGR:
        {
            if(!log_append_int(registers[instructions[IP + 1]]))
                printf("Failed to append log\n");

            IP = IP + 1;
            break;
        }
        case LOGS:
        {
            if(!log_append_int(stack[SP - registers[instructions[IP + 1]]]))
                printf("Failed to append log\n");

            IP = IP + 1;
            break;
        }
        case IF: 
        {
            if (registers[instructions[IP + 1]] == instructions[IP + 2]) 
            {
                IP = instructions[IP + 3];
                registers[JMP] = true;
            }
            else
                IP = IP + 3;
            break;
        }
        case IFN: 
        {
            if (registers[instructions[IP + 1]] != instructions[IP + 2]) 
            {
                IP = instructions[IP + 3];
                registers[JMP] = true;
            }
            else 
                IP = IP + 3;
            break;
        }
        case GLD: 
        {
            SP = SP + 1;
            IP = IP + 1;
            stack[SP] = registers[instructions[IP]];
            break;
        }
        case GPT: 
        {
            registers[instructions[IP + 1]] = stack[SP];
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
            sprintf(error_str, "%s: %d", "!Error! Unknown Instruction", instr);
            log_append_string(error_str);
            break;
        }
    }
}

bool init_instructions(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        log_append_string("!Error! could not read file");
        return false;
    }

    // allocate space for instructions
    instructions = malloc(sizeof(*instructions) * instruction_space); // 4 instructions

    // read the data from the file
    int num;
    int i = 0;
    while (fscanf(file, "%d", &num) > 0) 
    {
        instructions[i] = num;
        i++;
        if (i >= instruction_space) 
        {
            instruction_space *= 2;
            instructions = realloc(instructions, sizeof(*instructions) * instruction_space); // double size
        }
    }
    instruction_count = i; // Instructions read
    fclose(file);

    return true;
}

bool init_user_interface()
{
    initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    const int buffer_windows_titles_y = 3;
    const int buffer_windows_y = 4;
    
    // Title
    mvaddstr(1, 43, "Virtual Machine");

    // Creates outline of buffer Windows
    instruction_window = ui_create_new_window(24, 8, buffer_windows_y, 1);
    mvaddstr(buffer_windows_titles_y, 2, "Instr."); 
    stack_window = ui_create_new_window(24, 82, buffer_windows_y, 9);
    mvaddstr(buffer_windows_titles_y, 48, "Stack");
    register_window = ui_create_new_window(24, 8, buffer_windows_y, 91);
    mvaddstr(buffer_windows_titles_y, 93, "Reg.");

    refresh();
    return true;
}

int main(int argc, char** argv) 
{
    if (argc <= 1) 
    {
        log_append_string("!Error! no input files");
        return -1;
    }

    // Initalise instruction set
    if(!init_instructions(argv[1]))
    {
        log_append_string("!Error! could not initalise instructions");
        return -1;
    }

    // Initialize stack pointer
    SP = -1;

    // Initalize user interface
    if(!init_user_interface())
    {
        log_append_string("!Error! could not initalise user interface");
        return -1;
    }

    // Start tick
    registers[RUN] = true;
    while (registers[RUN] && IP < instruction_count) 
    {
        //sleep(1);

        // Evaluate instruction at IP.
        eval(instructions[IP]);

        // Update UI to show inner workings.
        if(!ui_update_instructions(instruction_window, instructions, instruction_count, IP) || 
           !ui_update_registers(register_window, registers) || 
           !ui_update_stack(stack_window, stack))
        {
            log_append_string("!Error! error updating UI (Windows not initalized?)");
            return -1;
        }

        if(!registers[JMP])
            IP = IP + 1;
    }

    // Pause
    getch();

    // clean up
    free(instructions);
    endwin(); // end UI

    return 0;
}