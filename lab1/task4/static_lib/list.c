#include "include/list.h"

struct list create_list(void* item){
	struct list ls = {(void**)malloc(sizeof(void*)), 1};
	ls.arr[0] = item;
	return ls;
}

void add(struct list* ls, void* item){
	(*ls).size++;
	(*ls).arr = (void**)realloc((*ls).arr, (*ls).size * sizeof(void*));
	(*ls).arr[(*ls).size-1] = item;
}

void insert(struct list* ls, int index, void* item){
	if(index >= 0 && index < (*ls).size){
		(*ls).size++;
		(*ls).arr = (void**)realloc((*ls).arr, (*ls).size * sizeof(void*));
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
		(*ls).arr = (void**)realloc((*ls).arr, (*ls).size * sizeof(void*));		
	}
}

void set(struct list* ls, int index, void* item){
	if(index >= 0 && index < (*ls).size)
		(*ls).arr[index] = item;	
}

void* get(struct list* ls, int index){
	if(index >= 0 && index < (*ls).size)
		return (*ls).arr[index];
}