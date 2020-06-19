#include "sketch.h"

char** get_args(char* str, char* delims){
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
	char** arg_lst;

	//While exit command not inputted
	do {
		//Clear input buffer
		memset(input, 0, MAX_INPUT_SIZE);

		//Get line of input
		printf(" > ");
		fgets(input, MAX_INPUT_SIZE, stdin);
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
