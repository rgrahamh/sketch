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

	//Setting breakpoint stuff
	break_num = 0;
	break_max = 16;
	#ifdef __x86_64__
	breakpoints = (struct breakpoint**)calloc(break_max, sizeof(struct breakpoint*));
	#endif

	//While exit command not inputted
	do {
		//Get line of input
		printf(" > ");
		fgets(input, MAX_INPUT_SIZE, stdin);
		if(arg_lst != NULL){
			free(arg_lst);
		}
		arg_lst = getArgs(input, " \n");
		if(arg_lst == NULL){
			printf("Improperly terminated argument!\n");
			arg_lst = &input;
			continue;
		}

		//Running the program
		if(strcmp(arg_lst[0], "r") == 0 || strcmp(arg_lst[0], "run") == 0){
			//Child process
			if((child_pid = fork()) == 0){
				//Start parsing after the 'r '
				arg_lst[0] = program_name;
				executeProgram(program_name, arg_lst);
			}
			//Parent process
			else{
				traceProcess(child_pid);
			}
		}
		else if(strcmp(arg_lst[0], "b") == 0 || strcmp(arg_lst[0], "breakpoint") == 0){
			setBreakpoint(arg_lst+1);
		}
		else if(strcmp(arg_lst[0], "d") == 0 || strcmp(arg_lst[0], "delete") == 0){
			deleteBreakpoint(arg_lst+1);
		}
		//Doing an objdump
		else if(strcmp(arg_lst[0], "o") == 0 || strcmp(arg_lst[0], "objdump") == 0){
			char* cmd = (char*)malloc(strlen(program_name) + 19);
			sprintf(cmd, "objdump -d %s | less", program_name);
			system(cmd);
			free(cmd);
		}
		//If we don't recognize the command
		else if(strcmp(arg_lst[0], "q") != 0 && strcmp(arg_lst[0], "quit") != 0){
			printf("Unrecognized command '%s'\n", arg_lst[0]);
		}
	} while(strcmp(arg_lst[0], "q") != 0);

	//Final frees
	if(arg_lst != NULL){
		free(arg_lst);
	}
	free(input);
	free(breakpoints);

	return 0;
}
