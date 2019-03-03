CC=gcc
CFLAGS=-g -Wall

all: keys.o libkeys.a server client
.PHONY: all

server: server.c
	$(CC) $(CFLAGS) -lrt -pthread -o server server.c

keys.o: keys.c
	$(CC) $(CFLAGS) -lrt -c keys.c

libkeys.a: keys.o
	ar -rv libkeys.a keys.o

client: client.c libkeys.a
	$(CC) $(CFLAGS) -lrt -o client client.c libkeys.a

.PHONY: clean
clean:
	rm -rf server client *.o *.a