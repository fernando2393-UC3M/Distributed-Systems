CC=gcc
CFLAGS=-Wall -g

keys.o: keys.c
	$(CC) $(CFLAGS) -c keys.c

libkeys.a: keys.o
	ar -rv libkeys.a keys.o

server: server.c server.o
	$(CC) $(CFLAGS) -o server server.c server.o

client: client.c keys.a
	$(CC) $(CFLAGS) -o client client.c libkeys.a

.PHONY: clean
clean:
	rm -rf *.o