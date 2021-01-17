#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#define MAX_PRS_COUNT 100
extern int errno;

int digits(int value){
	int counter = 1;
	while(value/=10)
		counter++;
	return counter;
}

int main(int argc, char** argv){
	//число процессов
	int pr_count = 1;
	if(argc > 1)
		pr_count = atoi(argv[1]);
	if(pr_count <= 0 || pr_count > MAX_PRS_COUNT){
		fprintf(stderr, "Non-valid number of processes!\n");
		return EXIT_FAILURE;
	}	

	//создание
	int pids[pr_count];
	for(int i = 0; i < pr_count; i++){
		pids[i] = fork();
		if(pids[i] < 0){	//ошибка
			fprintf(stderr, "Error in process creating: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		else if(pids[i] == 0){	//дочерний
			printf("Process %d is created!\n", getpid());
			//зерно в наносекундах
			struct timespec now;
			if(clock_gettime(CLOCK_REALTIME, &now) < 0){
				fprintf(stderr, "Error with time function!\n");
				return EXIT_FAILURE;
			}
			srand(now.tv_nsec);	
			//выбор сосотояния		
			unsigned short state = rand()%2;
			if(state)
				exit(1);			
			else
				while(1){}
		}
	}

	char pid[digits(MAX_PRS_COUNT)];
	sprintf(pid, "%i", pr_count-1); 
	char* command = (char*) malloc(sizeof("kill ")+sizeof(pid)); 
	strcpy(command, "ps ");
	//вызов ps для каждого дочернего процесса
	for(int i = 0; i < pr_count; i++){
		sprintf(pid, "%i", pids[i]);   		
		strcat(command, pid);
		system(command);		
		for(int i = 3; i < strlen(command); i++)	
			command[i] = '\0';
	}
	//вызов kill для каждого дочернего процесса
	strcpy(command, "kill \0");
	for(int i = 0; i < pr_count; i++){
		sprintf(pid, "%i", pids[i]);   		
		strcat(command, pid);
		system(command);		
		for(int i = 5; i < strlen(command); i++)	
			command[i] = '\0';
	} 
	free(command);
	return EXIT_SUCCESS;
}