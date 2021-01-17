#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>



void signal_action(int signo, siginfo_t* si, void* ucontext){
	if(signo == SIGUSR1){
		printf("Процессом %d Захвачен сигнал %s от процесса %d. Номер вызова: %d\n", getpid(),  sys_siglist[signo], si->si_pid, si->si_value.sival_int);
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv){
	int count = 5;
	if(argc > 1)
		count = atoi(argv[1]);

	struct sigaction act;
	act.sa_sigaction = signal_action;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGUSR1, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала SIGUSR1\n");
		exit(EXIT_FAILURE);
	}
	int pid = fork();
	if(pid < 0){
		printf("Ошибка создания дочернего процесса\n");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0){
		while(1)
			pause();
	}
	else{
		union sigval number;
		for(int i = 0; i < count; i++){
			number.sival_int = i+1;
			sigqueue(pid, SIGUSR1, number);
			usleep(100000);
		}
	}
	kill(pid, SIGTERM);
	waitpid(pid, NULL, 0);
	exit(EXIT_SUCCESS);
}