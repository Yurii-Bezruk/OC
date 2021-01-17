#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void on_thread_exit(void* arg){
	long iter = *(long*)arg;
	printf(">>>Thread exited on %ld%s iteration.\n", iter, iter%10 > 3 || iter == 11 || iter == 12 || iter == 13 ? "th" : (iter%10 == 1 ? "st" : (iter%10 == 2 ? "nd" : "rd")));
}

void* thread_func(void* args){
	long i;
	pthread_cleanup_push(on_thread_exit, &i);
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)){
		fprintf(stderr, "Error in thread cancel type setting.\n");
		exit(EXIT_FAILURE);
	}
	for(i=1; 1; i++){
		printf("Asynchronous thread. Iteration: %ld\n", i);
		sleep(1);
	}
	pthread_cleanup_pop(1);
	return NULL;
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	int delay = 3;
	if(argc > 1)
		delay = atoi(argv[1]);
	pthread_t thread;
	void* result_addr;

	if(pthread_create(&thread, NULL, &thread_func, NULL)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	sleep(delay);
	if(pthread_cancel(thread)){
		fprintf(stderr, "Error in thread cancelling.\n");
		return EXIT_FAILURE;
	}		
	if(pthread_join(thread, &result_addr)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}	
	if(result_addr == PTHREAD_CANCELED)
		printf("Thread has exited by cancelling\n");
	else
		printf("Thread has exited without canselling\n");
	return EXIT_SUCCESS;
}