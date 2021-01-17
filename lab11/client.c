#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM1 0
#define SEM2 1

int sem_id;
struct sembuf sems[1]; //структура операций с семафорами

void sem_block(int semaphore){
	sems[0].sem_num = semaphore;	
	sems[0].sem_op = -1;
	if(semop(sem_id, sems, 1) == -1){
		fprintf(stderr, "semop() -1 error\n");
		exit(EXIT_FAILURE);
	}
}
void sem_unblock(int semaphore){
	sems[0].sem_num = semaphore;	
	sems[0].sem_op = 1;
	if(semop(sem_id, sems, 1) == -1){
		fprintf(stderr, "semop() 1 error\n");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char** argv){
	int COUNT = 10;			
	if(argc > 1)
		COUNT = atoi(argv[1]);	
	COUNT++; //на одно больше - для передачи количества чисел в 0 элементе массива
	int SHMEM_SIZE = COUNT * sizeof(int); //перевод в байты

	//создание ключа
	key_t key = ftok("shmem.txt", 5);
	//поиск сегмента общей памяти с правами на чтение и запись
	int shm_id;
	shm_id = shmget(key, SHMEM_SIZE, IPC_CREAT | 0600);
	if(shm_id == -1){
		fprintf(stderr, "shmget() error\n");
		return EXIT_FAILURE;
	}

	key = ftok("sem.txt", 6);
	//создание массива семафоров с правами на чтение и запись
	int sem_id;
	sem_id = semget(key, 2, IPC_CREAT | 0600);
	if(sem_id == -1){
		fprintf(stderr, "semget() error\n");
		return EXIT_FAILURE;
	}

	//получаем доступ к общей памяти
	int* shm_buf = (int*) shmat(shm_id, NULL, 0);
	if(shm_buf == (int*) -1){
		fprintf(stderr, "subling shmat() error\n");
		return EXIT_FAILURE;
	}

	sems[0].sem_flg = 0;	
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

		sem_unblock(SEM1);
		// printf("sem1 unblocked\n");
		sem_block(SEM2);
		printf("Result is: %d\n", shm_buf[0]);

	}
	shmdt(shm_buf);	//отсоединяем память
	return EXIT_SUCCESS;
}