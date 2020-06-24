#ifndef TRACER_H
#define TRACER_H

#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

#include "../shared/shared.h"

void print_mem(pid_t child_pid, char** arg_lst);
void write_mem(pid_t child_pid, char** arg_lst);
void get_regs(pid_t child_pid, char* reg_name);

int trace_process(pid_t child_pid);

#endif
