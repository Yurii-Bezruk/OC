#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


void* thread_func(void* args){
	if(pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL)){
		fprintf(stderr, "Error in thread cancel type setting.\n");
		exit(EXIT_FAILURE);
	}
	long n = 1000000;
	double* pi = (double*) malloc(sizeof(double));
	*pi = 0.0;

	double znak = 1.0;
	double shag = 1.0;
	for(int i=0; i < n; i++){
		(*pi) += znak * (1/shag);		
		printf("%.20f\n", (*pi)*4);
		znak *= -1;
		shag += 2;
		//sleep(0.1);
	}
	(*pi) *= 4; 
	return pi;
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	int delay = 3;
	if(argc > 1)
		delay = atoi(argv[1]);
	pthread_t thread;
	void* result;

	if(pthread_create(&thread, NULL, &thread_func, NULL)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	sleep(delay);
	if(pthread_cancel(thread)){
		fprintf(stderr, "Error in thread cancelling.\n");
		//return EXIT_FAILURE;
	}		
	if(pthread_join(thread, &result)){
		fprintf(stderr, "Error in thread join.\n");
		return EXIT_FAILURE;
	}	
	if(result == PTHREAD_CANCELED)
		printf("\nThread has exited by cancelling\n");
	else{
		printf("\nThe PI is %.10f\n", *(double*)result);
		free(result);
	}
	return EXIT_SUCCESS;
}