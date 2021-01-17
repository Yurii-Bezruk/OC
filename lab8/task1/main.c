#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <semaphore.h>
#include "my_rand.h"

typedef struct{
	int from,to;
}th_arg;

int* BUFFER;
sem_t lock;
sem_t empty_items;
sem_t full_items;

void* producer_thread(void* arg){
	th_arg* args = (th_arg*) arg;
	seed();
	int random; 
	int position = 0;
	while(1){
		random = int_random(args->from, args->to);
		if(sem_wait(&empty_items)){
			fprintf(stderr, "Error in empty_items semaphore waiting in produser thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_wait(&lock)){
			fprintf(stderr, "Error in lock semaphore waiting in produser thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_getvalue(&full_items, &position)){
			fprintf(stderr, "Error in semaphore value getting in produser thread\n");
			exit(EXIT_FAILURE);
		}
		BUFFER[position] = random;
		printf("Produser produse item %d\n", random);
		if(sem_post(&lock)){
			fprintf(stderr, "Error in lock semaphore posting in produser thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&full_items)){
			fprintf(stderr, "Error in full_items semaphore posting in produser thread\n");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	return NULL;
}
void* consumer_thread(void* arg){
	int value = 0;
	int position = 0;
	while(1){
		if(sem_wait(&full_items)){
			fprintf(stderr, "Error in full_items semaphore waiting in consumer thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_wait(&lock)){
			fprintf(stderr, "Error in lock semaphore waiting in consumer thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_getvalue(&full_items, &position)){
			fprintf(stderr, "Error in semaphore value getting\n");
			exit(EXIT_FAILURE);
		}
		value = BUFFER[position];
		BUFFER[position] = 0;
		printf("Consumer consume item %d\n", value);
		if(sem_post(&lock)){
			fprintf(stderr, "Error in lock semaphore posting in consumer thread\n");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&empty_items)){
			fprintf(stderr, "Error in empty_items semaphore posting in consumer thread\n");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	return NULL;
}


void options(int argc, char** argv, int* size_addr, th_arg* args_addr, int* delay_addr){
	int opt;
	while((opt = getopt(argc, argv, "hs:f:t:d:")) != -1){
		switch(opt){
			case 'h':
				printf("Usage: sem [-h] [[-s <items>] | [-f <number>] | [-t <number>] | [-d <seconds>]]\n\n");
				printf("Arguments:\n");
				printf("-h            Show help (this message) and exit\n");
				printf("-s <items>    Set the specific buffer size\n");
				printf("-f <number>   Set the lower random limit\n");
				printf("-t <number>   Set the higher random limit\n");
				printf("-d <seconds>  Set the main thread waiting delay\n");
				exit(EXIT_SUCCESS);
			case 's':
				*size_addr = atoi(optarg);
				break;
			case 'f':
				args_addr->from = atoi(optarg);
				break;
			case 't':
				args_addr->to = atoi(optarg);
				break;
			case 'd':
				*delay_addr = atoi(optarg);
				break;
		}
	}
}

int main(int argc, char** argv){
	int size = 10, delay = 10;
	th_arg arg = {1, 10};
	if(argc > 1)
		options(argc, argv, &size, &arg, &delay);
	BUFFER = (int*) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++)
		BUFFER[i] = 0;

	if(sem_init(&lock, 0, 1)){
		fprintf(stderr, "Error in lock semaphore initialization\n");
		exit(EXIT_FAILURE);
	}
	if(sem_init(&empty_items, 0, size)){
		fprintf(stderr, "Error in empty_items semaphore initialization\n");
		exit(EXIT_FAILURE);
	}
	if(sem_init(&full_items, 0, 0)){
		fprintf(stderr, "Error in full_items semaphore initialization\n");
		exit(EXIT_FAILURE);
	}
	pthread_t producer, consumer;
	if(pthread_create(&producer, NULL, &producer_thread, &arg)){
		fprintf(stderr, "Error in producer thread creation\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&consumer, NULL, &consumer_thread, NULL)){
		fprintf(stderr, "Error in consumer thread creation\n");
		exit(EXIT_FAILURE);
	}
	for(int j = 1; j <= delay; j++){
		for(int i = 0; i < size; i++)
			printf("%d ", BUFFER[i]);
		printf("\n");
		sleep(1);
	}
	if(pthread_cancel(producer)){
		fprintf(stderr, "Error in producer thread cancel\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_cancel(consumer)){
		fprintf(stderr, "Error in consumer thread cancel\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(producer, NULL)){
		fprintf(stderr, "Error in producer thread join\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_join(consumer, NULL)){
		fprintf(stderr, "Error in consumer thread join\n");
		exit(EXIT_FAILURE);
	}	
	
	if(sem_destroy(&lock)){
		fprintf(stderr, "Error in lock semaphore destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}
	if(sem_destroy(&empty_items)){
		fprintf(stderr, "Error in empty_items semaphore destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}
	if(sem_destroy(&full_items)){
		fprintf(stderr, "Error in full_items semaphore destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < size; i++)
		printf("%d ", BUFFER[i]);
	printf("\n");	

	free(BUFFER);
	return EXIT_SUCCESS;
}