#include "tracer.h"

//Register declaration
#ifdef __x86_64__
const char* REGISTERS[] = {"r15", "r14", "r13", "r12", "rbp", "rbx", "r11", "r10", "r9", "r8", "rax", "rcx", "rdx", "rsi", "rdi", "orig_rax", "rip", "cs", "eflags", "rsp", "ss", "fs_base", "gs_base", "ds", "es", "fs", "gs"};
#else
const char* REGISTERS[] = {"ebx", "ecx", "edx", "esi", "edi", "ebp", "eax", "xds", "xes", "xfs", "xgs", "orig_eax", "eip", "xcs", "eflags", "esp", "xss"};
#endif

void printMem(pid_t child_pid, char** arg_lst){
	//If the print hasn't specified the type of print, don't execute the remaining code
	if(strlen(arg_lst[0]) == 1 || strlen(arg_lst[0]) == 5){
		printf("Please put a character after your print command to denote the format.\n");
		return;
	}

	//If we don't have an address
	if(arg_lst[1] == NULL){
		printf("Not enough arguments! Use the format:\np<c/d/x> <addr>\n");
		return;
	}

	//Get the format character
	char format_char = arg_lst[0][strlen(arg_lst[0])-1];

	//If we're not printing a string and we have an accepted format
	if(format_char == 'c' || format_char == 'd' || format_char == 'x'){
		char* format = malloc(4);
		sprintf(format, "%%%c\n", format_char);
		unsigned long long result = ptrace(PTRACE_PEEKDATA, child_pid, strtoul(arg_lst[1], NULL, 16), NULL);
		printf(format, result);
		free(format);
	}
	//Otherwise the format is unrecognized
	else{
		printf("Invalid format!\n");
	}
}

void writeMem(pid_t child_pid, char** arg_lst){
	//If we have an address and data
	if(arg_lst[2] == NULL){
		printf("Not enough arguments! Use the format:\nw <addr> <val>\n");
		return;
	}

	//If the address is word aligned
	long addr = strtoul(arg_lst[1], NULL, 16);
	if(addr % 4 != 0){
		printf("Address isn't aligned; cannot write to that location.\n");
		return;
	}

	//Update data
	if(ptrace(PTRACE_POKEDATA, child_pid, addr, strtoul(arg_lst[2], NULL, 16))){
		printf("Failed to place the data in tracee memory.\n");
	}
	else{
		printf("Placed the data in tracee memory.\n");
	}
}

void printRegs(pid_t child_pid, char* reg_name){
	//Get the registers
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

	//Iterate through and print the registers (or the register you're looking for)
	int num_regs = sizeof(struct user_regs_struct) / sizeof(unsigned long int);
	#ifdef __x86_64__
	long long int* reg_iter = (long long int*)&regs;
	#else
	long int* reg_iter = (long int*)&regs;
	#endif
	for(int i = 0; i < num_regs; i++){
		if(reg_name == NULL || strcmp(reg_name, REGISTERS[i]) == 0){
			#ifdef __x86_64__
			printf("%s: 0x%llx\n", REGISTERS[i], *(reg_iter + i));
			#else
			printf("%s: 0x%lx\n", REGISTERS[i], *(reg_iter + i));
			#endif
		}
	}
}

void flashRegs(pid_t child_pid, char** arg_lst){
	if(arg_lst[1] == NULL){
		printf("Please specify the register you wish to flash!\n");
		return;
	}
	else if(arg_lst[2] == NULL){
		printf("Please specify the number you wish to flash to the register!\n");
		return;
	}

	//Get the registers
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

	//Set the new register value
	int num_regs = sizeof(struct user_regs_struct) / sizeof(unsigned long int);
	#ifdef __x86_64__
	long long int* reg_iter = (long long int*)&regs;
	#else
	long int* reg_iter = (long int*)&regs;
	#endif
	for(int i = 0; i < num_regs; i++){
		if(arg_lst[1] == NULL || strcmp(arg_lst[1], REGISTERS[i]) == 0){
			*(reg_iter + i) = strtoul(arg_lst[2], 0, 16);
		}
	}

	//Update registers
	ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
}

