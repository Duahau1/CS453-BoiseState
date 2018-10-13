CFLAGS= -std=c99 -Wall


all: smash

smash: 
	gcc $(CFLAGS) smash.c -o smash


debug: CFLAGS += -DDEBUG -g -Og
debug: smash

valgrind: debug
	valgrind --leak-check=yes smash

clean:
	rm -f *.o smash
