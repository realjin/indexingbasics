all:
	rm -f main
	gcc -g index.c store.c reader_nsf.c main_nsf.c -o main
testonly:
	rm -f test
	gcc -g -w index.c store.c reader_nsf.c test.c -o test
lib:
	gcc -I../../adt -c -fPIC index.c docvector.c
	gcc -shared -o libindexing.so.1 index.o
installlib:
	sudo cp libindexing.so.1 /opt/lib
	sudo ln -sf /opt/lib/libindexing.so.1 /opt/lib/libindexing.so
