#ifndef TRACER_H
#define TRACER_H

#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "shared.h"

int trace_process(pid_t child_pid);

#endif
