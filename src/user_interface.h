#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <curses.h>

WINDOW *ui_create_new_window(int height, int width, int start_y, int start_x);
bool ui_update_instructions(WINDOW *window, int *instructions, int instr_count, int instr_ptr);
bool ui_update_stack(WINDOW *window, int *stack);
bool ui_update_registers(WINDOW *window, int *registers);

#endif