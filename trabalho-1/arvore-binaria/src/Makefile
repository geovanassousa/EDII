CC=clang
CFLAGS=-Wall -Wextra -O2 -I../includes

OBJS=main.o stream.o categoria.o programa.o apresentador.o utils.o

all: q1

q1: $(OBJS)
	$(CC) $(CFLAGS) -o ../output/q1 $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

stream.o: stream.c
	$(CC) $(CFLAGS) -c stream.c

categoria.o: categoria.c
	$(CC) $(CFLAGS) -c categoria.c

programa.o: programa.c
	$(CC) $(CFLAGS) -c programa.c

apresentador.o: apresentador.c
	$(CC) $(CFLAGS) -c apresentador.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o ../output/q1
