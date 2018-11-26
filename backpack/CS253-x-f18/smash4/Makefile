
CFLAGS= -std=c99 -Wall
OBJS= history.o

all: smash

%.o : %.c 
	gcc $(CFLAGS) -c -o $@ $<

smashLib.a : $(OBJS)
	ar r $@ $?

smash: smash.o smashLib.a 
	gcc $(CFLAGS) $^ -o $@

debug: CFLAGS += -DDEBUG -g -Og
debug: smash

valgrind: debug
	valgrind  --leak-check=yes --show-leak-kinds=all  smash

clean:
	rm -f *.o *.a smash 
