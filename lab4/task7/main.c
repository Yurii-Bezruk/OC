#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <math.h>


int main(int argc, char* argv[]){ //начальные значения
	int sibling_count = 1;
	char command[14] = "./point_count";
	char* rad_str = (char*) malloc(sizeof(char) * 4);
	if(!rad_str){
		fprintf(stderr, "Memory error\n");
		return EXIT_FAILURE;
	}
	strcpy(rad_str, "5.0");
	char* num_str = (char*) malloc(sizeof(char) * 3);
	if(!num_str){
		fprintf(stderr, "Memory error\n");
		return EXIT_FAILURE;
	}	
	strcpy(num_str, "10");
	//опции
	int opt;
	char* temp;
	while((opt = getopt(argc, argv, "r:n:c:")) != -1){
		switch(opt){
			case 'r':
				temp = realloc(rad_str, sizeof(char) * (strlen(optarg) + 1));
				if(!temp){
					fprintf(stderr, "Memory error\n");
					return EXIT_FAILURE;
				}
				rad_str = temp;
				strcpy(rad_str, optarg);
				break;
			case 'n':
				temp = realloc(num_str, sizeof(char) * (strlen(optarg) + 1));
				if(!temp){
					fprintf(stderr, "Memory error\n");
					return EXIT_FAILURE;
				}
				num_str = temp;
				strcpy(num_str, optarg);
				break;
			case 'c':
				sibling_count = atoi(optarg);
				break;
		}
	}	

	double rad = atof(rad_str);			//радиус круга
	double n = atof(num_str);			//общее количество точек
	double Sp = (2 * rad) * (2 * rad);			//площать квадрата, описанного вокруг круга
	double results[sibling_count];		//массив результатов
	//создание потомков и вычисление
	for(int i = 0; i<sibling_count; i++){
		pid_t pid = fork();
		if(!pid){		
			execl(command, command, "-r", rad_str, "-n", num_str, NULL);
			fprintf(stderr, "Executing error.\n");
			exit(EXIT_FAILURE);
		}
		else if(pid > 0){
			int status = 0;
			wait(&status);
			int m;
			if(WIFEXITED(status))
				m = WEXITSTATUS(status);			//количество точек, попавших в область круга
			else{
				fprintf(stderr, "Uncorrect process exit.\n");
				exit(EXIT_FAILURE);
			}			
			results[i] = (m/n)*Sp;				//приблизительная площадь круга
		}
		else{
			fprintf(stderr, "Fork error!\n");
			exit(EXIT_FAILURE);
		}
	}
	free(rad_str);
	free(num_str);
	//среднее значение
	double S = 0.0;
	for(int i = 0; i<sibling_count; i++)
		S += results[i];	
	S /= sibling_count;
	
	double pi = S/(rad * rad);
	double error = fabs(M_PI-pi);
	printf("Circe area: %.4f\n", S); 
	printf("Approximate PI: %.4f\n", pi);
	printf("Absolute error: %.4f\n", error);
	exit(EXIT_SUCCESS);
}

