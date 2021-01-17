#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100

void capitalize(char* message){
	int size = strlen(message);
	for(int i = 0; i < size; i++){
		message[i] = toupper(message[i]);
	}
}

int main(void){
	int to, from;
	char message[MAX_STRING_SIZE];
	int bytes;


	char* fifo1 = "fifo1.fifo";
	char* fifo2 = "fifo2.fifo";

	if((from = open(fifo1, O_RDONLY)) < 0){
		fprintf(stderr, "open 1 error\n");
		return EXIT_FAILURE;
	}
	if((to = open(fifo2, O_WRONLY)) < 0){
		fprintf(stderr, "open 2 error\n");
		return EXIT_FAILURE;
	}
	
	while(1){
		bytes = read(from, message, MAX_STRING_SIZE);
		if(bytes < 0){
			fprintf(stderr, "sibling write error\n");
			return EXIT_FAILURE;
		}
		else if(bytes != MAX_STRING_SIZE){
			fprintf(stderr, "sibling write exception\n");
			return EXIT_FAILURE;
		}
		if(!strcmp(message, "STOP\n"))
			break;
		capitalize(message);
		bytes = write(to, message, MAX_STRING_SIZE);
		if(bytes < 0){
			fprintf(stderr, "sibling read error\n");
			return EXIT_FAILURE;
		}
		else if(bytes != MAX_STRING_SIZE){
			fprintf(stderr, "sibling read exception\n");
			return EXIT_FAILURE;
		}
	}
	close(to);
	close(from);

	return EXIT_SUCCESS;
}