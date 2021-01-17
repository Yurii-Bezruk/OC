#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct{
	double x;
	double y;
}point;

void options(int argc, char** argv, double* rad, int* num){
	int opt;
	while((opt = getopt(argc, argv, "r:n:")) != -1){
		switch(opt){
			case 'r':
				*rad = atof(optarg);
				break;
			case 'n':
				*num = atoi(optarg);
				break;
		}
	}
}

void seed(){
	struct timespec now;
	if(clock_gettime(CLOCK_REALTIME, &now) < 0){
		fprintf(stderr, "Error with time function!\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);	
}

double get_random(double min, double max){
    return (double)(rand())/RAND_MAX*(max - min) + min;
}

void generate_points(point** points, double rad, int number){
	double from = 0.0-rad, to = 0.0+rad;
	seed();

	(*points)[0].x = get_random(from, to);
	(*points)[0].y = get_random(from, to);
	for(int i = 1; i < number; i++){
		do{
			(*points)[i].x = get_random(from, to);
			(*points)[i].y = get_random(from, to);
		}while((*points)[i].x == (*points)[i-1].x && (*points)[i].y == (*points)[i-1].y);
	}
}

double get_distance(point pt){
	return sqrt(pt.x*pt.x + pt.y*pt.y);
}

int main(int argc, char* argv[]){ 
	double rad = 1.0;
	int number = 10;
	
	options(argc, argv, &rad, &number);	

	point* points = (point*) malloc(number * sizeof(point)); 
	generate_points(&points, rad, number);
	
	int true_points_counter = 0;
	for(int i=0; i<number; i++)
		if(get_distance(points[i]) <= rad)
			true_points_counter++;
	free(points);
	exit(true_points_counter);
}

