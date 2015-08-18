all: ep1

ep1: ep1.c
	gcc -Wall -o ep1 ep1.c

clean: 
	rm -f ep1

