#include "instructions.h"
#include "registers.h"
#include "debug_tools.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Enables debug output when defined
#define DEBUG;

#define STACK_SIZE 256                // Arbituary value
static int stack[STACK_SIZE];         // Stack array
static int registers[REGISTER_SIZE];  // Registers Array

int *instructions;                    // Instructions array
int instruction_count = 0;            // Number of instructions to do
int instruction_space = 4;            // Space allocated for the instruction

/* Quick Access hack */
#define SP (registers[SP])
#define IP (registers[IP])
#define RUNNING (registers[RUN])
#define IS_JUMP (registers[JMP])
#define FETCH (instructions[IP])

void eval(int instr) 
{
    IS_JUMP = false;
    switch (instr) 
    {
        case HLT: 
        {
            RUNNING = false;
            printf("Finished Execution\n");
            break;
        }
        case PSH: 
        {
            SP = SP + 1;
            IP = IP + 1;
            stack[SP] = instructions[IP];
            #ifdef DEBUG
            printf("Pushed %d onto stack\n", instructions[IP]);
            #endif
            break;
        }
        case POP: 
        {
            SP = SP - 1;
            #ifdef DEBUG
            printf("Popped top of stack\n");
            #endif
            break;
        }
        case ADD: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] + registers[A];
            stack[SP] = registers[C];
            #ifdef DEBUG
            printf("%d + %d = %d\n", registers[B], registers[A], registers[C]);
            #endif
            break;
        }
        case MUL: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] * registers[A];
            stack[SP] = registers[C];
            #ifdef DEBUG
            printf("%d * %d = %d\n", registers[B], registers[A], registers[C]);
            #endif
            break;
        }
        case DIV: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] / registers[A];
            stack[SP] = registers[C];
            #ifdef DEBUG
            printf("%d / %d = %d\n", registers[B], registers[A], registers[C]);
            #endif
            break;
        }
        case SUB: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[B] - registers[A];
            stack[SP] = registers[C];
            #ifdef DEBUG
            printf("%d - %d = %d\n", registers[B], registers[A], registers[C]);
            #endif
            break;
        }
        case MOD:
        {
            registers[A] = stack[SP];
            SP = SP - 1;
            registers[B] = stack[SP];
            registers[C] = registers[A] % registers[B];
            stack[SP] = registers[C];
            #ifdef DEBUG
            printf("%d mod %d = %d\n", registers[A], registers[B], registers[C]);
            #endif
            break;
        }
        case SLT: 
        {
            SP = SP - 1;
            stack[SP] = registers[instructions[IP + 1]] < registers[instructions[IP + 2]];
            #ifdef DEBUG
            printf("Pushed %d < %d (%d) onto stack\n", 
                registers[instructions[IP + 1]], registers[instructions[IP + 2]], stack[SP]);
            #endif
            IP = IP + 2;
            break;
        }
        case MOV: 
        {
            registers[instructions[IP + 2]] = registers[instructions[IP + 1]];
            #ifdef DEBUG
            printf("Moved value (%d) from reg %d to reg %d\n", 
                registers[instructions[IP + 1]], instructions[IP + 1], instructions[IP + 2]);
            #endif
            IP = IP + 2;
            break;
        }
        case SET: 
        {
            registers[instructions[IP + 1]] = instructions[IP + 2];
            #ifdef DEBUG
            printf("Set reg %d to value %d\n", registers[instructions[IP + 1]], instructions[IP+2]);
            #endif
            IP = IP + 2;
            break;
        }
        case LOG: 
        {
            printf("LOG: %d\n", registers[instructions[IP + 1]]);
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
            #ifdef DEBUG
            printf("Loaded reg %d (%d) to top of stack\n", 
                instructions[IP], registers[instructions[IP]]);
            #endif
            break;
        }
        case GPT: 
        {
            registers[instructions[IP + 1]] = stack[SP];
            #ifdef DEBUG
            printf("Pushed top of stack (%d) to register %d\n",
                stack[SP], instructions[IP + 1]);
            #endif
            IP = IP + 1;
            break;
        }
        case NOP: 
        {
            #ifdef DEBUG
            printf("Do Nothing\n");
            #endif
            break;
        }
        default:
        {
            printf("Unknown Instruction %d\n", instr);
            break;
        }
    }
    #ifdef DEBUG
    printf("\nDebug Info:\n");
    printf("Stack:\n");
    print_stack(stack);
    printf("Registers:\n");
    print_registers(registers);
    #endif
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
        #ifdef DEBUG
        printf("Read instruction: %d\n", instructions[i]);
        #endif
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

    // initialize stack pointer
    SP = -1;

    // Start program tick
    RUNNING = true;
    int eval_cycle_count = 0;
    while (RUNNING && IP < instruction_count) 
    {
        // Debug information
        ++eval_cycle_count;
        #ifdef DEBUG
        printf("\n----------[EVAL CYCLE %d]----------\n", eval_cycle_count);
        #endif

        eval(FETCH);
        if(!IS_JUMP)
            IP = IP + 1;
    }

    // clean up
    free(instructions);

    return 0;
}