#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
extern char** environ;

int main(int argc, char* argv[]){
	if(argc == 1){
		if(clearenv() == -1){
			fprintf(stderr, "Error! Could not clear enviroment!\n");
			return -1;
		}
		else{
			printf("Enviroment successfuly cleared!\n");
			return 0;
		}
	}
	else{
		if(unsetenv(argv[1]) == -1)
			fprintf(stderr, "Error! Could not delete enviroment variable!\n");		
		else
			printf("Enviroment variable %s successfuly deleted!\n", argv[1]);
	}
	printf("Current enviroment:\n");
	for(char** env = environ; *env; env++){
		printf("%s\n", *env);
	}
	return 0;
}