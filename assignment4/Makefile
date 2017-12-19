CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
OBJECTS = stack.o priorityQueue.o huffman.o bv.o

.PHONY: all
all: encode decode

encode: $(OBJECTS) encode.o
	$(CC) $(CFLAGS) $(OBJECTS) encode.o -o encode
	
decode: $(OBJECTS) decode.o
	$(CC) $(CFLAGS) $(OBJECTS) decode.o -o decode
	
encode.o: encode.c
	$(CC) $(CFLAGS) encode.c -c 
	
decode.o: decode.c
	$(CC) $(CFLAGS) decode.c -c
	
stack.o: stack.c
	$(CC) $(CFLAGS) stack.c -c
	
bv.o: bv.c
	$(CC) $(CFLAGS) bv.c -c
	
priorityQueue.o: priorityQueue.c
	$(CC) $(CFLAGS) priorityQueue.c -c
	
huffman.o: huffman.c
	$(CC) $(CFLAGS) huffman.c -c
	
.PHONY: clean
clean:
	rm -f $(OBJECTS) decode encode decode.o encode.o
