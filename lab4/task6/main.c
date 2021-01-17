#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define PS "ps -l"

int main(int argc, char* argv[]){ 
	char command[13] = PS;
	char pid_str[6];

	pid_t pid = fork();
	if(!pid){		
		sprintf(pid_str, "%d", getpid());
		printf("Child process %s created, info (see STAT column for status information): \n", pid_str);
		strcat(command, pid_str);
		system(command);
		printf("\n");
	}
	else if(pid > 0){
		sprintf(pid_str, "%d", pid);
		strcat(command, pid_str);
		sleep(5);
		printf("Child process %s become zombie. Info from parent process: \n", pid_str);
		system(command);
		wait(NULL);
		sleep(5);
		printf("\nZombie process %s closed. Available processes (see S column for status information): \n", pid_str);		
		system(PS);
	}
	else{
		fprintf(stderr, "Fork error!\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

