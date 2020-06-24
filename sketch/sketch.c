#include "sketch.h"

int main(int argc, char** argv){
	if(argc != 2){
		printf("Please use the following format:\n./sketch <program_name>\n");
		return -1;
	}

	//Tracks the child's process
	pid_t child_pid = 0;

	//Input and argument list declaration
	char* input = malloc(MAX_INPUT_SIZE);
	char* program_name = argv[1];
	char** arg_lst = NULL;

	//While exit command not inputted
	do {
		//Get line of input
		printf(" > ");
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

		//Running the program
		if(strcmp(arg_lst[0], "r") == 0){
			//Child process
			if((child_pid = fork()) == 0){
				//Start parsing after the 'r '
				arg_lst[0] = program_name;
				execute_program(program_name, arg_lst);
			}
			//Parent process
			else{
				printf("PROCESS RETURNED %d\n", trace_process(child_pid));
			}
		}
		else{
			printf("Unrecognized command '%s'\n", arg_lst[0]);
		}
	} while(strcmp(arg_lst[0], "q") != 0);

	free(input);
	return 0;
}
