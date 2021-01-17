#include <stdlib.h>

struct list{
	void** arr;
	int size;
};

struct list create_list(void* item);
void add(struct list* ls, void* item);
void insert(struct list* ls, int index, void* item);
int size(struct list ls);
void delete(struct list* ls, int index);
void set(struct list* ls, int index, void* item);
void* get(struct list* ls, int index);