#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(void){
	int a = 6;
	if(!fork()){
		a=1;
		printf("%d\n", a);
		sleep(10);
	}
	else{
		//	a=3;
		printf("%d\n", a);
		sleep(3);
	}
}