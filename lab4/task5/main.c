#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pwd.h>
#include <errno.h>
#include <wait.h>

extern char** environ;
extern int errno;

void help();
void options(int argc, char* argv[], size_t* command_size_address);
int my_system(char* command);
void split_by(char* str, char*** words, int* size, char* splitter);


int main(int argc, char* argv[]){ 
	size_t command_size = 127;
	int result = 0;						//результат выполнения программы
	options(argc, argv, &command_size);
	
	struct passwd* info = getpwuid(getuid());	//получение информации о пользователе
	char* user = info->pw_name;
	printf("Working:\nFullName  UID  Shell\n");
	printf("%s  %i  %s\n\n", info->pw_gecos, info->pw_uid, info->pw_shell);
	
	//выделение буфера команд
	char* command = (char*) malloc((command_size + 1)* sizeof(char));
	if(!command){
		fprintf(stderr, "Memory error.\n");
		return EXIT_FAILURE;
	}

	while(1){		//командный процессор
		printf("%s$ ", user);
		if(getline(&command, &command_size, stdin) == -1){
			fprintf(stderr, "%s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		command[strlen(command)-1]='\0';
		if(strcmp(command, "stop") == 0)		//команда стоп завершает работу
			break;
		result = my_system(command);		//вывод результата
		if(result >= 0 && result != 127)
			printf("Result: %d\n", result);
		else
			fprintf(stderr, "Executing error: %s\n", strerror(errno));
	}

	free(command);
	return EXIT_SUCCESS;
}

void help(){
	printf("Simple command processor\n\n");
	printf("Usage: exe [arguments] [size] \n\n");
	printf("Arguments:\n");
	printf("-h        --help          Show help (this massage) and exit\n");
	printf("-b [size] --buffer [size] Run processor with given command buffer size. Default: 127\n");
}

void options(int argc, char* argv[], size_t* command_size_address){
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
				exit(EXIT_SUCCESS);
			case 'b':
				*command_size_address = atoi(optarg);
				break;
			case '?':
				exit(EXIT_FAILURE);
		}
	}
}

int my_system(char* command){	
	char** commands;
	int command_count;

	split_by(command, &commands, &command_count, ";");
	
	for(int i=0; i<command_count; i++){
		char** args;
		int arg_count;
		split_by(commands[i], &args, &arg_count, " ");
		char** temp = (char**) realloc(args, (arg_count+1)*sizeof(char*));
		if(!temp){
			fprintf(stderr, "Memory error.\n");
			return EXIT_FAILURE;
		}
		args = temp;
		args[arg_count]=NULL;

		pid_t pid = fork();
		if(!pid){
			execve(args[0], args, environ);
			execvp(args[0], args);
			fprintf(stderr, "Executing error: %s\n", strerror(errno));
			return -1;
		}
		else if(pid > 0){
			while(1){
				int res = wait(NULL);
				if(res == -1 && errno == ECHILD)
					break;
				
			}			
		}
		else{
			fprintf(stderr, "Fork error: %s\n", strerror(errno));
			return -1;
		} 
		free(args);
	}
	free(commands);
	return 0;
}

void split_by(char* str, char*** words, int* wordcount, char* splitter){
	int size = 5;
	*words = (char**) malloc(size* sizeof(char*));
	if(!(*words)){
		fprintf(stderr, "Memory error.\n");
		exit(EXIT_FAILURE);
	}
	for(int i=0; i<size; i++){
		(*words)[i] = (char*) malloc(strlen(str)* sizeof(char*));
		if(!((*words)[i])){
			fprintf(stderr, "Memory error.\n");
			exit(EXIT_FAILURE);
		}
	}

	char* word = strtok(str, splitter);	
	for(*wordcount = 0; word && (*wordcount <= size); (*wordcount)++){			
		if(*wordcount == size){
			size *= 2;
			char** temp = (char**) realloc(*words, size * sizeof(char*));
			if(!temp){
				fprintf(stderr, "Memory reallocation error.\n");
				break;
			}
			*words = temp;
		}
		strcpy((*words)[*wordcount], word);
		word = strtok(NULL, splitter);		
	}
}
// #include <errno.h>
// #include <string.h>
// #include <wait.h>

// #define DOT_COUNT 15

// extern int errno;


// int main(int argc, char** argv){
// 	int i, status;

// 	if(!fork()){
// 		for(i=0; i<DOT_COUNT; i++){
// 			fprintf(stderr, ".");
// 			sleep(1);
// 		}
// 		exit(5);
// 	}
// 	while(1){
// 		if(!waitpid(-1, &status, WNOHANG))
// 			fprintf(stderr, "*");
// 		else{
// 			fprintf(stderr, "(exit)\n");
// 			break;
// 		}
// 		sleep(2);
// 	}
// 	if(WIFEXITED(status)){
// 		fprintf(stderr, "Exited with code=%d\n", WIFEXITED(status));
// 	}
// 	else if(WIFSIGNALED(status)){
// 		fprintf(stderr, "Exited by signal\n");
// 	}
// 	return EXIT_SUCCESS;
// }