#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

void seed(){
	struct timespec now;
	if(clock_gettime(CLOCK_REALTIME, &now) < 0){
		fprintf(stderr, "Error with time function!\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);	
}

int get_random(int min, int max){
    return rand()%(max - min) + min;
}


typedef struct{
	int from, to, size;
	int* arr;
	pthread_rwlock_t lock;
}th_arg;



void* inf_func(void* argument){
	th_arg* arg = (th_arg*) argument;
	while(1){
		pthread_rwlock_rdlock(&arg->lock);
		printf("Informator gives such array state: ");
		for(int i=0; i < arg->size; i++)
			printf("%d ", (arg->arr)[i]);
		pthread_rwlock_unlock(&arg->lock);
		printf("\n");
		sleep(1);
	}
	return NULL;
}
void* write_func(void* argument){
	th_arg* arg = (th_arg*) argument;
	seed();
	int random = get_random(arg->from, arg->to);
	sleep(random);
	pthread_rwlock_wrlock(&arg->lock);
	arg->arr[get_random(0, arg->size - 1)] = random;
	printf("\tWriter put random value %d to array\n", random);
	pthread_rwlock_unlock(&arg->lock);
	return NULL;
}
void* read_func(void* argument){
	th_arg* arg = (th_arg*) argument;
	seed();
	int random = get_random(0, arg->size - 1);
	sleep(random);
	pthread_rwlock_rdlock(&arg->lock);
	random = arg->arr[random];
	printf("\t\tReader get random value %d from array\n", random);
	pthread_rwlock_unlock(&arg->lock);
	sleep(1);
	return NULL;
}


void options(int argc, char** argv, int* write_count_addr, int* read_count_addr, int* from_addr, int* to_addr, int* size_addr);

int main(int argc, char** argv){
	int FROM = 0, TO = 9, SIZE = 10, write_count = 2, read_count = write_count*2;
	if(argc > 1)
		options(argc, argv, &write_count, &read_count, &FROM, &TO, &SIZE);
	int* ARR = (int*) malloc(sizeof(int) * SIZE);
	for(int i=0; i<SIZE; i++)
		ARR[i] = 0;

	th_arg* arg = (th_arg*) malloc(sizeof(th_arg));
	if(!arg){
		fprintf(stderr, "Memory error during thread args dinamic allocation\n");
		exit(EXIT_FAILURE);
	}
	arg->from = FROM;
	arg->to = TO;
	arg->size = SIZE;
	arg->arr = ARR;
	if(pthread_rwlock_init(&(arg->lock), NULL)){
		fprintf(stderr, "Error in rwlock initialization\n");
		exit(EXIT_FAILURE);
	}
	
	pthread_attr_t attr;
	pthread_t informator;
	if(pthread_attr_init(&attr)){
		fprintf(stderr, "Error in detached thread attr initialization.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)){
		fprintf(stderr, "Error in detached thread attr setting.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&informator, &attr, &inf_func, arg)){
		fprintf(stderr, "Error in detached thread creation.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_attr_destroy(&attr)){
		fprintf(stderr, "Error in detached thread attr destroying.\n");
		exit(EXIT_FAILURE);
	}

	
	pthread_t* writers = (pthread_t*) malloc(write_count * sizeof(pthread_t));
	pthread_t* readers = (pthread_t*) malloc(read_count * sizeof(pthread_t));
	for(int i = 0; i < write_count; i++){
		if(pthread_create(writers + i, NULL, &write_func, arg)){
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < read_count; i++){
		if(pthread_create(readers + i, NULL, &read_func, arg)){
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < write_count; i++){
		if(pthread_join(writers[i], NULL)){
			fprintf(stderr, "Error in thread join\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < read_count; i++){
		if(pthread_join(readers[i], NULL)){
			fprintf(stderr, "Error in thread join\n");
			exit(EXIT_FAILURE);
		}
	}
	if(pthread_cancel(informator)){
		fprintf(stderr, "Error in thread cancelling.\n");
		return EXIT_FAILURE;
	}
	

	pthread_rwlock_destroy(&(arg->lock));
	free(arg);

	free(writers);
	free(readers);
	free(ARR);
	return EXIT_SUCCESS;
}

void options(int argc, char** argv, int* write_count_addr, int* read_count_addr, int* from_addr, int* to_addr, int* size_addr){
	int opt;
	while((opt = getopt(argc, argv, "hw:r:f:t:s:")) != -1){
		switch(opt){
			case 'h':
				printf("Usage: ./rwlock [-h] | [[-w <number>] | [-r <number>] | [-f <count>] | [-t <number>] | [-s <number>]]\n\n");
				printf("Arguments:\n");
				printf("-h           Show help (this message) and exit\n");
				printf("-w <number>  Set writers count\n");
				printf("-r <number>  Set readers count\n");
				printf("-f <number>  Set the lower random limit (FROM)\n");
				printf("-t <number>  Set the higher random limit (TO)\n");
				printf("-s <number>  Set array size\n");
				exit(EXIT_SUCCESS);
			case 'w':
				*write_count_addr = atoi(optarg);
				break;
			case 'r':
				*read_count_addr = atoi(optarg);
				break;	
			case 'f':
				*from_addr = atoi(optarg);
				break;
			case 't':
				*to_addr = atoi(optarg);
				break;
			case 's':
				*size_addr = atoi(optarg);
				break;		
		}
	}
}