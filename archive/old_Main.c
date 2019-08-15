#include <stdbool.h>

typedef enum 
{
    PSH,
    ADD,
    POP,
    SET,
    HLT
} InstructionSet;

typedef enum
{
    A, B, C, D, E, F, IP, SP,
    NUM_OF_REGISTERS
} Registers;

const int program[] = 
{   /* Array of Instructions */
    PSH, 5,
    PSH, 6,
    ADD,
    POP,
    HLT
};


#define sp (registers[SP]) /* Stack pointer, points to top of stack */

#define ip (registers[IP]) /* Instruction pointer, points to currently executing instruction */
bool running = true;
int stack[256]; /* The stack (Stacks are LIFO(Last in, Last out)) */
int registers[NUM_OF_REGISTERS]; /* Registers */

/* Fetches the currently instruction */
int fetch()
{
    return program[ip];
}

void eval(int instr)
{
    switch(instr)
    {
        case HLT:
        {
            running = false;
            break;
        }
        case PSH:
        {
            sp++;
            stack[sp] = program[++ip];
            break;
        }
        case POP:
        {
            int val_popped = stack[sp--];
            printf("popped %d\n", val_popped);
            break;
        }
        case ADD:
        {
            /* Pop values from stack and store */
            int a = stack[sp--];
            int b = stack[sp--];

            /* Add result and push back on top of stack */
            int result = b + a;
            sp++;
            stack[sp] = result;
            break;
        }
    }
}


bool running = true;
int main()
{
    while(running)
    {
        eval(fetch());
        ip++; // increment instruction pointer
    }
    return 0;
}