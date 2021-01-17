#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("This programm is getenv test\n\n");
		printf("Usage: getenv [variable names] Print the value of the given enviroment variables\n\n");
		
	}
	else{
		for(int i = 1; i<argc; i++){
			printf("%s=%s\n", argv[i], getenv(argv[i]));
		}
	}
	return 0;
}