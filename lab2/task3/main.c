#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void help(){
	printf("This programm is getopt test\n\n");
	printf("Usage: opts [arguments] [files] Write information into a files\n\n");
	printf("Arguments:\n");
	printf("-h  --help     Show help (this massage) and exit\n");
	printf("-o  --output   Establish specific file name\n");
	printf("-c  --compile  Enable a special mode\n");
}

int main(int argc, char* argv[]){ 
	char* filepath = "data.txt";
	char* mode = "SIMPLE";
	const struct option long_opt[] = {
		{"help", 0, 0, 'h'},
		{"output", 1, 0, 'o'},
		{"compile", 0 , 0, 'c'},
		{NULL, 0, NULL, 0}
	};
	int opt;
	while((opt = getopt_long(argc, argv, "ho:c", long_opt, NULL)) != -1){
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
			case '?':;
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