int continueProgram(pid_t child_pid){
	struct user_regs_struct* regs = (struct user_regs_struct*)malloc(sizeof(struct user_regs_struct));
	char break_hit = 0;
	int status;
	do{
		//Step once
		if(ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL)){
			free(regs);
			return -1;
		}
		waitpid(child_pid, &status, 0);
		
		//Get the rip register
		ptrace(PTRACE_GETREGS, child_pid, NULL, regs);
		#ifdef __x86_64__
		long long int instr = regs->rip;
		#else
		long int instr = regs->eip;
		#endif

		//Go through each breakpoint to see if we get a match
		for(int i = 0; i < break_num; i++){
			if(breakpoints[i] != 0x0 && breakpoints[i] == instr){
				#ifdef __x86_64__
				printf("Breakpoint %d hit: %llu\n", i, breakpoints[i]);
				#else
				printf("Breakpoint %d hit: %lu\n", i, breakpoints[i]);
				#endif
				free(regs);
				return 0;
			}
		}
	} while(break_hit == 0);
	free(regs);
	return 0;
}

int traceProcess(pid_t child_pid){
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
		arg_lst = getArgs(input, " \n");
		if(arg_lst == NULL){
			printf("Improperly terminated argument!\n");
			arg_lst = &input;
			continue;
		}

		//Parsing the commands
		//If you want to step to the next instruction
		if(strcmp(arg_lst[0], "n") == 0 || strcmp(arg_lst[0], "next") == 0){
			if(ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL)){
				printf("Error stepping to the next instruction.\n");
				return -1;
			}
			printf("Stepping the program...\n");
		}
		//If you want to continue to the next syscall
		else if(strcmp(arg_lst[0], "s") == 0 || strcmp(arg_lst[0], "syscall") == 0){
			if(ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL)){
				printf("Error continuing the program to syscall.\n");
				return -1;
			}
			printf("Continuing the program to next syscall...\n");
		}
		//If you want to continue the program
		else if(strcmp(arg_lst[0], "c") == 0 || strcmp(arg_lst[0], "continue") == 0){
			printf("Continuing the program...\n");
			if(continueProgram(child_pid)){
				return 0;
			}
			continue;
		}
		//If you want to print a piece of memory
		else if(arg_lst[0][0] == 'p' || strncmp(arg_lst[0], "print", 5) == 0){
			printMem(child_pid, arg_lst);
			continue;
		}
		else if(strcmp(arg_lst[0], "w") == 0 || strcmp(arg_lst[0], "write") == 0){
			writeMem(child_pid, arg_lst);
			continue;
		}
		else if(strcmp(arg_lst[0], "r") == 0 || strcmp(arg_lst[0], "registers") == 0){
			printRegs(child_pid, arg_lst[1]);
			continue;
		}
		else if(strcmp(arg_lst[0], "f") == 0 || strcmp(arg_lst[0], "flash") == 0){
			flashRegs(child_pid, arg_lst);
			continue;
		}
		else if(strcmp(arg_lst[0], "b") == 0 || strcmp(arg_lst[0], "breakpoint") == 0){
			setBreakpoint(arg_lst);
			continue;
		}
		else if(strcmp(arg_lst[0], "d") == 0 || strcmp(arg_lst[0], "delete") == 0){
			deleteBreakpoint(arg_lst);
			continue;
		}
		else if(strcmp(arg_lst[0], "q") == 0 || strcmp(arg_lst[0], "quit") == 0){
			free(input);
			free(arg_lst);
			return 0;
		}
		else{
			printf("Unrecognized command!\n");
			continue;
		}
		waitpid(child_pid, &status, 0);
	}
	free(input);
	return 0;
}
