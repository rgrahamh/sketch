# Sketch
Sketch is an x86 Linux debugger (built for 64 or 32-bit, only tested on 64 right now) that utilizes ptrace's core functionality. Nothing too complex, mainly just trying to utilize the ptrace function calls.

## Commands
### Either state
- `b`: Add a breakpoint
- `d`: Delete a breakpoint
- `q`: Quit
### Before program execution begins:
- `o`: Do an objdump (to look at the assembly)
- `r <args>`: Execute the program with any nubmer of arguments (if you want to extend the number of possible arguments, change the constant at the top of shared.h).
### After program execution begins:
- `c`/`continue`: Continues the program's execution
- `n`/`next`: Steps the program to the next instruction
- `p<format> <addr>`/`print<format> <addr>`: Print the contents at `<addr>` with the specified format (currently, `c`, `d`, and `x` are supported)
- `f <addr> <val>`/`flash <addr> <val>`: Flashes the value specified into the register that you want
- `r <reg>`/`register <reg>`: Prints the contents of the desired register (using lowercase convention; look at `<sys/user.h>` for reference on values). If you want to print all registers, don't specify a register to print.
- `s`/`syscall`: Goes up until the next syscall

## Author's Notes
This is in very early development (I started like a week ago and am trying to build this in free time amidst IRL craziness), so don't use this for a hot minute (until I get a chance to work on it some more). Thanks!

-- rgrahamh
