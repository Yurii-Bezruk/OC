#include "my_rand.h"

void seed(){
	struct timespec now;
	if(clock_gettime(CLOCK_REALTIME, &now) < 0)
	srand(now.tv_nsec);	
}

int int_random(int min, int max){
    return rand()%(max - min) + min;
}
double double_random(double min, double max){
     return (double)(rand())/RAND_MAX*(max - min) + min;
}