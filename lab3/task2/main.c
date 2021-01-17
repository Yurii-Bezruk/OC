#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	int result;
	if(argc > 2){
		char* command = (char*) malloc(sizeof(char) * strlen(argv[1]));
		if(command == NULL){
			fprintf(stderr, "Memory error.\n");
			return -1;
		}
		strcpy(command, argv[1]);
		for(int i=2; i<argc; i++){
			command = strcat(command, " ");
			command = strcat(command, argv[i]);	
		}
		result = system(command);
		if(result >= 0 && result != 127)
			printf("Результат выполнения: %d\n", result);
		else
			printf("Ошибка выполнения. \n");
		free(command);
	}
	else if(argc == 2){
		result = system(argv[1]);
		if(result >= 0 && result != 127)
			printf("Результат выполнения: %d\n", result);
		else
			printf("Ошибка выполнения. \n");
	}
	else{
		printf("No arguments to run.\n");
		printf("Usage: run [command]\n");
		printf("    or run [command] [options]\n");
		printf("    or run \"[command] [options] ; [command] [options] ; ...\"\n");
	}
	return EXIT_SUCCESS;
}