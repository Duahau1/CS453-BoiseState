CFLAGS= -std=c99 -Wall
LINK= f1.o f2.o f3.o main.o

all: myprog

%.o: %.c myHeader.h
	gcc $(CFLAGS) -c $< -o $@ 

myprog: $(LINK)
	gcc $(CFLAGS) $(LINK) -o myprog

clean:
	rm -f *.o myprog

