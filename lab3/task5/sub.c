#include "lib.h"

void writetofile(char* filename, int count){
	srand(time(NULL));
	FILE* file = fopen(filename, "w");
	for(int i=0; i<count; i++){
		fprintf(file, "%.2f ", (rand()%100)/100.0);
	}
	fprintf(file, "\n");
	fclose(file);
}

int main(int argc, char** argv){	
	char number[strlen(argv[1])];
	strcpy(number, argv[1]);
	int count = atoi(number);	
	printf("Sublim %d is running...\n", count);
	strcat(number, ".txt");
	char* name = (char*) malloc (sizeof("default")+sizeof(number));
	strcpy(name, "default");	
	char* value = getenv("FILE_NAME");
	if(value != NULL){
		if(value[0] != '\0'){
			name = (char*)realloc(name, sizeof(value)+sizeof(number));
			strcpy(name, value);
		}
	}
	strcat(name, number);
	writetofile(name, count);
	free(name);	
	printf("Sublim %d is finished.\n", count);
	return EXIT_SUCCESS;
}