CFLAGS= -std=c99 -Wall


all: smash
smash: smash.o history.o
	gcc $(CFLAGS) $^ -o $@

debug: CFLAGS += -DDEBUG -g -Og
debug: smash

valgrind: debug
	valgrind  --leak-check=yes --show-leak-kinds=all  smash

clean:
	rm -f *.o smash
