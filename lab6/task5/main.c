#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct{
	int from, to;
}range;

int digits(int value){
	int counter = 1;
	while(value/=10)
		counter++;
	return counter;
}

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

static pthread_key_t str_key;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

void close_str(void* str){
	free(str);
}

void key_init(void){
	if(pthread_key_create(&str_key, close_str)){
		fprintf(stderr, "Error in thread key creating.\n");
		exit(EXIT_FAILURE);
	}
}



void* thread_func(void* args){
	if(pthread_once(&once_control, key_init)){
		fprintf(stderr, "Error in once key initializing.\n");
		exit(EXIT_FAILURE);
	}
	int id = (int) pthread_self();
	char* common = "thread is running. Iteration:";
	char* full = (char*) malloc(sizeof(char)*(strlen(common) + digits(id) + 2));
	sprintf(full, "%d ", id);
	strcat(full, common);
	if(pthread_setspecific(str_key, full)){	
		fprintf(stderr, "Error in key setting.\n");
		exit(EXIT_FAILURE);
	}

	range* arg = (range*)args;
	seed();
	int iter_count = get_random(arg->from, arg->to);
	for(int i=0; i<iter_count; i++){
		char* str = (char*) pthread_getspecific(str_key);
		if(!str){
			fprintf(stderr, "Error in key getting.\n");
			exit(EXIT_FAILURE);
		}
		printf("%s %d/%d. Random: %d\n", str, i+1, iter_count, get_random(arg->from, arg->to));
		sleep(1);
	}
	return NULL;
}

void create_threads(pthread_t threads[], int count, range* arg){
	for(int i=0; i<count; i++){
		if(pthread_create(threads+i, NULL, &thread_func, arg)){
			fprintf(stderr, "Error in thread creation.\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i=0; i<count; i++){	
		if(pthread_join(threads[i], NULL)){
			fprintf(stderr, "Error in thread join.\n");
			exit(EXIT_FAILURE);
		}	
	}
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	int count = 1;
	range arg = {1, 10};
	if(argc == 2)
		count = atoi(argv[1]);
	else if(argc == 4){
		count = atoi(argv[1]);
		arg.from = atoi(argv[2]);
		arg.to = atoi(argv[3]);
	}	
	pthread_t threads[count];
	
	create_threads(threads, count, &arg);
	return EXIT_SUCCESS;
}