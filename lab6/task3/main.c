#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


void* thread_func(void* args){
	int count = *(int*)args * 2;
	if(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)){
		fprintf(stderr, "Error in thread cancel state setting.\n");
		exit(EXIT_FAILURE);
	}
	for(long i=1; i<=count; i++){
		printf("Uncanselable thread. Iteration: %ld\n", i);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	int delay = 3;
	if(argc > 1)
		delay = atoi(argv[1]);
	pthread_t thread;
	void* result;

	if(pthread_create(&thread, NULL, &thread_func, &delay)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	sleep(delay);
	if(pthread_cancel(thread)){
		fprintf(stderr, "Error in thread cancelling.\n");
		return EXIT_FAILURE;
	}		
	if(pthread_join(thread, &result)){
		fprintf(stderr, "Error in thread join.\n");
		return EXIT_FAILURE;
	}	
	if(result == PTHREAD_CANCELED)
		printf("Thread has exited by cancelling\n");
	else
		printf("Thread has exited without canselling\n");
	return EXIT_SUCCESS;
}