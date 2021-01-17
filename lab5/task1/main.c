#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* child_thread(void* args){
	for(int i=1; i<=10; i++){
		printf("\tChild Thread. Iteration: %d\n", i);
		sleep(1);
	}
	return NULL;
}

int main(void){
	setbuf(stdout, NULL);
	pthread_t child;
	if(pthread_create(&child, NULL, &child_thread, NULL)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}

	for(int i=1; i<=10; i++){
		printf("Main Thread. Iteration: %d\n", i);
		sleep(2);
	}
	return EXIT_SUCCESS;
}