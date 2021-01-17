cond: my_rand.o main.o
	gcc -o cond my_rand.o main.o -lpthread
	rm *.o *.gch
my_rand.o: my_rand.h my_rand.c
	gcc -c my_rand.h my_rand.c
main.o: main.c
	gcc -c main.c