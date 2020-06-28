#ifndef SHARED_H
#define SHARED_H

#define MAX_INPUT_SIZE 4096
#define MAX_ARGS 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long long int instr_offset;
unsigned int break_num;
unsigned int break_max;
#ifdef __x86_64__
long long int* breakpoints;
#else
long int* breakpoints;
#endif

char** getArgs(char* str, char* delims);

void setBreakpoint(char** break_lst);
void deleteBreakpoint(char** break_lst);

#endif
