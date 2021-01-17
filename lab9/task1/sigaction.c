#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signal.h>

void signal_action(int signo, siginfo_t* si, void* ucontext){
	if(signo == SIGINT || signo == SIGTERM)
		printf("Захвачен сигнал %s от процесса %d\n", sys_siglist[si->si_signo], si->si_pid);
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
	sigset_t mask; 
	if(sigemptyset(&mask)){
		printf("Ошибка инициализации набора сигналов");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGINT)){
		printf("Ошибка добавления сигнала в набор");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGTERM)){
		printf("Ошибка добавления сигнала в набор");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGUSR1)){
		printf("Ошибка добавления сигнала в набор");
		exit(EXIT_FAILURE);
	}
	if(sigaddset(&mask, SIGPROF)){
		printf("Ошибка добавления сигнала в набор");
		exit(EXIT_FAILURE);
	}

	struct sigaction act;
	act.sa_sigaction = signal_action;
	act.sa_mask = mask;
	act.sa_flags = SA_SIGINFO;	

	if(sigaction(SIGINT, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала SIGINT");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGTERM, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала SIGTERM");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGUSR1, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала SIGUSR1");
		exit(EXIT_FAILURE);
	}
	act.sa_sigaction = NULL;
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	if(sigaction(SIGPROF, &act, NULL)){
		printf("Ошибка восстановлния режима поведения сигнала SIGPROF");
		exit(EXIT_FAILURE);
	}
	act.sa_handler = SIG_IGN;
	if(sigaction(SIGHUP, &act, NULL)){
		printf("Ошибка регистрации игнорирования сигнала SIGHUP");
		exit(EXIT_FAILURE);
	}
	while(1)
		pause();
	return EXIT_SUCCESS;
}