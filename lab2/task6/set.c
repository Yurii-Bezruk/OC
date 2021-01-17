#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("This programm is setenv test\n\n");
		printf("Usage: setenv [variable name] [value] Set the value to given enviroment variable\n\n");		
	}
	else{
		if((setenv(argv[1], argv[2], 1) != -1) && (strcmp(getenv(argv[1]),argv[2]) == 0))
			printf("%s=%s\n", argv[1], argv[2]);
		else
			fprintf(stderr, "Error! Could not set the variable value\n");		
	}
	return 0;
}