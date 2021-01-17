#include "world.h"

struct world initiateWorld(struct world space){
	printf("How many points you want to create? : ");
	int size; scanf("%d", &size);
	printf("Enter minimal point's weight: ");
	int min_w; scanf("%d", &min_w);
	printf("Enter maximal point's weight: ");
	int max_w; scanf("%d", &max_w);
	printf("Enter the lower limit of axes: ");
	int min_d; scanf("%d", &min_d);
	printf("Enter the higher limit of axes: ");
	int max_d; scanf("%d", &max_d);
	space = createWorld(size, min_w, max_w, min_d, max_d);
	return space;
}

int main(void){
	int worlds_count;
	struct world* worlds;
	printf("How many worlds you want to create? : ");
	scanf("%d", &worlds_count);
	worlds = (struct world*) malloc(worlds_count * sizeof(struct world));
	for(int i = 0; i < worlds_count; i++){
		printf("World %d:\n", i+1);
		worlds[i] = initiateWorld(worlds[i]);
		printf("World %d successfully created!\n", i+1);
	}
	int answer;	
	while(worlds_count > 0){
		printf("Choose points world you want to launch (1-%d) [0 for exit]: ", worlds_count);
		scanf("%d", &answer); 
		if(answer != 0){
			printf("World %d:\nsize: %d\nminimal point's weight: %d\nmaximal point's weight: %d\nlower limit of axes: %d\nhigher limit of axes: %d\nlife:\n",
			 answer, worlds[answer-1].size, worlds[answer-1].MIN_WEIGHT, worlds[answer-1].MAX_WEIGHT, worlds[answer-1].MIN_DISTANCE,
			  worlds[answer-1].MAX_DISTANCE);
			printPoints(worlds[answer-1]);
			while(worlds[answer-1].size > 1){
				worlds[answer-1] = disappear(worlds[answer-1]);
				printPoints(worlds[answer-1]);
			}
			free(worlds[answer-1].points);
			worlds[answer-1] = worlds[worlds_count-1];
			worlds_count--;
			worlds = (struct world*) realloc(worlds, worlds_count * sizeof(struct world));
		}
		else break;
	}
	for(int i=0; i < worlds_count; i++){
		free(worlds[i].points);
	}
	free(worlds);
	return EXIT_SUCCESS;
}