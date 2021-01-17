#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

char* message = "parent process message";
sigset_t wait;
int pid;

void signal_handler(int signo){
	if(signo == SIGUSR1){
		if(sigprocmask(SIG_BLOCK, &wait, NULL)){
			printf("Ошибка установки сигнальной маски процессу");
			exit(EXIT_FAILURE);
		}
		printf("%s\n", message);
		if(sigprocmask(SIG_UNBLOCK, &wait, NULL)){
			printf("Ошибка установки сигнальной маски процессу");
			exit(EXIT_FAILURE);
		}
		//sleep(0);
		if(kill(pid, SIGUSR1)){
			printf("Ошибка отправки сигнала\n");
		}
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv){
	if(signal(SIGUSR1, signal_handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала SIGUSR1\n");
		exit(EXIT_FAILURE);
	}

	if(sigemptyset(&wait)){
		printf("Ошибка инициализации набора сигналов");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&wait, SIGUSR1)){
		printf("Ошибка добавления сигнала в набор");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if(pid < 0){
		printf("Ошибка создания дочернего процесса\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		pid = getppid();
		message = "child process message";
		kill(pid, SIGUSR1);	
	}
	while(1){		
		pause();
	}
	exit(EXIT_SUCCESS);
}