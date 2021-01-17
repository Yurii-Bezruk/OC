#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

typedef struct{
	int count;
	long int* VALUE;
}th_arg;

void* thread_add(void* arg){
	th_arg* args = (th_arg*) arg;
	for(int i = 0; i < args->count; i++)
		(*(args->VALUE))++;
	return NULL;
}
void* thread_neg(void* arg){
	th_arg* args = (th_arg*) arg;
	for(int i = 0; i < args->count; i++)
		(*(args->VALUE))--;
	return NULL;
}

void options(int argc, char** argv, int* t_count_addr, int* o_count_addr){
	int opt;
	while((opt = getopt(argc, argv, "ht:o:")) != -1){
		switch(opt){
			case 'h':
				printf("Usage: race [[-h] | [-t <count>] [-o <count>]]\n\n");
				printf("Arguments:\n");
				printf("-h          Show help (this message) and exit\n");
				printf("-t <count>  Set the specific thread pair count\n");
				printf("-o <count>  Set the specific thread operations count\n");
				exit(EXIT_SUCCESS);
			case 't':
				*t_count_addr = atoi(optarg);
				break;
			case 'o':
				*o_count_addr = atoi(optarg);
				break;
		}
	}

}

int main(int argc, char** argv){
	long int VALUE = 0;
	int t_count = 1, o_count = 10000;
	if(argc > 1)
		options(argc, argv, &t_count, &o_count);
	th_arg arg = {o_count, &VALUE};
	
	pthread_t* threads_add = (pthread_t*) malloc(t_count * sizeof(pthread_t));
	pthread_t* threads_neg = (pthread_t*) malloc(t_count * sizeof(pthread_t));
	for(int i = 0; i < t_count; i++){
		if(pthread_create(threads_add + i, NULL, &thread_add, &arg)){
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
		if(pthread_create(threads_neg + i, NULL, &thread_neg, &arg)){
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
		if(pthread_join(threads_add[i], NULL)){
			fprintf(stderr, "Error in thread join\n");
			exit(EXIT_FAILURE);
		}
		if(pthread_join(threads_neg[i], NULL)){
			fprintf(stderr, "Error in thread join\n");
			exit(EXIT_FAILURE);
		}
		printf("Pair #%d: %ld\n", i+1, VALUE);
		VALUE = 0;
	}
	
	free(threads_add);
	free(threads_neg);
	return EXIT_SUCCESS;
}