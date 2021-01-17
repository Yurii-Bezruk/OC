#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
//void exit(int status),
//void _Exit(int status)

int main(void){
	int i=0;
	setbuf(stdout, NULL);
	for(i=0; i<10; i++){
		printf(".");
		sleep(1);
	}
	printf("\n");
}