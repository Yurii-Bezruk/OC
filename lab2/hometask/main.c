#include "opts.h"

int main(int argc, char* argv[]){
	if(argc == 1){
		status();
		return 0;
	}
	const struct option long_opt[] = {
		{"help", 0, 0, 'h'},
		{"info", 1, 0, 'i'},
		{"set", 1 , 0, 's'},
		{"assign", 1 , 0, 'a'},
		{"value", 1 , 0, 'v'},
		{"del", 1 , 0, 'd'},
		{"clear", 0, 0, 'c'},
		{NULL, 0, NULL, 0}
	};
	int opt;
	char* variable = NULL;
	char* value = NULL;
	while((opt = getopt_long(argc, argv, "hi:s:a:v:d:c", long_opt, NULL)) != -1){
		switch(opt){
			case 'h': 
				help();
				return 0;
			case 'i':
				info(optarg);
				break;
			case 's':
				set(optarg);
				break;
			case 'a':
				variable = (char*) malloc(strlen(optarg) * sizeof(char));
				strcpy(variable, optarg);
				break;
			case 'v':
				value = (char*) malloc(strlen(optarg) * sizeof(char));
				strcpy(value, optarg);
				break;
			case 'd':
				del(optarg);
				break;
			case 'c':
				clear();
				break;
			case '?':
				return -1;
		}
	}
	if(variable == NULL && value == NULL){
		return 0;
	}
	else{			//если хотя бы одна из опций -а или -v была вызвана
		if(variable == NULL){
			fprintf(stderr, "Error! Variable for value %s is not set!\n", value);
			free(value);
			return -1;
		}
		if(value == NULL){
			value = (char*) malloc(sizeof(char));
			value[0] = '\0';
		}
		assign_value(variable, value);
		free(variable);
		free(value);
		return 0;
	}
}