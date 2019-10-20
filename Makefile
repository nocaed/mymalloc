
all: memgrind

mymalloc: mymalloc.c
	gcc -Wall -g -o mymalloc.c
                                       
memgrind: mymalloc.o
	gcc -Wall -g -o memgrind memgrind.c mymalloc.o

clean:
	rm -f *.o mymalloc memgrind 