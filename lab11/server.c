#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SEM1 0
#define SEM2 1


int shm_id;
int* shm_buf;
int sem_id;
struct sembuf sems[1]; //структура операций с семафорами

void sem_block(int semaphore){
	sems[0].sem_num = semaphore;	
	sems[0].sem_op = -1;
	if(semop(sem_id, sems, 1) == -1){
		fprintf(stderr, "semop() error\n");
		exit(EXIT_FAILURE);
	}
}
void sem_unblock(int semaphore){
	sems[0].sem_num = semaphore;	
	sems[0].sem_op = 1;
	if(semop(sem_id, sems, 1) == -1){
		fprintf(stderr, "semop() error\n");
		exit(EXIT_FAILURE);
	}
}

void signal_handler(int signo){
	if(signo == SIGINT){
		exit(EXIT_SUCCESS);
	}
	else{
		printf("Захвачен неожиданный сигнал!\n");
		exit(EXIT_FAILURE);
	}
}

void exit_handler(void){
	shmdt(shm_buf);	//отсоединяем память
	shmctl(shm_id, IPC_RMID, NULL);	//удаляем общий сегмент памяти	
	semctl(sem_id, 0, IPC_RMID, NULL);	//удаляем массив семафоров
}

int main(int argc, char** argv){
	atexit(exit_handler);
	signal(SIGINT, signal_handler);

	int COUNT = 10;			
	if(argc > 1)
		COUNT = atoi(argv[1]);	
	COUNT++; //на одно больше - для передачи количества чисел в 0 элементе массива
	int SHMEM_SIZE = COUNT * sizeof(int); //перевод в байты

	//создание ключа
	key_t key = ftok("shmem.txt", 5);
	//создание сегмента общей памяти с правами на чтение и запись
	
	shm_id = shmget(key, SHMEM_SIZE, IPC_CREAT | IPC_EXCL | 0600);
	if(shm_id == -1){
		fprintf(stderr, "shmget() error\n");
		return EXIT_FAILURE;
	}

	key = ftok("sem.txt", 6);
	//создание массива семафоров с правами на чтение и запись
	
	sem_id = semget(key, 2, IPC_CREAT | IPC_EXCL | 0600);
	if(sem_id == -1){
		fprintf(stderr, "semget() error\n");
		return EXIT_FAILURE;
	}

	//присоединение сегмента общем памяти к адресному пространству процесса
	shm_buf = (int*) shmat(shm_id, NULL, 0);
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
	
	//инициализация операций с семафорами
	sems[0].sem_flg = 0;	

	while(1){
		sem_block(SEM1);
		//вычисляем сумму
		int sum = 0;
		for(int i = 1; i < shm_buf[0]; i++)
			sum += shm_buf[i];
		shm_buf[0] = sum;
		sem_unblock(SEM2);
	}
	
	return EXIT_SUCCESS;
}