#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
	double a, b;
	double* e;
	double* result;
	pthread_mutex_t* lock;
	double (*function) (double x);
}th_arg;

//[0; 2]:
double f1(double x){		//5.3333333
	return (4 - x*x);
}
double f2(double x){		//9.3333333
	return (x*x*x + 2*x*x);
}
double f3(double x){		//7.3333333
	return 2*x*x + fabs(x);		
}	

double integral(double a, double b, double e, double (*f)(double x) ){		// [a, b]; n - число разбиений
	double n = 100;
	double I2n = 0.0;
	double In = 2 * In;					//интегральные сумма
	while(1){
		double h = (b-a)/n;				//шаг разбиения		
		for(double x = a; x <= b; x += h)
			I2n += f(x + h/2);
		I2n *= h;

		if(fabs(In - I2n) < e)		//удваиваем количество интервалов пока не достигнем заданной точности
			break;
		else{
			In = I2n;
			I2n = 0.0;
			n *= 2;
		}
	}
	return I2n;
}

void* thread_func(void* argument){
	th_arg* arg = (th_arg*) argument;
	double I = integral(arg->a, arg->b, *(arg->e), arg->function);
	pthread_mutex_lock(arg->lock);
	(*(arg->result)) += I;
	pthread_mutex_unlock(arg->lock);
	return NULL;
}

void options(int argc, char** argv, int* t_count_addr, double* a_addr, double* b_addr, double* e_addr);

int main(int argc, char** argv){
	double RESULT = 0.0, A = 0.0, B = 2.0, E = 1e-10;
	double (*FUNCTION)(double x) = f1;
	int thread_count = 1;
	if(argc > 1)
		options(argc, argv, &thread_count, &A, &B, &E);

	pthread_mutex_t MUTEX;
	if(pthread_mutex_init(&MUTEX, NULL)){
		fprintf(stderr, "Error in mutex initialization\n");
		exit(EXIT_FAILURE);
	}

	th_arg* arg = (th_arg*) malloc(thread_count * sizeof(th_arg));
	if(!arg){
		fprintf(stderr, "Memory error during mutex dinamic allocation\n");
		exit(EXIT_FAILURE);
	}
	
	double H = (B - A)/thread_count;
	pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	for(int i = 0; i < thread_count; i++){
		arg[i].a = A + i*H;
		arg[i].b = arg[i].a + H;
		arg[i].e = &E;
		arg[i].result = &RESULT;
		arg[i].lock = &MUTEX;
		arg[i].function = FUNCTION;
		if(pthread_create(threads + i, NULL, &thread_func, arg + i)){
			fprintf(stderr, "Error in thread creation\n");
			exit(EXIT_FAILURE);
		}
	}
	for(int i = 0; i < thread_count; i++){
		if(pthread_join(threads[i], NULL)){
			fprintf(stderr, "Error in thread join\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("I = %.10f\n", RESULT);
	pthread_mutex_destroy(&MUTEX);
	free(arg);
	free(threads);
	return EXIT_SUCCESS;
}

void options(int argc, char** argv, int* t_count_addr, double* a_addr, double* b_addr, double* e_addr){
	int opt;
	while((opt = getopt(argc, argv, "hf:t:c:e:")) != -1){
		switch(opt){
			case 'h': 
				printf("Usage: ./integral [-h] | [[-f <number>] [-t <number>] | [-c <count>] | [-e <number>]]\n\n");
				printf("Arguments:\n");
				printf("-h           Show help (this message) and exit\n");
				printf("-f <number>  Set the lower integration limit (FROM)\n");
				printf("-t <number>  Set the higher integration limit (TO)\n");
				printf("-c <count>   Set the specific thread count\n");
				printf("-e <number>  Set the integration epsilon-precision\n");
				exit(EXIT_SUCCESS);
			case 'f':
				*a_addr = atof(optarg);
				break;
			case 't':
				*b_addr = atof(optarg);
				break;
			case 'c':
				*t_count_addr = atoi(optarg);
				break;
			case 'e':
				*e_addr = atof(optarg);
				break;			
		}
	}

}