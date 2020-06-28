#ifndef SHARED_H
#define SHARED_H

#define MAX_INPUT_SIZE 4096
#define MAX_ARGS 32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** getArgs(char* str, char* delims);

#endif
