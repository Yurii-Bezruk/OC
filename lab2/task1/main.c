#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
	if(argc < 4){
		printf("Too few arguments...\n");
		return -1;
	}
	double a = atof(argv[1]);
	double b = atof(argv[2]);
	double c = atof(argv[3]);

	double D = b*b - 4*a*c;
	if(D >= 0){
		double x1 = (-b + sqrt(D))/(2*a);
		double x2 = (-b - sqrt(D))/(2*a);		
		printf("(%.1f, %.1f)\n", x1, x2);
		return 0;
	}
	else{
		printf("%s\n", "D < 0 !!!");
		return -1;
	}	
}