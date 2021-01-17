#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>

int shm_id;
int* shm_buf;
int continue_flag = 0;			

void signal_action(int signo, siginfo_t* si, void* ucontext){
	if(signo == SIGRTMIN){	//сигнал передачи идентификатора
		continue_flag = 1;
		shm_id = si->si_value.sival_int;
	}
	else if(signo == SIGRTMIN+1){	//сигнал начала вычислений
		continue_flag = 1;
	}
	else{
		printf("Захвачен неожиданный сигнал в потомке!\n");
		exit(EXIT_FAILURE);
	}
}

void wait_for_signal(){   //ожидание нужного сигнала
	while(!continue_flag)
		pause();
	continue_flag = 0;
}

void exit_handler(void){
	shmdt(shm_buf);
}

int main(int argc, char** argv){
	atexit(exit_handler);

	//получаем id родителя
	int pid = getppid();
	
	//маска для обработчика, для защиты
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

	//регистрируем обработчики
	struct sigaction act;
	act.sa_sigaction = signal_action;
	act.sa_mask = mask;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGRTMIN, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала");
		exit(EXIT_FAILURE);
	}
	if(sigaction(SIGRTMIN+1, &act, NULL)){
		printf("Ошибка регистрации обработчика сигнала");
		exit(EXIT_FAILURE);
	}

	//посылаем сигнал о готовности получить сигнал с ключом и получаем его
	kill(pid, SIGRTMIN);
	wait_for_signal();

	//получаем доступ к общей памяти
	shm_buf = (int*) shmat(shm_id, NULL, 0);
	if(shm_buf == (int*) -1){
		fprintf(stderr, "subling shmat() error\n");
		return EXIT_FAILURE;
	}

	while(1){
		//ждем сигнала начала вычислений
		wait_for_signal();

		//вычисляем сумму
		int sum = 0;
		for(int i = 1; i < shm_buf[0]; i++)
			sum += shm_buf[i];
		shm_buf[0] = sum;
		
		//отправляем сигнал о том, что вычисления окончены
		kill(pid, SIGRTMIN);
	}
	return EXIT_SUCCESS;
}