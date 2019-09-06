#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "curses.h"
#include "registers.h"

/* Properties */
int *vm_instructions;
int vm_instruction_count;
int vm_instruction_space;
int vm_stack[256];
int vm_registers[REGISTER_SIZE];

/* Functionality */
int vm_initalize(char* filename);
void vm_shutdown();
int vm_load_instructions(char *filename);
int vm_tick();
int vm_evaluate(int instruction);

#endif