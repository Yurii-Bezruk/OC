#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
	char* word;
	if(argc > 1){
		word = (char*) malloc(sizeof(argv[1]));
		strcpy(word, argv[1]);
	}
	else{
		word = (char*) malloc(sizeof("goodbye"));
		strcpy(word, "goodbye");
	}
	printf("Say %s\n", word);
	free(word);
	return 0;
}

