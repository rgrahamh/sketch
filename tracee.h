#ifndef TRACEE_H
#define TRACEE_H

#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_program(char* program_name, char** argv);

#endif
