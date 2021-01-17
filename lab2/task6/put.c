#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("This programm is putenv test\n\n");
		printf("Usage: putenv [variable name] [value] Set the value to given enviroment variable\n\n");		
	}
	else{
		char var[strlen(argv[1])], val[strlen(argv[1])];
		strcpy(var, argv[1]);
		strcpy(val, argv[2]);	
		char* out = strcat(var, "=");
		out = strcat(out, val);
		if((putenv(out) != -1) && (strcmp(getenv(argv[1]),argv[2]) == 0))
			printf("%s\n", out);
		else
			fprintf(stderr, "Error! Could not set the variable value\n");
		
	}
	return 0;
}