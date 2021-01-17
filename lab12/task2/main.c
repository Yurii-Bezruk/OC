#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_STRING_SIZE 100

int main(void){
	int to, from;
	char message[MAX_STRING_SIZE];
	int bytes;

	char* fifo1 = "fifo1.fifo";
	char* fifo2 = "fifo2.fifo";

	(void)umask(0);

	if(mknod(fifo1, S_IFIFO | 0666, 0) < 0){
		fprintf(stderr, "mknod 1 error\n");
		return EXIT_FAILURE;
	}
	if(mknod(fifo2, S_IFIFO | 0666, 0) < 0){
		fprintf(stderr, "mknod 2 error\n");
		return EXIT_FAILURE;
	}
	if((to = open(fifo1, O_WRONLY)) < 0){
		fprintf(stderr, "open 1 error\n");
		return EXIT_FAILURE;
	}
	if((from = open(fifo2, O_RDONLY)) < 0){
		fprintf(stderr, "open 2 error\n");
		return EXIT_FAILURE;
	}
	while(1){
		printf("Enter a string: ");
		fgets(message, MAX_STRING_SIZE, stdin);			
		bytes = write(to, message, MAX_STRING_SIZE);
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

		bytes = read(from, message, MAX_STRING_SIZE);
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
	close(to);
	close(from);

	unlink(fifo1);
	unlink(fifo2);
	return EXIT_SUCCESS;
}