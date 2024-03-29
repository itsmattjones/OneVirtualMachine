#include "user_interface.h"
#include "virtual_machine.h"
#include "logger.h"
#include <curses.h>

WINDOW *ui_create_new_window(int height, int width, int start_y, int start_x)
{	
    WINDOW *local_win;
	local_win = newwin(height, width, start_y, start_x);
	box(local_win, 0 , 0); // default characters for the v and h lines
	wrefresh(local_win); // Show that box
	return local_win;
}

int ui_initalize()
{
    window_height = 24;
    window_start_y_idx = 4;
    window_title_start_y_idx = 3;

    // Initalize and setup ncurses.
    initscr();  // Init
    cbreak();   // Disable line buffering
    noecho();   // Disable getch echoing
    timeout(0); // Non-blocking getch.
    clear();
    refresh();

    // Display base user interface layout.
    mvaddstr(1, 43, "Virtual Machine");
    ui_instruction_window = ui_create_new_window(window_height, 8, window_start_y_idx, 1);
    mvaddstr(window_title_start_y_idx, 2, "Instr."); 
    ui_stack_window = ui_create_new_window(window_height, 82, window_start_y_idx, 9);
    mvaddstr(window_title_start_y_idx, 48, "Stack");
    ui_register_window = ui_create_new_window(window_height, 8, window_start_y_idx, 91);
    mvaddstr(window_title_start_y_idx, 93, "Reg.");

    refresh();
    return 0;
}

int ui_update_instructions()
{
    if(ui_instruction_window == NULL)
        return -1;

    int curr_instruction = vm_registers[IP];
    for(int curr_line = 1; curr_line < (getmaxy(ui_instruction_window) - 1); curr_line++)
    {
        if(curr_instruction < vm_instruction_count)
        {
            // Output instruction if one's available.
            char instruction_str[12];
            sprintf(instruction_str, "%d", vm_instructions[curr_instruction]);
            mvwaddstr(ui_instruction_window, curr_line, 1, instruction_str);
            wrefresh(ui_instruction_window);
        }
        else
        {
            // Clear line hack for null instructions (hack due to window width).
            mvwaddstr(ui_instruction_window, curr_line, 1, "     ");
            wrefresh(ui_instruction_window);
        }
        curr_instruction++;
    }
    return 0;
}

int ui_update_stack()
{
    if(ui_stack_window == NULL)
        return -1;

    // Output values of entire stack.
    int stack_idx = 0;
    int curr_col = 2;
    while(curr_col <= 80)
    {
        for(int line = 1; line < (getmaxy(ui_stack_window) - 1); line++)
        {
            char stack_str[12];
            sprintf(stack_str, "0x%04d ", vm_stack[stack_idx]);
            mvwaddstr(ui_stack_window, line, curr_col, stack_str);
            wrefresh(ui_stack_window);
            stack_idx++;
        }
        curr_col += 8;
    }
    return 0;
}

int ui_update_registers()
{
    if(ui_register_window == NULL)
        return -1;

    // Output the value of each register.
    for(int i = 0; i < REGISTER_SIZE; i++)
    {
        char register_str[12];
        sprintf(register_str, "%04d", vm_registers[i]);
        mvwaddstr(ui_register_window, i + 1, 1, register_str);
        wrefresh(ui_register_window);
    }
    return 0;
}

int ui_tick()
{
    // Update each section of the UI.
    if(ui_update_instructions() != 0)
        logger_log_string("[ERROR] Could not update the Instructions Window");
    if(ui_update_registers() != 0)
        logger_log_string("[ERROR] Could not update the Registers Window");
    if(ui_update_stack() != 0)
        logger_log_string("[ERROR] Could not update the Stack Window");
    return 0;
}

void ui_shutdown()
{
    endwin();
}