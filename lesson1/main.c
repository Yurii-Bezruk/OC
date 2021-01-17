#include <stdio.h>
#include <stdlib.h>
#include "include/func.h"

int main(void){
	double x, res1, res2;
	printf("x:");
	scanf("%lf", &x);
	res1 = f(x); res2 = g(x);
	printf("f(%5.2f)=%g\n", x, res1);
	printf("g(%5.2f)=%g\n", x, res2);

	return EXIT_SUCCESS;
}