#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define TRUE 1
#define FALSE 0

struct point{
	int x;
	int y;
	int z;
	int weight;
};
struct world{
	struct point* points;
	int size;
	int MIN_DISTANCE;
	int MAX_DISTANCE;
	int MIN_WEIGHT;
	int MAX_WEIGHT;
};

struct world createPoints(struct world space);
int min_weight_point(struct world space);
struct world disappear(struct world space);
void printPoints(struct world space);

struct world createWorld(int size, int min_w, int max_w, int min_d, int max_d);
struct world initiateWorld(struct world space);