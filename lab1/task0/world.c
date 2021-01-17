#include "world.h"

struct world createPoints(struct world space){		//создает и заполняет массив точек
	srand(time(0));
	space.points = (struct point*) malloc(space.size * sizeof(struct point));
	for(int i=0; i<space.size; i++){
		int unique = TRUE;
		do{
			space.points[i].x=rand() % (space.MAX_DISTANCE - space.MIN_DISTANCE) + space.MIN_DISTANCE;
			space.points[i].y=rand() % (space.MAX_DISTANCE - space.MIN_DISTANCE) + space.MIN_DISTANCE;
			space.points[i].z=rand() % (space.MAX_DISTANCE - space.MIN_DISTANCE) + space.MIN_DISTANCE;
			for(int j=0; j<i; j++)
				if(space.points[j].x == space.points[i].x &&
					space.points[j].y == space.points[i].y &&
					space.points[j].z == space.points[i].z)
					unique = FALSE;
		}while(!unique);
		space.points[i].weight=rand() % (space.MAX_WEIGHT-space.MIN_WEIGHT) + space.MIN_WEIGHT;
	}
	return space;
}

int min_weight_point(struct world space){		//возвращает индекс минимального элемента
	int min_weight = space.points[0].weight;
	int min_index = 0;
	for (int i = 0; i < space.size; i++)	{
		if(space.points[i].weight < min_weight){
			min_weight = space.points[i].weight;
			min_index = i;
		}		
	}
	return min_index;
}

struct world disappear(struct world space){		//исчезновение точки
	int pnt_num = min_weight_point(space);
	double min_distance = space.MAX_DISTANCE * space.MAX_DISTANCE;
	double current_distance;
	int point_index;
	for(int i = 0; i < space.size; i++){
		if(i != pnt_num){
			current_distance = sqrt(pow(space.points[i].x - space.points[pnt_num].x,2) + 
				pow(space.points[i].y - space.points[pnt_num].y,2) + 
				pow(space.points[i].z - space.points[pnt_num].z,2));
			if(current_distance < min_distance){
				min_distance = current_distance;
				point_index = i;
			}
		}
	}
	space.points[point_index].weight += space.points[pnt_num].weight;
	space.points[pnt_num] = space.points[space.size - 1];
	space.size--;
	space.points = (struct point*) realloc(space.points, space.size * sizeof(struct point));
	return space;
}

void printPoints(struct world space){
	for(int i=0 ; i < space.size; i++)
		printf("(%d, %d, %d) weight: %d\n", space.points[i].x, space.points[i].y, space.points[i].z, space.points[i].weight);
	printf("\n");
}

struct world createWorld(int size, int min_w, int max_w, int min_d, int max_d){
	struct world space;
	space.size = size;
	space.MIN_WEIGHT = min_w;
	space.MAX_WEIGHT = max_w;
	space.MIN_DISTANCE = min_d;
	space.MAX_DISTANCE = max_d;
	space = createPoints(space);
	return space;
}
