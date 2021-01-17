#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void help(){
	printf("Information about process\n\n");
	printf("Usage: info [-h | -{mode}]\n\n");
	printf("Arguments:\n");
	printf("-h  Show help (this massage) and exit\n\n");
	printf("Mode:\n");
	printf("-0 or -d  Equal to default mode: PID, PPID and PGID\n");
	printf("-1 or -u  User mode: default mode + UID and GID\n");
	printf("-2 or -e  Effective mode: user mode + eUID and eGID\n");
}

void default_mode(){
	printf("Process info:\n");
	printf("PID:    %d\n", getpid());
	printf("PPID:   %d\n", getppid());
	printf("PGID:   %d\n", getpgrp());
}

void user_mode(){
	default_mode();
	printf("UID:    %d\n", getuid());
	printf("GID:    %d\n", getgid());
}

void effective_mode(){
	user_mode();
	printf("eUID:   %d\n", geteuid());
	printf("eGID:   %d\n", getegid());
}

int main(int argc, char** argv){
	if(argc <= 1){
		default_mode();
		return EXIT_SUCCESS;
	}
	int opt;		
	while((opt = getopt(argc, argv, "h012due")) != -1){
		switch(opt){
			case 'h': 
				help();				
				break;
			case '0':
			case 'd':
				default_mode();
				break;
			case '1':
			case 'u':
				user_mode();
				break;
			case '2':
			case 'e':
				effective_mode();
				break;
		}
		return EXIT_SUCCESS;
	}
}