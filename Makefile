FILES  = server.c modules/*.c include/*.h
INPUT  = server.c modules/*.c
CFLAGS = -Wall -g -pthread
OUTPUT = server

all: server

server: $(FILES)
	gcc -o $(OUTPUT) $(CFLAGS) $(INPUT)

clean: 
	rm -f server

