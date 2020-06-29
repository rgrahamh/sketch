#include "shared.h"

char** getArgs(char* str, char* delims){
	//The new argument list
	char** arg_lst = (char**)calloc(sizeof(char*), MAX_ARGS);
	arg_lst[0] = str;
	int arg_iter = 1;

	//Calculated length pre-loop
	int len = strlen(str);
	//Tracks the delimiter number
	int delim_num = strlen(delims);
	//Use
	char capped_quote = '\0';

	//Parsing through the arguments
	for(int i = 0; i < len; i++){
		//Quote capturing
		if(str[i] == '\"' || str[i] == '\''){
			if(capped_quote == '\0'){
				capped_quote = str[i];
				continue;
			}
			else if(capped_quote == str[i]){
				capped_quote = '\0';
				continue;
			}
		}
		//Dealing with escaped characters
		else if(str[i] == '\\'){
			char* curr_addr = &(str[i]);
			strcpy(curr_addr, curr_addr+1);
			continue;
		}
		//Checking against all specified separation delimeters
		for(int j = 0; j < delim_num; j++){
			if(str[i] == delims[j] && capped_quote == '\0'){
				str[i] = '\0';

				//If there's a next argument, add it
				if(i + 1 < len){
					arg_lst[arg_iter++] = str + i + 1;
				}

				//If we go over the max iterations
				if(arg_iter > MAX_ARGS){
					return NULL;
				}
			}
		}
	}
	if(capped_quote == '\"' || capped_quote == '\''){
		return NULL;
	}

	for(int i = 0; i < arg_iter; i++){
		int arg_len = strlen(arg_lst[i]);
		if(arg_len >= 2 && ((arg_lst[i][0] == '\'' && arg_lst[i][arg_len-1] == '\'') || (arg_lst[i][0] == '\"' && arg_lst[i][arg_len-1] == '\"'))){
			arg_lst[i][arg_len-1] = '\0';
			arg_lst[i] = arg_lst[i] + 1;
		}
	}

	return arg_lst;
}

void setBreakpoint(char** break_lst){
	for(int i = 0; i < MAX_ARGS - 1 && break_lst[i] != NULL; i++){
		//If the array isn't big enough for another breakpoint
		if(break_num + 1 > break_max){
			break_max *= 2;
			#ifdef __x86_64__
			breakpoints = realloc(breakpoints, sizeof(long long int) * break_max);
			memset(breakpoints + (break_max / 2), 0, sizeof(long long int) * (break_max / 2));
			#else
			breakpoints = realloc(sizeof(long int) * break_max);
			memset(breakpoints + (break_max / 2), 0, sizeof(long int) * (break_max / 2));
			#endif
		}
		//Find a free breakpoint slot
		for(int j = 0; i < break_max; j++){
			if(breakpoints[j] == 0x0){
				breakpoints[j] = strtoul(break_lst[i], 0, 16);
				printf("Added breakpoint %d: 0x%llx\n", j, breakpoints[j]);
				break;
			}
		}
		break_num++;
	}
}

void deleteBreakpoint(char** break_lst){
	//For each passed in breakpoint
	for(int i = 0; i < MAX_ARGS - 1; i++){
		if(break_lst[i] != NULL){
			unsigned long long addr = strtoul(break_lst[i], 0, 16);
			//Remove the breakpoint
			for(int j = 0; j < break_max; j++){
				if(addr == breakpoints[j]){
					breakpoints[j] = 0x0;
					break_num--;
					printf("Removed breakpoint %d: 0x%llx\n", j, addr);
					return;
				}
			}
			printf("Breakpoint at %s not found!\n", break_lst[i]);
		}
	}
}
