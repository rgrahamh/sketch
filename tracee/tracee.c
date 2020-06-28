#include "tracee.h"

void executeProgram(char* program_name, char** argv){
	//Allow for this process to be traced by the parent
	if(ptrace(PTRACE_TRACEME, 0, 0, 0)){
		printf("Issues allowing the child to be traced!\n");
		return;
	}

	printf("Executing the program %s\n", program_name);
	execv(program_name, argv);
}
