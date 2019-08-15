# Registers
||Type||Name||
| General Purpose     | A, B, C, D, E, F, I, J |
| Excess              | EX, EXA                |
| Instruction Pointer | IP                     |
| Stack Pointer       | SP                     |
| Running (IsRunning) | RUN                    |
| Is Jump             | JMP                    | // If the IP is assigned by jmp instructions(such as IF,IFN),it should not increase 1 any more

# Instructions
||Id||Instruction||Description||Example Usage||
|0 | HLT | Halts the program                         | hlt              |
|1 | PSH | Pushes <value> to stack                   | psh val          |
|2 | POP | Pops value from stack                     | pop              |
|3 | ADD | Adds top two values on stack              | add              |
|4 | MUL | Multiplies top two values on stack        | mul              |
|5 | DIV | Divides top two values on stack           | div              |
|6 | SUB | Subtracks top two vlaues on stack         | sub              |
|7 | SLT | Pushes (reg_a < reg_b) to stack           | slt reg_a, reg_b |
|8 | MOV | moves the value in reg_a to reg_b         | mov reg_a, reg_b |
|9 | SET | Sets the reg to a value                   | set reg, val     |
|10| LOG | Prints out something                      | log a            |
|11| IF  | If the register == value branch to the ip | if reg val ip    |
|12| IFN | If the register != val branch to the ip   | ifn reg val ip   |
|13| GLD | Loads the register to the stack           | gld reg          |
|14| GPT | Pushes top of stack to the given register | gpt reg          |
|15| NOP | Nothing                                   | nop              |
