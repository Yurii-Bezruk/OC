#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100
#define OUT 0
#define IN 1

void capitalize(char* message){
	int size = strlen(message);
	for(int i = 0; i < size; i++){
		message[i] = toupper(message[i]);
	}
}

int main(void){
	int to[2], from[2];
	char message[MAX_STRING_SIZE];
	int bytes;

	if(pipe(to) < 0){
		fprintf(stderr, "pipe error\n");
		return EXIT_FAILURE;
	}
	if(pipe(from) < 0){
		fprintf(stderr, "pipe error\n");
		return EXIT_FAILURE;
	}

	int res = fork();
	if(res < 0){
		fprintf(stderr, "fork error\n");
		return EXIT_FAILURE;
	}
	else if(res > 0){		//parent
		close(to[OUT]);
		close(from[IN]);

		while(1){
			printf("Enter a string: ");
			fgets(message, MAX_STRING_SIZE, stdin);			
			bytes = write(to[IN], message, MAX_STRING_SIZE);
			if(bytes < 0){
				fprintf(stderr, "parent write error\n");
				return EXIT_FAILURE;
			}
			else if(bytes != MAX_STRING_SIZE){
				fprintf(stderr, "parent write exception\n");
				return EXIT_FAILURE;
			}
			if(!strcmp(message, "STOP\n"))
				break;
			bytes = read(from[OUT], message, MAX_STRING_SIZE);
			if(bytes < 0){
				fprintf(stderr, "parent read error\n");
				return EXIT_FAILURE;
			}
			else if(bytes != MAX_STRING_SIZE){
				fprintf(stderr, "parent read exception\n");
				return EXIT_FAILURE;
			}
			printf("Capitalized: %s", message);
		}
		close(to[IN]);
		close(from[OUT]);
	}
	else{		//sibling
		close(to[IN]);
		close(from[OUT]);

		while(1){
			bytes = read(to[OUT], message, MAX_STRING_SIZE);
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
			bytes = write(from[IN], message, MAX_STRING_SIZE);
			if(bytes < 0){
				fprintf(stderr, "sibling read error\n");
				return EXIT_FAILURE;
			}
			else if(bytes != MAX_STRING_SIZE){
				fprintf(stderr, "sibling read exception\n");
				return EXIT_FAILURE;
			}
		}		
		close(to[OUT]);
		close(from[IN]);
	}

	return EXIT_SUCCESS;
}