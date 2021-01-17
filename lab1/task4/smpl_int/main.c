#include <stdio.h>
#include <stdlib.h>

struct list{
	int* arr;
	int size;
};

struct list create_list(int item){
	struct list ls = {(int*)malloc(sizeof(int)), 1};
	ls.arr[0] = item;
	return ls;
}

void add(struct list* ls, int item){
	(*ls).size++;
	(*ls).arr = (int*)realloc((*ls).arr, (*ls).size * sizeof(int));
	(*ls).arr[(*ls).size-1] = item;
}


void insert(struct list* ls, int index, int item){
	if(index >= 0 && index < (*ls).size){
		(*ls).size++;
		(*ls).arr = (int*)realloc((*ls).arr, (*ls).size * sizeof(int));
		for(int i = (*ls).size-1; i > index; i--){
			(*ls).arr[i] = (*ls).arr[i-1];
		}
		(*ls).arr[index] = item;
	}
}

int size(struct list ls){
	return ls.size;
}

void delete(struct list* ls, int index){
	if(index >= 0 && index < (*ls).size){
		for(int i = index; i < (*ls).size-1; i++)
				(*ls).arr[i] = (*ls).arr[i+1];
		(*ls).size--;
		(*ls).arr = (int*)realloc((*ls).arr, (*ls).size * sizeof(int));		
	}
}

void set(struct list* ls, int index, int item){
	if(index >= 0 && index < (*ls).size)
		(*ls).arr[index] = item;	
}

int get(struct list* ls, int index){
	if(index >= 0 && index < (*ls).size)
		return (*ls).arr[index];
}

void print_list(struct list ls){
	for(int i=0; i<ls.size; i++)
		printf("%d ", ls.arr[i]);
	printf("\n");
}

int main(void){
	int a = 56;
	struct list ls = create_list(a);
	a = 1;
	add(&ls, a);
	print_list(ls);

	free(ls.arr);
	return EXIT_SUCCESS;
}