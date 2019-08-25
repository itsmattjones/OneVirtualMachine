# OneVirtualMachine
## What's this?
This is a WIP experimental virtual machine which uses a stack and registers to fetch, decode and execute instructions from a file.<br>

## Documentation
### Registers
| Type                | Name                   |
|---------------------|------------------------|
| General Purpose     | A, B, C, D, E, F, I, J |
| Excess              | EX, EXA                |
| Instruction Pointer | IP                     |
| Stack Pointer       | SP                     |
| Running (IsRunning) | RUN                    |
| Is Jump             | JMP                    |

### Instructions
| Id | Instruction | Description                               | Example Usage    |
|----|-------------|-------------------------------------------|------------------|
|0   | HLT         | Halts the program                         | hlt              |
|1   | PSH         | Pushes <value> to stack                   | psh val          |
|2   | POP         | Pops value from stack                     | pop              |
|3   | ADD         | Adds top two values on stack              | add              |
|4   | MUL         | Multiplies top two values on stack        | mul              |
|5   | DIV         | Divides top two values on stack           | div              |
|6   | SUB         | Subtracks top two vlaues on stack         | sub              |
|7   | MOD         | Modulo of top two values on stack         | mod              |       
|8   | SLT         | Pushes (reg_a < reg_b) to stack           | slt reg_a, reg_b |
|9   | MOV         | moves the value in reg_a to reg_b         | mov reg_a, reg_b |
|10  | SET         | Sets the reg to a value                   | set reg, val     |
|11  | LOG         | Prints out something                      | log a            |
|12  | IF          | If the register == value branch to the ip | if reg val ip    |
|13  | IFN         | If the register != val branch to the ip   | ifn reg val ip   |
|14  | GLD         | Loads the register to the stack           | gld reg          |
|15  | GPT         | Pushes top of stack to the given register | gpt reg          |
|16  | NOP         | Nothing                                   | nop              |
