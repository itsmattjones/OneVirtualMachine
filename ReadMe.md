# OneVirtualMachine
## What's this?
This is a conceptual stack based virtual machine which fetches, decodes and executes instructions from a given file. It also provides a simple terminal based UI built with ncurses to show the process taking place.<br>

## Documentation
### Usage
Navigate to the containing directory (/src/) in a terminal window.<br>
Run the command '*make*' to build with cmake<br>
Run the command '*./OneVirtualMachine [InstructionFile]*' to run the program<br>

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
|1   | PSH         | Pushes <value> to stack                   | psh value        |
|2   | POP         | Pops value from stack                     | pop              |
|3   | ADD         | Adds top two values on stack              | add              |
|4   | MUL         | Multiplies top two values on stack        | mul              |
|5   | DIV         | Divides top two values on stack           | div              |
|6   | SUB         | Subtracks top two vlaues on stack         | sub              |
|7   | MOD         | Modulo of top two values on stack         | mod              |       
|8   | SLT         | Pushes (reg_a < reg_b) to stack           | slt reg_a, reg_b |
|9   | MOV         | moves the value in reg_a to reg_b         | mov reg_a, reg_b |
|10  | SET         | Sets the reg to a value                   | set reg, value   |
|11  | LOG         | Writes the given value to log             | log 24           |
|12  | LOGR        | Write the value of the given reg to log   | logr 1           |
|13  | LOGS        | Writes the value at top of stack - value  | logs value       |
|14  | IF          | If the register == value branch to the ip | if reg value ip  |
|15  | IFN         | If the register != val branch to the ip   | ifn reg value ip |
|16  | GLD         | Loads the register to the stack           | gld reg          |
|17  | GPT         | Pushes top of stack to the given register | gpt reg          |
|18  | NOP         | Nothing                                   | nop              |
