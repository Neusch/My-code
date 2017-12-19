CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
OBJECTS = hash.o aes.o bv.o linkedList.o banhammer.o lex.yy.o bloomFilter.o


.PHONY: all
all: newspeak

newspeak: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lfl -o banhammer
	
banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c
	
hash_test.o: hash.c 
	$(CC) $(CFLAGS) -c hash.c 
	
aes.o: aes.c
	$(CC) $(CFLAGS) -c aes.c
	
bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c
	
linkedList.o: linkedList.c
	$(CC) $(CFLAGS) -c linkedList.c
	
bloomFilter.o: bloomFilter.c
	$(CC) $(CFLAGS) -c bloomFilter.c
	
lex.yy.o : lex.yy.c
	$(CC) -c lex.yy.c -lfl
# I do not use the CFLAGS identifier because it gives me warnings because i'm using flex

lex.yy.c: words.l
	flex words.l

	
	
.PHONY: clean
clean:
	rm -f $(OBJECTS) banhammer lex.yy.c
	