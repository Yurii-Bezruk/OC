#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>

void print_info(struct utsname* info, char* hostname, long hostid){
	printf("Machine info:\n");
	printf("\tSystem name:    %s\n", info->sysname);
	printf("\tNode name:      %s\n", info->nodename);
	printf("\tRelease number: %s\n", info->release);
	printf("\tSystem version: %s\n", info->version);
	printf("\tMachine type:   %s\n", info->machine);
	if(hostname != NULL)
		printf("\tHost name:      %s\n", hostname);
	printf("\tHost ID:        %ld\n", hostid);
}

int main(void){ 
	struct utsname* info = (struct utsname*) malloc(sizeof(struct utsname));
	if(uname(info) != -1){
		char* name = (char*) malloc(10*sizeof(char));
		if(gethostname(name, 10) == -1){
			free(name);
			name = NULL;
		}
		print_info(info, name, gethostid());
		free(name);
	}
	else
		printf("Error: %s\nCannot get information!\n", strerror(errno));
	free(info);
	return 0;
}