test1: main.o world.o
	gcc -o task0 main.o world.o -lm
	rm *.o *.gch
	./task0
world.o: world.c world.h
	gcc -c world.c world.h
main.o: main.c
	gcc -c main.c
clean:
	rm *.o