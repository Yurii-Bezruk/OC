#include "opts.h"

void status(){
	extern char** environ;
	printf("Current environment status:\n\n");
	for(char** env = environ; *env; env++){
		printf("%s\n", *env);
	}
}

void help(){
	printf("Environ\n\n");
	printf("Usage: environ [arguments] Print infomation about environment variables\n\n");
	printf("Arguments:\n");
	printf("-h              --help             Show help (this massage) and exit\n");
	printf("-i [var]        --info [var]       Show the environment variable value\n");
	printf("-s [var=value]  --set [var=value]  Set the environment variable as value by the given statement\n");
	printf("-a [var]        --assign [var]     Set the environment variable by given in -v option value.\n");
	printf("                                   If -v option is not given, variable is set by empty string\n");
	printf("-v [value]      --value [value]    Set the value to variable given in -a option.\n");
	printf("-d [var]        --del [var]        Delete the environment variable\n");
	printf("-c              --clear            Clear the environment (delete all environment variables)\n");
}

void info(char* var){
	char* value = getenv(var);
	if(value != NULL)
		printf("%s=%s\n", var, value);
	else
		printf("Variable %s does not exist.\n", var);
}

void set(char* statement){
	if(putenv(statement) != -1)
		printf("Value successfully established.\n%s\n", statement);
	else
		fprintf(stderr, "Error! Could not set the variable value\n");
}

void assign_value(char* var, char* val){
	if((setenv(var, val, 1) != -1) && (strcmp(getenv(var),val) == 0))
			printf("Value successfully established.\n%s=%s\n", var, getenv(var));
		else
			fprintf(stderr, "Error! Could not set the variable value\n");
}

void del(char* var){
	if(unsetenv(var) != -1)
		printf("Enviroment variable %s successfuly deleted!\n", var);
	else
		fprintf(stderr, "Error! Could not delete enviroment variable!\n");		
}

void clear(){
	if(clearenv() != -1)
		printf("Enviroment successfuly cleared!\n");
	else
		fprintf(stderr, "Error! Could not clear enviroment!\n");
		
}