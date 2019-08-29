#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <curses.h>

WINDOW *create_new_window(int height, int width, int start_y, int start_x);
bool update_instructions(WINDOW *window, int *instructions, int instr_count, int instr_ptr);
bool update_stack(WINDOW *window, int *stack);
bool update_registers(WINDOW *window, int *registers);

#endif