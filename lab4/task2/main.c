#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
extern int errno;

int main(int argc, char** argv){
	time_t seconds = 3;
	if(argc > 1)
		seconds = atoi(argv[1]);
	if(seconds <= 0){
		fprintf(stderr, "Non-valid number of seconds!\n");
		return EXIT_FAILURE;
	}
	long counter = 0;
	if(fork() < 0){
		fprintf(stderr, "Error in process creating: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	time_t start = time(NULL);
	while((time(NULL) - start) < seconds){
		counter++;
	}	
	printf("Process %d made %ld iterations in %ld seconds\n", getpid(), counter, seconds);
	return EXIT_SUCCESS;
}