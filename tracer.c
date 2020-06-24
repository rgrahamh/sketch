#include "tracer.h"

int trace_process(pid_t child_pid){
	int status = 0;
	waitpid(child_pid, &status, 0);

	char* input = (char*)calloc(MAX_INPUT_SIZE, sizeof(char));
	char** arg_lst = NULL;

	//Continue the program while it hasn't exited
	while(!WIFEXITED(status)){
		//Get line of input
		printf("-> ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		if(arg_lst != NULL){
			free(arg_lst);
		}
		arg_lst = get_args(input, " \n");
		if(arg_lst == NULL){
			printf("Improperly terminated argument!\n");
			arg_lst = &input;
			continue;
		}

		//Parsing the commands
		//If you want to step the program
		if(strcmp(arg_lst[0], "n") == 0 || strcmp(arg_lst[0], "next")){
			if(ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL)){
				printf("Error stepping to the next instruction.\n");
				return -1;
			}
			printf("Stepping the program...");
		}
		//If you want to continue the program
		else if(strcmp(arg_lst[0], "c") == 0 || strcmp(arg_lst[0], "continue") == 0){
			if(ptrace(PTRACE_CONT, child_pid, NULL, NULL)){
				printf("Error continuing the program.\n");
				return -1;
			}
			printf("Continuing the program...");
		}
		else if(arg_lst[0][0] == 'p' || strncmp(arg_lst[0], "print", 5) == 0){
			//If the print hasn't specified the type of print, don't execute the remaining code
			if(strlen(arg_lst[0]) == 1 || strlen(arg_lst[0]) == 5){
				printf("Please put a character after your print command to denote the format.");
			}
			//If we have an address
			if(arg_lst[1] != NULL){
				//Get the format character
				char format_char = arg_lst[0][strlen(arg_lst[0])-1];

				//If we're not printing a string and we have an accepted format
				if(format_char != 's' && (format_char == 'c' || format_char == 'd' || format_char == 'x')){
					char* format = malloc(4);
					sprintf(format, "%%%c\n", format_char);
					unsigned long long result = ptrace(PTRACE_PEEKDATA, child_pid, strtoul(arg_lst[1], NULL, 16), NULL);
					printf(format, result);
				}
				//Else if we're printing a string
				else if(format_char == 's'){
					for(char* cursor = input; *cursor != '\0'; cursor++){
						printf("%c", *cursor);
					}
					printf("\n");
				}
				//Otherwise the format is unrecognized
				else{
					printf("Invalid format!\n");
				}
			}
			else{
				printf("Not enough arguments! Use the format:\np<s/c/d/x> <addr>\n");
			}
		}
		waitpid(child_pid, &status, 0);
	}
	return 0;
}
