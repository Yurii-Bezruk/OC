#include "lib.h"

void help(){
	printf("...\n\n");
	printf("Usage: write [arguments] [files] [count] Write random numbers to given count of files\n");
	printf("                                         with specific file name. If file name is not\n");
	printf("                                         set, using \"default*\". If count of files  \n");
	printf("                                         is not set, creating single file.\n\n");
	printf("Arguments:\n");
	printf("-h         --help            Show help (this massage) and exit\n");
	printf("-f [file]  --file [file]     Sets the specific output file name.\n");
	printf("-n [count] --number [count]  Sets the count of output files.\n");
}	

int main(int argc, char* argv[]){ 
	char* count = (char*) malloc(sizeof("1"));
	strcpy(count, "1");
	if(argc > 1){		
		const struct option long_opt[] = {			//длинные опции
			{"help", 0, 0, 'h'},
			{"file", 1, 0, 'f'},
			{"number", 1, 0, 'n'},
			{NULL, 0, NULL, 0}
		};
		int opt;		//текущая опция
		while((opt = getopt_long(argc, argv, "hf:n:", long_opt, NULL)) != -1){
			switch(opt){
				case 'h': 
					help();
					return EXIT_SUCCESS;
				case 'f':
					if((setenv("FILE_NAME", optarg, 1) == -1) || (strcmp(getenv("FILE_NAME"),optarg) != 0))
						fprintf(stderr, "Error! Could not set the variable value\n");				
					break;
				case 'n':
					if(atoi(optarg) == 0){
						fprintf(stderr, "Error! Uncorrect files count\n");
						break;
					}
					count = (char*) realloc(count, sizeof(optarg));
					strcpy(count, optarg);
					break;
				case '?':
					return EXIT_FAILURE;
			}
		}
	}
	char* sub = (char*) malloc(sizeof("./sub ")+sizeof(count)); 
	strcpy(sub, "./sub ");
	int num_count = atoi(count);
	for(int i=1; i<=num_count; i++){
		sprintf(count, "%i", i);   		//перевод номера в строку
		strcat(sub, count);
		system(sub);					//запуск дочерней программы с указанным номером
		for(int i=6; i<strlen(sub); i++)	//очистка номера
			sub[i] = '\0';
	}	
	free(count);
	free(sub);
	return EXIT_SUCCESS;
}