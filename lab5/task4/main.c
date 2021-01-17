#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <getopt.h>

typedef struct {
	int count;
	long* arr;
}thread_args;

int is_simple(long n){
	for(long i = 2; i <= sqrt(n); i++)
		if(!(n % i))
			return 0;	
	return 1;
}

long fact(long n){
	if(n == 0)
		return 1;
	else if(n == 1)
		return 1;
	else 
		return n * fact(n-1);
}

void* thread_catalan(void* argument){
	thread_args* args = (thread_args*) argument;
	for(int n = 0; n < args->count; n++){
		args->arr[n] = fact(2*n)/(fact(n+1) * fact(n));
	}
	return (void*) (args->arr);
}

void* thread_simple(void* argument){
	thread_args* args = (thread_args*) argument;
	int counter = 0;
	for(long n = 0; counter < args->count; n++){
		if(is_simple(n)){
			args->arr[counter] = n;
			counter++;
		}
	}
	return (void*) (args->arr);		
}

int main(int argc, char** argv){
	setbuf(stdout, NULL);
	pthread_t catalan, simple;
	int catalan_count = 10, simple_count = 10;
	if(argc > 1){
		int opt;
		while((opt = getopt(argc, argv, "c:s:")) != -1){
			switch(opt){
				case 'c':
					catalan_count = atoi(optarg);
					break;
				case 's':
					simple_count = atoi(optarg);
					break;
			}
		}	
	}
	long* catalan_arr = (long*) malloc(catalan_count * sizeof(long));
	long* simple_arr = (long*) malloc(simple_count * sizeof(long));
	thread_args catalan_args = {catalan_count, catalan_arr};
	thread_args simple_args = {simple_count, simple_arr};
	
	if(pthread_create(&catalan, NULL, &thread_catalan, &catalan_args)){
		fprintf(stderr, "Error in thread 1 creation.\n");
		return EXIT_FAILURE;
	}			
	if(pthread_create(&simple, NULL, &thread_simple, &simple_args)){
		fprintf(stderr, "Error in thread 2 creation.\n");
		return EXIT_FAILURE;
	}

	void* result = NULL;
	if(pthread_join(catalan, &result)){
		fprintf(stderr, "Error in thread 1 join.\n");
		return EXIT_FAILURE;
	}
	if(result)
		catalan_arr = (long*)result;	
	if(pthread_join(simple, &result)){
		fprintf(stderr, "Error in thread 2 join.\n");
		return EXIT_FAILURE;
	}
	if(result)
		simple_arr = (long*)result;	

	printf("First %d Catalan digits: ", catalan_count);
	for(int i=0; i<catalan_count; i++)
		printf("%ld ", catalan_arr[i]);
	printf("\nFirst %d simple digits: ", simple_count);
	for(int i=0; i<simple_count; i++)
		printf("%ld ", simple_arr[i]);
	printf("\n");
	free(catalan_arr);
	free(simple_arr);
	return EXIT_SUCCESS;
}