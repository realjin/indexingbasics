all:
	rm -f main
	gcc -g index.c store.c reader_nsf.c main_nsf.c -o main
testonly:
	rm -f test
	gcc -g index.c store.c reader_nsf.c test.c -o test
