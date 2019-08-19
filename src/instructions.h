#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/* Instructions */
typedef enum 
{
    HLT,
    PSH, 
    POP,
    ADD, 
    MUL, 
    DIV, 
    SUB, 
    SLT, 
    MOV, 
    SET, 
    LOG, 
    IF,  
    IFN, 
    GLD, 
    GPT,
    NOP
} Instructions;

/*
ToDo Add:
MOD - modulus
NOT - Logic gate
XOR - ""
OR  - ""
AND - ""
*/

#endif