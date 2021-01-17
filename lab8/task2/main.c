#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "my_rand.h"

typedef struct{
	int from,to;
}th_arg;

int SHARED = 0;
int flag = 0;
pthread_mutex_t mutex_lock;
pthread_cond_t cond_producer;
pthread_cond_t cond_consumer;

void* producer_thread(void* arg){
	th_arg* args = (th_arg*) arg;
	seed();
	int random; 
	while(1){
		random = int_random(args->from, args->to);
		if(pthread_mutex_lock(&mutex_lock)){
			fprintf(stderr, "Error in mutex locking in producer thread\n");
			exit(EXIT_FAILURE);
		}
		while(flag){
			if(pthread_cond_wait(&cond_producer, &mutex_lock)){
				fprintf(stderr, "Error in waiting in consumer thread\n");
				exit(EXIT_FAILURE);
			}
		}
		SHARED = random;
		flag = 1;
		printf("Producer produce item %d\n", random);
		if(pthread_cond_signal(&cond_consumer)){
			fprintf(stderr, "Error in signaling in producer thread\n");
			exit(EXIT_FAILURE);
		}
		if(pthread_mutex_unlock(&mutex_lock)){
			fprintf(stderr, "Error in mutex unlocking in producer thread\n");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
	return NULL;
}
void* consumer_thread(void* arg){
	int value = 0;
	while(1){
		if(pthread_mutex_lock(&mutex_lock)){
			fprintf(stderr, "Error in mutex locking in consumer thread\n");
			exit(EXIT_FAILURE);
		}
		while(!flag){
			if(pthread_cond_wait(&cond_consumer, &mutex_lock)){
				fprintf(stderr, "Error in waiting in consumer thread\n");
				exit(EXIT_FAILURE);
			}
		}
		value = SHARED;
		SHARED = 0;
		flag = 0;
		printf("Consumer consume item %d\n", value);
		if(pthread_cond_signal(&cond_producer)){
			fprintf(stderr, "Error in signaling in producer thread\n");
			exit(EXIT_FAILURE);
		}
		if(pthread_mutex_unlock(&mutex_lock)){
			fprintf(stderr, "Error in mutex unlocking in consumer thread\n");
			exit(EXIT_FAILURE);
		}		
	}
	return NULL;
}


void options(int argc, char** argv, th_arg* args_addr, int* delay_addr){
	int opt;
	while((opt = getopt(argc, argv, "hs:f:t:d:")) != -1){
		switch(opt){
			case 'h':
				printf("Usage: cond [-h] [[-s <items>] | [-f <number>] | [-t <number>] | [-d <seconds>]]\n\n");
				printf("Arguments:\n");
				printf("-h            Show help (this message) and exit\n");
				printf("-f <number>   Set the lower random limit\n");
				printf("-t <number>   Set the higher random limit\n");
				printf("-d <seconds>  Set the main thread waiting delay\n");
				exit(EXIT_SUCCESS);
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
	int delay = 10;
	th_arg arg = {1, 10};
	if(argc > 1)
		options(argc, argv, &arg, &delay);

	if(pthread_mutex_init(&mutex_lock, NULL)){
		fprintf(stderr, "Error in mutex initialization\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_cond_init(&cond_producer, NULL)){
		fprintf(stderr, "Error in conditional variable initialization\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_cond_init(&cond_consumer, NULL)){
		fprintf(stderr, "Error in conditional variable initialization\n");
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
		printf("Item is: %d\n", SHARED);
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
	
	if(pthread_mutex_unlock(&mutex_lock)){
		fprintf(stderr, "Error in mutex unlocking in consumer thread\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_mutex_destroy(&mutex_lock)){
		fprintf(stderr, "Error in mutex destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_cond_destroy(&cond_producer)){
		fprintf(stderr, "Error in conditional variable destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_cond_destroy(&cond_consumer)){
		fprintf(stderr, "Error in conditional variable destroying!!!!!\n");
		exit(EXIT_FAILURE);
	}

	
	return EXIT_SUCCESS;
}