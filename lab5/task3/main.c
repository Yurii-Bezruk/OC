#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

struct thread_args{
	int from, to, count, target, exit_status;
};

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

void* thread_1(void* arg){
	printf("Thread 1 started...\n");
	for(int i = 1; i <= *(int*)arg; i++){
		printf("Thread 1 Iteration %d\n",  i);
		sleep(1);
	}
	printf("Thread 1 finished.\n");
	return NULL;
}

void* thread_2(void* arg){
	printf("\tThread 2 started...\n");
	struct thread_args* args = (struct thread_args*)arg; 	
	
	seed();
	for(int i = 0; i < args->count; i++){
		int random = get_random(args->from, args->to);
		printf("\tThread 2 Random: %d\n",  random);
		if(random == args->target){
			printf("\tThread 2 exited.\n");
			pthread_exit((void*) &(args->exit_status));	
		}
		sleep(1);
	}
	printf("\tThread 2 finished.\n");
	return NULL;		
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	pthread_t thr1, thr2;	
	int iter_count = 5;
	const int EXIT_BY_TARGET = 1;
	struct thread_args args = {2, 6, 10, 5, EXIT_BY_TARGET};
	if(argc == 6){
		iter_count = atoi(argv[1]);
		args.from = atoi(argv[2]);
		args.to = atoi(argv[3]);
		args.count = atoi(argv[4]);
		args.target = atoi(argv[5]);
	}
	
	if(pthread_create(&thr1, NULL, &thread_1, &iter_count)){
		fprintf(stderr, "Error in thread 1 creation.\n");
		return EXIT_FAILURE;
	}			
	if(pthread_create(&thr2, NULL, &thread_2, &args)){
		fprintf(stderr, "Error in thread 2 creation.\n");
		return EXIT_FAILURE;
	}

	void* status_addr;
	int status = 0;

	if(pthread_join(thr1, NULL)){
		fprintf(stderr, "Error in thread 1 join.\n");
		return EXIT_FAILURE;
	}
	if(pthread_join(thr2, &status_addr)){
		fprintf(stderr, "Error in thread 1 join.\n");
		return EXIT_FAILURE;
	}

	if(status_addr)
		status = *(int*)status_addr;
	if(status == 1)
		printf("Main Thread Info: Thread 2 exited by target complitting\n"); 
	return EXIT_SUCCESS;
}