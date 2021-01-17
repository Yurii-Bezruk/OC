#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

char* message;

void alarm_handler(int signo){
	if(signo == SIGALRM){
		printf("%s\n", message);
		exit(EXIT_SUCCESS);
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

void exit_handler(void){
	free(message);
}

int main(int argc, char** argv){
	atexit(exit_handler);
	int delay = 10;
	
	if(argc > 1)
		delay = atoi(argv[1]);
	if(argc > 2){
		message = (char*) malloc(sizeof(char) * (strlen(argv[2]) + 1));
		strcpy(message, argv[2]);
	}
	else{
		message = (char*) malloc(sizeof(char) * (strlen("HELLO, WORLD!") + 1));
		strcpy(message, "HELLO, WORLD!");
	}

	if(signal(SIGALRM, alarm_handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала SIGALRM\n");
		exit(EXIT_FAILURE);
	}

	int pid = fork();
	if(pid == 0){
		alarm(delay);
		while(1)
			pause();
	}
	else if(pid < 0){
		printf("Ошибка создания дочернего процесса\n");
		exit(EXIT_FAILURE);
	}	
	return EXIT_SUCCESS;
}