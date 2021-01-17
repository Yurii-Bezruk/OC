#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

void print_info(struct passwd* user){
	if(user != NULL){
		printf("%s info:\n", user->pw_name);
		printf("\tName:      %s\n", user->pw_name);
		printf("\tUID:       %d\n", user->pw_uid);
		printf("\tGID:       %d\n", user->pw_gid);
		printf("\tDir:       %s\n", user->pw_dir);
		printf("\tFull name: %s\n", user->pw_gecos);
		printf("\tShell:     %s\n", user->pw_shell);
	}
	else 
		fprintf(stderr, "Error! could not get information!\n");	
}

int main(void){
	struct passwd* info = getpwuid(getuid());
	print_info(info);
	info = getpwnam("root");
	print_info(info);
	return 0;
}