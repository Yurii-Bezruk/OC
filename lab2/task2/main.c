#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void help(){
	printf("This programm is getopt test\n\n");
	printf("Usage: opts [arguments] [files] Write information into a files\n\n");
	printf("Arguments:\n");
	printf("-h   Show help (this massage) and exit\n");
	printf("-o   Establish specific file name\n");
	printf("-c   Enable a special mode\n");
}

int main(int argc, char* argv[]){ 
	char* filepath = "data.txt";
	char* mode = "SIMPLE";
	int opt;
	while((opt = getopt(argc, argv, ":ho:c")) != -1){
		switch(opt){
			case 'h': 
				help();
				return 0;
			case 'o':
				filepath = optarg;
				break;
			case 'c':
				mode = "SPECIAL";
				break;
			case '?':
				printf("Unknown argument -%c. Try this: -h -o -c\n", (char)optopt);
				return -1;
			case ':':
				printf("No arguments for option -%c. Try this: -%c [files]\n", (char)optopt, (char)optopt);
				return -1;
		}
	}
	FILE* file = fopen(filepath, "w");
	if(file == NULL){
		printf("Could not open the file!");
		return -1;
	}
	fprintf(file, "This is a test file %s for opts programm, started in %s mode\n", filepath, mode);

	if (optind < argc){
		for(int i = optind; i<argc; i++){
			FILE* addition_file = fopen(argv[i], "w");
			if(addition_file != NULL){
				fprintf(addition_file, "This is an addition test file %s for opts programm, started in %s mode\n<---->", argv[i], mode);
				fclose(addition_file);
			}
			else{
				printf("Could not open the addition file %s!\n", argv[i]);
			}
		}		
	}
	fclose(file);
	printf("Information was written into %s file\n\n", filepath);
	return 0;
}