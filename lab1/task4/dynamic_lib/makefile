task4: path liblist.so
	gcc -o task4 -I ./include -L ./lib main.c -llist -lm
	rm *.o
	echo "finished!"
path:
	export LD_LIBRARY_PATH=~/OC/lab1/task4/dynamic_lib/lib/
liblist.so: list.o
	gcc -shared -o liblist.so list.o
	mv liblist.so ~/OC/lab1/task4/dynamic_lib/lib/
list.o: list.c
	gcc -c -fpic list.c
run: 
	./task4