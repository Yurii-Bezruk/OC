#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
extern int errno;
	

int my_execvp(char* file, char** args){	
	char* path = getenv("PATH");
	if (!path)
		return -1;
    char *last;
    char* tokens[strlen(path)/2];
    tokens[0] = strtok_r(path, ":", &last);
    for (int i = 1; 1; i++) {
        tokens[i] = strtok_r(NULL, ":", &last);
        if(!tokens[i])
        	break;        
    }    
	for(int i = 0; tokens[i]; i++){
		char command[strlen(tokens[i])+strlen(file)+1];
		strcpy(command, tokens[i]);
		strcat(command, "/");
		strcat(command, file);
		execv(command, args);
	}
	return -1;
}

int my_execlp(char* file, ...){
	char* path = getenv("PATH");
	if (!path)
		return -1;
    char *last;
    char* tokens[strlen(path)/2];
    tokens[0] = strtok_r(path, ":", &last);
    for (int i = 1; 1; i++) {
        tokens[i] = strtok_r(NULL, ":", &last);
        if(!tokens[i])
        	break;        
    }

    char** args = (char**) malloc(sizeof(char*));
    va_list factor;
    va_start(factor, file);
    args[0] = va_arg(factor, char*);
    int size = 0;
    for(int i=1; 1; i++){
    	char* arg = va_arg(factor, char*);
    	char** tmps = (char**) realloc(args, (i+1)*sizeof(char*));
    	if(tmps){
    		args = tmps;
    		args[i] = arg;
    	}
    	size++;
    	if(!arg)
    		break;
    }
    size++;    
    va_end(factor);   
     
	for(int i = 0; tokens[i]; i++){
		char command[strlen(tokens[i])+strlen(file)+1];
		strcpy(command, tokens[i]);
		strcat(command, "/");
		strcat(command, file);
		char* final_args[size];
		for(int i=0; i<size-1; i++)
			memcpy(final_args[i], args[i], strlen(args[i]));
		final_args[size-1]=NULL;
		free(args);
		execv(command, final_args);
	}
	return -1;
}

int main(int argc, char** argv){
	if(setenv("PATH", "/home/yurii/OC/lab4:/home/yurii/OC/lab4/task4", 1) < 0)
		fprintf(stderr, "Path set error!!!\n");

	char* args[] = {"smth", NULL};
	my_execlp("smth", "smth", "hello", NULL);
	fprintf(stderr, "Error: %s\n", strerror(errno));
	// my_execvp("smth", args);
	// fprintf(stderr, "Error: %s\n", strerror(errno));
	return EXIT_SUCCESS;
}