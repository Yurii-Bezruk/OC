#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
	int index;
	int* size_addr;
}thread_arg;

int* ARR;



void* thread_func(void* arg){
	thread_arg* args = (thread_arg*) arg;
	int n = args->index;
	printf("Thread %d created...\n", n);
	printf("Thread %d sleep for %d seconds...\n", n, n);
	sleep(n);
	seed();
	int random = get_random(1, 10);	
	printf("Thread %d generate random number %d\n", n, random);
	ARR[n] = random;
	*(args->size_addr) += 1;
	printf("Thread %d sleep for %d seconds...\n", n, random);
	sleep(random);
	return NULL;
}

void* detached_func(void* arg){
	printf("Detached thread created...\n");
	thread_arg* args = (thread_arg*) arg;
	while(*(args->size_addr) < args->index){
		if(*(args->size_addr) > 0){
			printf("Detached thread info: current array state: ");
			for(int i = 0; i < *(args->size_addr); i++)
				printf("%d ", ARR[i]);
			printf("\n");
		}
		sleep(1);
	}
	printf("Detached thread info: current array state: ");
	for(int i = 0; i < *(args->size_addr); i++)
		printf("%d ", ARR[i]);
	printf("\nDetached thread finished.\n");
	return NULL;
}

void create_threads(pthread_t threads[], int count){
	int size = 0;
	thread_arg detached_arg = {count, &size};
	
	pthread_attr_t attr;
	pthread_t detached_thread;
	if(pthread_attr_init(&attr)){
		fprintf(stderr, "Error in detached thread attr initialization.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)){
		fprintf(stderr, "Error in detached thread attr setting.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&detached_thread, &attr, &detached_func, &detached_arg)){
		fprintf(stderr, "Error in detached thread creation.\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_attr_destroy(&attr)){
		fprintf(stderr, "Error in detached thread attr destroying.\n");
		exit(EXIT_FAILURE);
	}
	
	thread_arg args[count];
	for(int i = 0; i < count; i++){
		args[i].index = i;
		args[i].size_addr = &size;
		if(pthread_create(&(threads[i]), NULL, &thread_func, args+i)){
			fprintf(stderr, "Error in thread %d creation.\n", i);
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < count; i++){
		if(pthread_join(threads[i], NULL)){
			fprintf(stderr, "Error in thread %d join.\n", i);
			exit(EXIT_FAILURE);
		}
		else{
			printf("Thread %d finished.\n", i);
		}
	}
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	int count = 1;
	if(argc > 1)
		count = atoi(argv[1]);

	ARR = (int*) malloc(sizeof(int) * count);
	if(!ARR){
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}

	pthread_t threads[count];
	create_threads(threads, count);	
	free(ARR);
	return EXIT_SUCCESS;
}