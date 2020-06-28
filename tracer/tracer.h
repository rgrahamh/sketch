#ifndef TRACER_H
#define TRACER_H

#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

#include "../shared/shared.h"

void printMem(pid_t child_pid, char** arg_lst);
void writeMem(pid_t child_pid, char** arg_lst);
void printRegs(pid_t child_pid, char* reg_name);

int traceProcess(pid_t child_pid);

#endif
