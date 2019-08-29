#include "instructions.h"
#include "registers.h"
#include "debug_tools.h"
#include "user_interface.h"
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

/* Quick Access hack */
#define SP (registers[SP])
#define IP (registers[IP])
#define RUNNING (registers[RUN])
#define IS_JUMP (registers[JMP])
#define FETCH (instructions[IP])

/* User Interface Windows */
WINDOW *instruction_window;
WINDOW *stack_window;
WINDOW *register_window;
WINDOW *output_window;

void eval(int instr) 
{
    IS_JUMP = false;
    switch (instr) 
    {
        case HLT: 
        {
            RUNNING = false;
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
            // printf("LOG: %d\n", registers[instructions[IP + 1]]);
            // ToDo: Need to log to UI or something
            IP = IP + 1;
            break;
        }
        case IF: 
        {
            if (registers[instructions[IP + 1]] == instructions[IP + 2]) 
            {
                IP = instructions[IP + 3];
                IS_JUMP = true;
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
                IS_JUMP = true;
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
            // printf("Unknown Instruction %d\n", instr);
            // ToDo: Throw error, on ui?
            break;
        }
    }
}

bool init_instructions(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("error: could not read file `%s`\n", filename);
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

    // Buffer Windows
    instruction_window = create_new_window(24, 8, buffer_windows_y, 1);
    mvaddstr(buffer_windows_titles_y, 2, "Instr."); 
    stack_window = create_new_window(24, 82, buffer_windows_y, 9);
    mvaddstr(buffer_windows_titles_y, 48, "Stack");
    register_window = create_new_window(24, 8, buffer_windows_y, 91);
    mvaddstr(buffer_windows_titles_y, 93, "Reg.");

    // Output window
    output_window = create_new_window(8, 98, 30, 1);
    mvaddstr(29, 46, "Output");

    refresh();
    return true;
}

int main(int argc, char** argv) 
{
    if (argc <= 1) 
    {
        printf("error: no input files\n");
        return -1;
    }

    // initalise instruction set
    if(!init_instructions(argv[1]))
    {
        printf("error: could not initalise instructions\n");
        return -1;
    }

    // Initalize user interface
    if(!init_user_interface())
    {
        printf("error: could not initalise user interface\n");
        return -1;
    }

    // initialize stack pointer
    SP = -1;

    // Start program tick
    RUNNING = true;
    while (RUNNING && IP < instruction_count) 
    {
        //sleep(1);

        eval(FETCH);
        if(!IS_JUMP)
            IP = IP + 1;

        // Update UI
        if(!update_instructions(instruction_window, instructions, instruction_count, IP) || 
           !update_registers(register_window, registers) || 
           !update_stack(stack_window, stack))
        {
            printf("Error updating UI (Windows not initalized?)\n");
            return -1;
        }
    }

    // Pause
    getch();

    // clean up
    free(instructions);
    endwin(); // end UI

    return 0;
}