#include "tracer.h"

int trace_process(pid_t child_pid){
	int status = 0;
	waitpid(child_pid, &status, 0);

	int instr_num = 0;
	printf("Seeing if we enter...\n");
	printf("Status: %d\n", status);
	while(!WIFEXITED(status)){
		instr_num++;
		if(ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL)){
			printf("Error getting next instruction!\n");
			return -1;
		}
		waitpid(child_pid, &status, 0);
	}
	printf("%d\n", instr_num);
	return 0;
}
