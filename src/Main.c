#include "instructions.h"
#include "registers.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_SIZE 256
static int stack[STACK_SIZE];         // Stack array
static int registers[REGISTER_SIZE];  // Registers Array

int *instructions;                    // Instructions array
int instruction_count = 0;            // Number of instructions to do
int instruction_space = 4;            // Space allocated for the instruction

/* Quick ways to get SP, IP, JMP, RUN and Fetch current instruction set */
#define SP (registers[SP])
#define IP (registers[IP])
#define RUNNING (registers[RUN])
#define IS_JUMP (registers[JMP])
#define FETCH (instructions[IP])

/* prints the stack from A to B */
void print_stack() 
{
    for (int i = 0; i < SP; i++) 
    {
        printf("0x%04d ", stack[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
    if (SP != 0) { printf("\n"); }
}

void print_registers() 
{
    printf("Register Dump:\n");
    for (int i = 0; i < REGISTER_SIZE; i++) 
    {
        printf("%04d ", registers[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
}

int find_empty_register()
{
    for (int i = 0; i < REGISTER_SIZE; i++)
        if (i != registers[EX] && i != registers[EXA]) { return i; }
    
    return EX;
}

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
            /* SP = SP - 1; */

            registers[C] = registers[B] + registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d + %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case MUL: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /*SP = SP - 1;*/

            registers[C] = registers[B] * registers[A];

            /*SP = SP + 1;*/
            stack[SP] = registers[C];
            printf("%d * %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case DIV: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /* SP = SP - 1;*/

            registers[C] = registers[B] / registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d / %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case SUB: 
        {
            registers[A] = stack[SP];
            SP = SP - 1;

            registers[B] = stack[SP];
            /* SP = SP - 1; */

            registers[C] = registers[B] - registers[A];

            /* SP = SP + 1; */
            stack[SP] = registers[C];
            printf("%d - %d = %d\n", registers[B], registers[A], registers[C]);
            break;
        }
        case SLT: 
        {
            SP = SP - 1;
            stack[SP] = stack[SP+1] < stack[SP];
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
            printf("%d\n", registers[instructions[IP + 1]]);
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
        case GLD: {
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
            printf("Do Nothing\n");
            break;
        }
        default:
         {
            printf("Unknown Instruction %d\n", instr);
            break;
        }
    }
    print_stack();
    print_registers();
}

int main(int argc, char** argv) 
{
    if (argc <= 1) 
    {
        printf("error: no input files\n");
        return -1;
    }

    char *filename = argv[1]; // filename
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        printf("error: could not read file `%s`\n", filename);
        return -1;
    }

    // allocate space for instructions
    instructions = malloc(sizeof(*instructions) * instruction_space); // 4 instructions

    // read the "binary" file
    int num;
    int i = 0;
    while (fscanf(file, "%d", &num) > 0) 
    {
        instructions[i] = num;
        printf("%d\n", instructions[i]);
        i++;
        if (i >= instruction_space) 
        {
            instruction_space *= 2;
            instructions = realloc(instructions, sizeof(*instructions) * instruction_space); // double size
        }
    }
    
    // set 'instruction_count' to number of instructions read
    instruction_count = i;

    // close the file
    fclose(file);

    // initialize stack pointer
    SP = -1;

    // loop through program, but don't
    // go out of the programs bounds
    RUNNING = true;
    while (RUNNING && IP < instruction_count) 
    {
        eval(FETCH);
        if(!IS_JUMP)
            IP = IP + 1;
    }

    // clean up instructions
    free(instructions);

    return 0;
}