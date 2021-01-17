#include <stdio.h>
#include "include/list.h"

void print_int_list(struct list ls){
	for(int i=0; i<ls.size; i++)
		printf("%d ", *((int*)ls.arr[i]));
	printf("\n");
}

int main(void){
	int a=56,b=67,c=-67,d=9,t=-8;
	//double b = 4.5;
	//char *c = "fff";
	struct list ls = create_list(&a);	
	add(&ls, &b);
	add(&ls, &c);
	print_int_list(ls);	
	insert(&ls, 1, &d);
	print_int_list(ls);
	delete(&ls, 0);
	print_int_list(ls);
	set(&ls, 1, &t);
	print_int_list(ls);
	printf("%d\n", *((int*)get(&ls, 0)));
	/*printf("%.2f\n", *((double*)ls.arr[1]));
	printf("%s\n", *((char**)ls.arr[2]));*/

	free(ls.arr);
	return EXIT_SUCCESS;
}