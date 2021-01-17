#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct thread_args{
	char* name;
	char* str;
	int num;
};

void* thread_func(void* args){
	struct thread_args* Args = (struct thread_args*)args; 
	for(int i = 1; i <= Args->num; i++){
		printf("%s. %s %d\n", Args->name, Args->str, i);
		//sleep(1);
	}
	return NULL;
}

int main(void){
	setbuf(stdout, NULL);
	pthread_t threads[4];
	

	struct thread_args t1_args = {"Thread 1", "HELLO", 1};	
	if(pthread_create(&(threads[0]), NULL, &thread_func, &t1_args)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}	
	struct thread_args t2_args = {"Thread 2", "HOW ARE YOU?", 7};	
	if(pthread_create(&(threads[1]), NULL, &thread_func, &t2_args)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	struct thread_args t3_args = {"Thread 3", "GOOD BYE..", 5};
	if(pthread_create(&(threads[2]), NULL, &thread_func, &t3_args)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	struct thread_args t4_args = {"Thread 4", "SEE YOU LATE", 10};	
	if(pthread_create(&(threads[3]), NULL, &thread_func, &t4_args)){
		fprintf(stderr, "Error in thread creation.\n");
		return EXIT_FAILURE;
	}
	sleep(1);
	return EXIT_SUCCESS;
}