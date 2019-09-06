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
    MOD,
    SLT, 
    MOV, 
    SET, 
    LOG,
    LOGR,
    LOGS, 
    IF,  
    IFN, 
    GLD, 
    GPT,
    NOP
} Instructions;

#endif