#include <curses.h>
#include "registers.h"

WINDOW *create_new_window(int height, int width, int start_y, int start_x)
{	
    WINDOW *local_win;
	local_win = newwin(height, width, start_y, start_x);
    // 0, 0 gives default characters for the vertical and horrizontal lines
	box(local_win, 0 , 0); 
	wrefresh(local_win); // Show that box
	return local_win;
}

bool update_instructions(WINDOW *window, int *instructions, int instr_count, int instr_ptr)
{
    if(window == NULL)
        return false;

    for(int i = 0; i < ((instr_count - instr_ptr) - 1); i++)
    {
        char instruction_str[12];
        sprintf(instruction_str, "%d", instructions[instr_ptr + i]);
        mvwaddstr(window, i + 1, 1, instruction_str);
        wrefresh(window);
    }
    return true;
}

bool update_stack(WINDOW *window, int *stack)
{
    if(window == NULL)
        return false;

    int stack_idx = 0;
    int curr_col = 2;
    while(curr_col <= 80)
    {
        for(int line = 1; line < (getmaxy(window) - 1); line++)
        {
            char stack_str[12];
            sprintf(stack_str, "0x%04d ", stack[stack_idx]);
            mvwaddstr(window, line, curr_col, stack_str);
            wrefresh(window);
            stack_idx++;
        }
        curr_col += 8;
    }

    return true;
}

bool update_registers(WINDOW *window, int *registers)
{
    if(window == NULL)
        return false;

    for(int i = 0; i < REGISTER_SIZE; i++)
    {
        char register_str[12];
        sprintf(register_str, "%04d", registers[i]);
        mvwaddstr(window, i + 1, 1, register_str);
        wrefresh(window);
    }
    return true;
}