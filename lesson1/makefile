test1: main.o func.o
	gcc -o test1 main.o func.o -lm
func.o: func.c include/func.h
	gcc -c func.c
main.o: main.c
	gcc -c main.c
run: 
	rm -f *.o
	./test1
