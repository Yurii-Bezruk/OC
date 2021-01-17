#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/shm.h>

int continue_flag = 0;

void handler(int signo){
	if(signo == SIGRTMIN){
		continue_flag = 1;
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

void wait_for_signal(){	   //ожидание нужного сигнала
	while(!continue_flag)
		pause();
	continue_flag = 0;
}

int main(int argc, char** argv){
	//регистрация обработчика ответных сигналов
	if(signal(SIGRTMIN, handler) == SIG_ERR){
		printf("Ошибка регистрации обработчика сигнала");
		exit(EXIT_FAILURE);
	}

	//установка количества записываемых чисел
	int COUNT = 10;			
	if(argc > 1)
		COUNT = atoi(argv[1]);	
	COUNT++; //на одно больше - для передачи количества чисел в 0 элементе массива
	int SHMEM_SIZE = COUNT * sizeof(int); //перевод в байты

	//создание сегмента общей памяти с правами на чтение и запись
	int shm_id;
	shm_id = shmget(IPC_PRIVATE, SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0600);
	if(shm_id == -1){
		fprintf(stderr, "shmget() error\n");
		return EXIT_FAILURE;
	}

	//присоединение сегмента общем памяти к адресному пространству процесса
	int* shm_buf = (int*) shmat(shm_id, NULL, 0);
	if(shm_buf == (int*) -1){
		fprintf(stderr, "shmat() error\n");
		return EXIT_FAILURE;
	}

	//проверка на соответствие размера
	struct shmid_ds ds;
	shmctl(shm_id, IPC_STAT, &ds);	
	if(ds.shm_segsz < SHMEM_SIZE){
		fprintf(stderr, "error: segsize=%ld\n", ds.shm_segsz);
		return EXIT_FAILURE;
	}

	//создание процесса-потомка и загрузка бинарного образа
	int pid = fork();
	if(pid < 0){
		fprintf(stderr, "fork() error\n");
		return EXIT_FAILURE;
	}
	else if(pid == 0){
		execl("./second", "./second", NULL);
		fprintf(stderr, "exec() error\n");
		return EXIT_FAILURE;
	}
	//родительский процесс

	//ожидаем сигнала готовности к передаче от потомка
	wait_for_signal();

	//отправка сигнала с идентификатором общей памяти
	union sigval number;
	number.sival_int = shm_id;
	sigqueue(pid, SIGRTMIN, number);

	while(1){
		//ввод количества вводимых данных
		printf("Enter number count (< 0 for exit): ");
		int count;
		scanf("%d", &count);
		if(count < 0)	//значение меньшее 0 - завершение сеанса
			break;
		else if(count >= COUNT){	//не больше чем выделено памяти
			count = COUNT-1;
			fprintf(stderr, "Wrong number count, %d established.\n", count);
		}
		count++;
		shm_buf[0] = count;

		//ввод данных
		printf("Enter numbers: ");
		for(int i = 1; i < count; i++){
			scanf("%d", shm_buf+i);
		}

		//отправляем сигнал о том что потомок может начать работу
		kill(pid, SIGRTMIN+1);

		//ожидание результата
		wait_for_signal();
		printf("Result is: %d\n", shm_buf[0]);
	}

	shmdt(shm_buf);	//отсоединяем память
	kill(pid, SIGTERM); 	//посылаем сигнал о завершении дочернему процессу
	waitpid(pid, NULL, 0); 		//ожидаем завершения
	shmctl(shm_id, IPC_RMID, NULL);	//удаляем общий сегмент памяти
	return EXIT_SUCCESS;
}