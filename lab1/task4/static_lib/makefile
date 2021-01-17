task4: main.c liblist.a 
	gcc -o task4 -I ./include -L ./lib main.c -llist -lm
	rm *.o
	echo "finished!"
liblist.a: list.o
	ar r liblist.a list.o
	ranlib liblist.a
	mv liblist.a ./lib
list.o: list.c
	gcc -c list.c
run: 
	./task4