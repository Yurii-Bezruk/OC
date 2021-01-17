#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pwd.h>
#include <errno.h>

void help(){
	printf("Simple command processor\n\n");
	printf("Usage: exe [arguments] [size] \n\n");
	printf("Arguments:\n");
	printf("-h        --help          Show help (this massage) and exit\n");
	printf("-b [size] --buffer [size] Run processor with given command buffer size. Default: 127\n");
}

void exit_handler(void){
	printf("Wait 3 seconds...\n");
	sleep(3);
	system("clear");
}

int main(int argc, char* argv[]){ 
	size_t command_size = 127;
	int result;
	atexit(exit_handler);			//обработчик выхода

	const struct option long_opt[] = {			//длинные опции
		{"help", 0, 0, 'h'},
		{"buffer", 1, 0, 'b'},
		{NULL, 0, NULL, 0}
	};
	int opt;		//текущая опция
	while((opt = getopt_long(argc, argv, "hb:", long_opt, NULL)) != -1){
		switch(opt){
			case 'h': 
				help();
				return EXIT_SUCCESS;
			case 'b':
				command_size = atoi(optarg);
				break;
			case '?':
				return EXIT_FAILURE;
		}
	}
	struct passwd* info = getpwuid(getuid());	//получение информации о пользователе
	char* user = info->pw_name;
	printf("Working:\nFullName  UID  Shell\n");
	printf("%s  %i  %s\n\n", info->pw_gecos, info->pw_uid, info->pw_shell);
	
	//выделение буфера команд
	char* command = (char*) malloc(command_size* sizeof(char));
	if(command == NULL){
		fprintf(stderr, "Memory error.\n");
		return EXIT_FAILURE;
	}

	while(1){		//командный процессор
		printf("%s$ ", user);
		if(getline(&command, &command_size, stdin) == -1)
			fprintf(stderr, "%s\n", strerror(errno));
		if(strcmp(command, "stop\n") == 0)		//команда стоп завершает работу
			break;
		result = system(command);		//вывод результата
		if(result >= 0 && result != 127)
			printf("Result: %d\n", result);
		else
			fprintf(stderr, "Executing error: %s\n", strerror(errno));
	}

	free(command);
	return EXIT_SUCCESS;
}