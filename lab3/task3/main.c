#include "handlers.h"

int main(int argc, char* argv[]){
	if(argc > 1){
		int count = atoi(argv[1]);
		if(count > 32 && count < 0){
			printf("Error! [count] must be >=0 & <=32\n");
			return EXIT_FAILURE;
		}
		for(int i = 0; i < count; i++)
			atexit(handler_arr[i]);
		exit(0);	
	}
	else{
		printf("Usage: reg [count]      Register [count] exit handlers. [count] is >=0 & <=32\n");
	}
	return EXIT_SUCCESS;
}