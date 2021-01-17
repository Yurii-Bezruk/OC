#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


void signal_handler(int signo){
	if(signo == SIGINT || signo == SIGTERM)
		printf("Захвачен сигнал %s\n", sys_siglist[signo]);
	else if(signo == SIGUSR1){
		printf("Завершение работы программы...\n");
		exit(EXIT_SUCCESS);
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv){
	if(signal(SIGINT, signal_handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала SIGINT");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGTERM, signal_handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала SIGTERM");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGUSR1, signal_handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала SIGUSR1");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGPROF, SIG_DFL) == SIG_ERR){
		printf("Ошибка восстановлния режима поведения сигнала SIGPROF");
		exit(EXIT_FAILURE);
	}
	if(signal(SIGHUP, SIG_IGN) == SIG_ERR){
		printf("Ошибка регистрации игнорирования сигнала SIGHUP");
		exit(EXIT_FAILURE);
	}
	while(1)
		pause();
	return EXIT_SUCCESS;
}