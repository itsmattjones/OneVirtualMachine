#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <curses.h>

/* Properties */
WINDOW *ui_instruction_window;
WINDOW *ui_stack_window;
WINDOW *ui_register_window;

/* Functionality */
WINDOW *ui_create_new_window(int height, int width, int start_y, int start_x);
int ui_initalize();
int ui_tick();
void ui_shutdown();
int ui_update_instructions();
int ui_update_stack();
int ui_update_registers();

#endif