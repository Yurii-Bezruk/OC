environ: opts.o main.o
	gcc -o environ main.o opts.o
	rm *.o *.gch
main.o: main.c
	gcc -c main.c
opts.o:	opts.c opts.h
	gcc -c opts.c opts.h