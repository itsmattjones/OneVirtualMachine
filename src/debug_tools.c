#include "debug_tools.h"
#include "instructions.h"
#include "registers.h"
#include <stdio.h>

void print_stack(int stack[]) 
{
    for (int i = 0; i < SP; i++) 
    {
        printf("0x%04d ", stack[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
    if (SP != 0) { printf("\n"); }
}

void print_registers(int registers[]) 
{
    printf("Register Dump:\n");
    for (int i = 0; i < REGISTER_SIZE; i++) 
    {
        printf("%04d ", registers[i]);
        if ((i + 1) % 4 == 0) { printf("\n"); }
    }
}

int find_empty_register(int registers[])
{
    for (int i = 0; i < REGISTER_SIZE; i++)
        if (i != registers[EX] && i != registers[EXA]) { return i; }
    
    return EX;
}