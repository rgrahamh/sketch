# Sketch
Sketch is a Linux debugger that utilizes ptrace's core functionality. Nothing too complex, mainly just trying to utilize the pre-existing function calls.

## Commands
### Before program execution begins:
- `r <args>`: Execute the program with any nubmer of arguments (if you want to extend the number of possible arguments, change the constant at the top of shared.h).
- `q`: Quit
### After program execution begins:
- `p<format> <addr>`/`print<format> <addr>`: Print the contents at `<addr>` with the specified format (currently, `s`, `c`, `d`, and `x` are supported)
- `n`/`next`: Steps the program to the next instruction
- `c`/`continue`: Continues the program's execution

## Author's Notes
This is in very early development (I started like a week ago and am trying to build this in free time amidst IRL craziness), so don't use this for a hot minute (until I get a chance to work on it some more). Thanks!

-- rgrahamh
